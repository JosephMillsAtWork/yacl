#include <archive.h>
#include <archive_entry.h>
#include <QStandardPaths>
#include "compressor.h"
#include "exception.h"
#include "error.h"

// archive.h might #define this for Android
#ifdef open
#  undef open
#endif
// these are not defined on all platforms
#ifndef S_IREAD
#  define S_IREAD S_IRUSR
#endif
#ifndef S_IEXEC
#  define S_IEXEC S_IXUSR
#endif

Compressor::Compressor( QObject *parent) :
      QObject( parent )
    , m_format( GZIP )
{
    m_entry = nullptr;
    m_archive = nullptr;
    m_vaildSuffix = QStringList() << "tar" << "gz" << "tar.gz"
                                                                   << "xz"
                                                                   << "zstd"
                                                                   << "bzip"
                                                                   << "bzip2"
                                                                   << "zip" ;
}

Compressor::~Compressor()
{
    m_entry = nullptr;
    m_archive = nullptr;
}

bool Compressor::compress()
{
    if (!wasCanceled())
    {
        create();
    }

    return !wasCanceled() && !hasFailed();
}

bool Compressor::hasFailed() const
{
    return m_failed || wasCanceled();
}

bool Compressor::wasCanceled() const
{
    return m_canceled.load();
}

Error Compressor::errorCode() const
{
    return wasCanceled() ? Error::Canceled : (m_failed ? m_errorCode : Error::None);
}

QString Compressor::errorString() const
{
    return wasCanceled() ? qSL("canceled") : (m_failed ? m_errorString : QString());
}

/*! \internal
  This function can be called from another thread while create() is running
*/
void Compressor::cancel()
{
    m_canceled.fetchAndStoreOrdered(1);
}

bool Compressor::create()
{
    char buff[ 64 * 1024 ];
    size_t len;
    int r;
    m_archive = nullptr;
    m_entry = nullptr;
    struct archive *disk = nullptr;

    if( !sanitize() )
    {
        qDebug() << "Failed to sanitize compression";
        return false;
    }

    char *outDirectory = m_archiveAbsoluteFilePath.toLatin1().data();
    qDebug() << outDirectory;

    char *inputDirectory = m_sourceDirectory.toLatin1().data();
    qDebug() << inputDirectory;

    try {
        // Add all files
        qDebug() << "iter" << m_sourceDirectory;
        m_TemporaryFile.setFileName( m_archiveAbsoluteFilePath );
        /* Open Temporary file for write. */
        if(!m_TemporaryFile.open( QIODevice::WriteOnly ) )
        {
            //          emit error(ArchiveWriteOpenError, m_TemporaryFile->fileName());
            return false;
        }


        m_archive = archive_write_new();
        archive_write_add_filter_none(m_archive);
        archive_write_set_format_ustar(m_archive);

        /* Finally open the write archive using the handle of the Temporary file. */
        if( archive_write_open_fd(m_archive, m_TemporaryFile.handle() ) != ARCHIVE_OK )
        {

            //           m_ArchiveWrite.clear();
            //           emit error(ArchiveWriteOpenError, m_TemporaryFile->fileName());
            if(m_archive)
            {
                archive_write_free(m_archive);
            }
            return false;
        }

        disk = archive_read_disk_new();
        archive_read_disk_set_standard_lookup( disk );
        r = archive_read_disk_open( disk, QFile::encodeName( m_sourceDirectory ).constData() );
        QDir targetDir( m_sourceDirectory );
        QDirIterator it( m_sourceDirectory, QDir::AllEntries | QDir::System | QDir::NoDotAndDotDot | QDir::Hidden, QDirIterator::Subdirectories );
        while ( it.hasNext() )
        {
            it.next();
            QFileInfo itFi = it.fileInfo();
            if( !itFi.isDir() )
            {
                m_entry = archive_entry_new();
                r = archive_read_next_header2( disk, m_entry );
                if ( r == ARCHIVE_EOF )
                {
                     break;
                }
                if (r != ARCHIVE_OK) {
                    qDebug() << "archive_read_next_header2( disk, m_entry ) ERROR";
                    return false;
                }

                archive_read_disk_descend( disk );
                QString testPath = QString(
                            targetDir.relativeFilePath( itFi.absoluteFilePath() )
                            ).prepend( targetDir.dirName().append("/") );
                archive_entry_set_pathname( m_entry, testPath.toLatin1().data() );

                archive_read_disk_descend( disk );
                r = archive_write_header( m_archive, m_entry );

                if ( r > ARCHIVE_FAILED )
                {
                    QFile file( it.fileInfo().absoluteFilePath() );
                    if( !file.open(QIODevice::ReadOnly ) )
                    {
                        if( m_entry )
                            archive_entry_free( m_entry );
                        if ( m_archive )
                            archive_write_free( m_archive );

                        return false;
                    }
                    len = file.read( buff, sizeof( buff ) );
                    while( len > 0 )
                    {
                        archive_write_data( m_archive, buff, len );
                        len = file.read( buff, sizeof( buff ) );
                    }
                    file.close();
                }
            }
        }


        if( m_entry )
        {
            archive_entry_free( m_entry );
            archive_write_finish_entry( m_archive );
        }
        if( disk )
        {
            archive_read_free( disk );
        }
        if( m_archive )
        {
            archive_write_close( m_archive );
            archive_write_free( m_archive );
        }
        m_TemporaryFile.commit();
        return true;

    } catch (const Exception &e) {
        if (!wasCanceled())
            setError(e.errorCode(), e.errorString());
    }

    if( disk )
        archive_read_free( disk );

    if ( m_archive )
        archive_write_free( m_archive );



    return false;
}

void Compressor::setError(Error errorCode, const QString &errorString)
{
    m_failed = true;

    // only the first error is the one that counts!
    if ( m_errorCode == Error::None ) {
        m_errorCode = errorCode;
        m_errorString = errorString;
    }
}


/*!

   \param file
   \return returns the suffixs of the file. This is not meant to be called outside of sanitize
 */
QString Compressor::getSuffix( const QFileInfo &file )
{
    QString ext = file.suffix().toLower();
    if( ext.isEmpty() )
    {

        switch (m_format )
        {
        case GZIP :
            ext = "tar.gz";
            break;
        case XZ :
            ext = "xz";
            break;
        case ZSTD:
            ext = "zstd";
            break;
        case BZIP:
            ext = "bzip2";
            break;
        case ZIP:
            ext = "zip";
            break;
        default:
            ext = QString();
            break;
        }
    }
    return ext;
}

bool Compressor::sanitize()
{
    if( m_outputDirectory.isEmpty() )
    {
        m_outputDirectory = QStandardPaths::writableLocation( QStandardPaths::TempLocation );
    }

    QDir dc( m_outputDirectory );
    if( !dc.exists() )
    {
        if( dc.mkpath( m_outputDirectory ) )
        {
            qDebug() <<  "could not enable format_zip_streamable support";
            return false;
        }
    }

    QDir srcDir( m_sourceDirectory );
    if( m_sourceDirectory.isEmpty() )
    {
        qDebug() << "Source directory is empty";
        return false;
    }
    else
    {
        if( !srcDir.exists() )
        {
            qDebug() << "Source directory does not exist";
            return false;
        }

    }

    if( m_archiveName.isEmpty() )
    {
        m_archiveName = srcDir.dirName();
        if( m_archiveName.isEmpty() )
        {
            qDebug() << "Could not guess the name of the archive.";
            return false;
        }
        else
        {
            qDebug() << "Setting archive name to " << m_archive;
        }
    }



    m_archiveAbsoluteFilePath = QString("%1/%2")
            .arg( m_outputDirectory )
            .arg( m_archiveName );
    QFileInfo abInfo = QFileInfo( m_archiveAbsoluteFilePath );
    if( abInfo.suffix().isEmpty() )
    {
        m_archiveAbsoluteFilePath = QString("%1/%2.%3")
                .arg( m_outputDirectory )
                .arg( m_archiveName )
                .arg( getSuffix( abInfo ) );
    }

    abInfo = QFileInfo( m_archiveAbsoluteFilePath );
    if( !m_vaildSuffix.contains( abInfo.suffix() ) )
    {
        QString vsStr = "unvaild suffix/format/compression type vaild formates are ";
        for( const QString s : m_vaildSuffix )
        {
            vsStr.append( s );
        }
        qDebug() << vsStr;
        return false;
    }
    return true;
}
