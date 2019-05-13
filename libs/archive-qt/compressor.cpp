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
  , m_entry( nullptr )
  , m_archive( nullptr )
  , m_disk( nullptr )
  , m_format( GNUTAR )
  , m_hasPassword( false )
{

    // later the hash should look like so. this way we can get the suffix
    //QPair<QString, QString>( "7zip", "7z");

    m_comperssionHash.insert( SevenZip , "7zip");
    m_comperssionHash.insert( AR, "ar");
    m_comperssionHash.insert( ARBSD, "arbsd");
    m_comperssionHash.insert( ARGNU, "argnu");
    m_comperssionHash.insert( ARSVR4, "arsvr4");
    m_comperssionHash.insert( BSDTAR, "bsdtar");
    m_comperssionHash.insert( CD9660, "cd9660");
    m_comperssionHash.insert( CPIO, "cpio");
    m_comperssionHash.insert( GNUTAR, "gnutar");
    m_comperssionHash.insert( ISO, "iso");
    m_comperssionHash.insert( ISO9660, "iso9660");
    m_comperssionHash.insert( MTREE, "mtree");
    m_comperssionHash.insert( MTREE_CLASSIC, "mtree-classic");
    m_comperssionHash.insert( NEWC, "newc");
    m_comperssionHash.insert( ODC, "odc");
    m_comperssionHash.insert( OLDTAR, "oldtar");
    m_comperssionHash.insert( PAX, "pax");
    m_comperssionHash.insert( PAXR, "paxr");
    m_comperssionHash.insert( POSIX, "posix");
    m_comperssionHash.insert( RAW, "raw");
    m_comperssionHash.insert( RPAX, "rpax");
    m_comperssionHash.insert( SHAR, "shar");
    m_comperssionHash.insert( SHARDUMP, "shardump");
    m_comperssionHash.insert( USTAR, "ustar");
    m_comperssionHash.insert( V7TAR, "v7tar");
    m_comperssionHash.insert( V7, "v7");
    m_comperssionHash.insert( WARC, "warc");
    m_comperssionHash.insert( XAR, "xar");
    m_comperssionHash.insert( ZIP, "zip");

    if( !m_vaildSuffix.isEmpty() )
    {
        m_vaildSuffix.clear();
    }
    QHashIterator<CompressionType, QString> cIt = m_comperssionHash;
    while (cIt.hasNext())
    {
        cIt.next();
        m_vaildSuffix.append( cIt.value() );
    }
}

Compressor::~Compressor()
{
    m_entry = nullptr;
    m_archive = nullptr;
    m_disk = nullptr;
}

bool Compressor::compress()
{
    if ( !wasCanceled() )
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
    m_canceled.fetchAndStoreOrdered( 1 );
}

bool Compressor::create()
{
    bool ret = false;
    int i = 0;
    char buff[ 64 * 1024 ];
    size_t len;
    int r;
    m_archive = nullptr;
    m_entry = nullptr;
    m_disk = nullptr;

    if( !sanitize() )
    {
        qDebug() << "Failed to sanitize compression";
        return false;
    }

    m_TemporaryFile.setFileName( m_archiveAbsoluteFilePath );

    if( !m_TemporaryFile.open( QIODevice::WriteOnly ) )
    {
        return false;
    }

    m_archive = archive_write_new();
    archive_write_add_filter_none( m_archive );
    qDebug() << "TYPE " << m_comperssionHash.value( m_format ).toLatin1().data();

    archive_write_set_format_by_name( m_archive, m_comperssionHash.value( m_format ).toLatin1().data() );

    if( archive_write_open_fd( m_archive, m_TemporaryFile.handle() ) != ARCHIVE_OK )
    {
        if( m_archive )
        {
            archive_write_free( m_archive );
        }
        return false;
    }

    QDir targetDir( m_sourceDirectory );
    m_disk = archive_read_disk_new();
    archive_read_disk_set_standard_lookup( m_disk );
    r = archive_read_disk_open( m_disk, QFile::encodeName( m_sourceDirectory ).constData() );
    while ( i == 0 )
    {
        m_entry = archive_entry_new();
        r = archive_read_next_header2( m_disk, m_entry );
        if ( r == ARCHIVE_EOF )
        {
            i++;
            break;
        }
        if ( r != ARCHIVE_OK )
        {
            qDebug() << "archive_read_next_header2( disk, m_entry ) ERROR";
            i = 1;
        }

        QString fileName =  QString::fromUtf8( archive_entry_pathname( m_entry ) );

        QFileInfo fileInfo( fileName );
        qDebug() << "NAME " << fileInfo.fileName();
        if( !fileInfo.isDir()  )
        {
            QString testPath = QString( targetDir.relativeFilePath( fileInfo.absoluteFilePath() ) )
                    .prepend( targetDir.dirName().append("/") );

            qDebug() << "SAVE PATH " << testPath;
            archive_entry_set_pathname( m_entry, testPath.toLatin1().data() );
            r = archive_write_header( m_archive, m_entry );

            if ( r > ARCHIVE_FAILED )
            {
                QFile file( fileInfo.absoluteFilePath() );
                if( !file.open( QIODevice::ReadOnly ) )
                {
                    if( m_entry )
                    {
                        archive_entry_free( m_entry );
                    }
                    if ( m_archive )
                    {
                        archive_write_free( m_archive );
                    }
                    return false;
                }
                len = file.read( buff, sizeof( buff ) );
                while( len > 0 )
                {
                    archive_write_data( m_archive, buff, len );
                    len = file.read( buff, sizeof( buff ) );
                }
                archive_read_disk_descend( m_disk );
                file.close();
            }
            else
            {
                qDebug() << "This sucks" << r;
            }
        }
        else
        {
            qDebug() << "Is Directory skiping";
            archive_read_disk_descend( m_disk );
        }
    }

//    if( m_hasPassword && m_passwd.size() > 0 )
//    {
//        archive_write_set_passphrase( m_disk, m_passwd.toLatin1().data( ) );
//        archive_write_set_passphrase( m_archive, m_passwd.toLatin1().data( ) );
//    }

    if( m_entry )
    {
        archive_entry_free( m_entry );
        archive_write_finish_entry( m_archive );
    }
    if( m_disk )
    {
        archive_read_free( m_disk );
    }
    if( m_archive )
    {
        archive_write_close( m_archive );
        archive_write_free( m_archive );
    }
    m_TemporaryFile.commit();
    ret = true;


    if( m_disk )
    {
        archive_read_free( m_disk );
    }

    if ( m_archive )
    {
        archive_write_free( m_archive );
    }
    return ret;
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
        switch ( m_format )
        {
//        case GZIP :
//            ext = "tar.gz";
//            break;
//        case XZ :
//            ext = "xz";
//            break;
//        case ZSTD:
//            ext = "zstd";
//            break;
//        case BZIP:
//            ext = "bzip2";
//            break;
//        case ZIP:
//            ext = "zip";
//            break;
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

//    abInfo = QFileInfo( m_archiveAbsoluteFilePath );
//    if( !m_vaildSuffix.contains( abInfo.suffix() ) )
//    {
//        QString vsStr = "unvaild suffix/format/compression type vaild formates are ";
//        for( const QString s : m_vaildSuffix )
//        {
//            vsStr.append( s );
//        }
//        qDebug() << vsStr;
//        return false;
//    }
    return true;
}
