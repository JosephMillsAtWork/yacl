#include "compressor.h"


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
  , m_failed( false )
  , m_errorCode( Error::None )
  , m_entry( nullptr )
  , m_archive( nullptr )
  , m_disk( nullptr )
  , m_format( GNUTAR )
  , m_hasPassword( false )
{
    fillMetadata();}


Compressor::~Compressor()
{
    clearMemory();
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
    return wasCanceled() ? Error::Canceled : ( m_failed ? m_errorCode : Error::None );
}

QString Compressor::errorString() const
{
    return wasCanceled() ?  "canceled"  : ( m_failed ? m_errorString : QString() );
}

Compressor::CompressionType Compressor::format() const
{
    return m_format;
}

void Compressor::setFormat( const Compressor::CompressionType &format )
{
    if ( m_format != format )
    {
        m_format = format;
        emit formatChanged();
    }
}

QString Compressor::sourceDirectory() const
{
    return m_sourceDirectory;
}

void Compressor::setSourceDirectory( const QString &sourceDirectory )
{
    if ( m_sourceDirectory != sourceDirectory )
    {
        m_sourceDirectory = sourceDirectory;
        emit sourceDirectoryChanged();
    }
}

QString Compressor::archiveName() const
{
    return m_archiveName;
}

void Compressor::setArchiveName(const QString &archiveName)
{
    if ( m_archiveName != archiveName )
    {
        m_archiveName = archiveName;
        emit archiveNameChanged();
    }
}

QString Compressor::outputDirectory() const
{
    return m_outputDirectory;
}

void Compressor::setOutputDirectory( const QString &outputDirectory )
{
    if ( m_outputDirectory != outputDirectory )
    {
        m_outputDirectory = outputDirectory;
        emit outputDirectoryChanged();
    }
}

QString Compressor::passwd() const
{
    return m_passwd;
}

void Compressor::setPasswd( const QString &passwd )
{
    if ( m_passwd != passwd )
    {
        m_passwd = passwd;
        emit passwdChanged();
    }
}

bool Compressor::hasPassword() const
{
    return m_hasPassword;
}

void Compressor::setHasPassword( const bool &hasPassword )
{
    if ( m_hasPassword != hasPassword )
    {
        m_hasPassword = hasPassword;
        emit hasPasswordChanged();
    }
}

void Compressor::fillMetadata()
{
    m_comperssionHash.insert( SevenZip , "7zip" );
    m_comperssionHash.insert( AR, "ar" );
    m_comperssionHash.insert( ARBSD, "arbsd" );
    m_comperssionHash.insert( ARGNU, "argnu" );
    m_comperssionHash.insert( ARSVR4, "arsvr4" );
    m_comperssionHash.insert( BSDTAR, "bsdtar" );
    m_comperssionHash.insert( CD9660, "cd9660" );
    m_comperssionHash.insert( CPIO, "cpio" );
    m_comperssionHash.insert( GNUTAR, "gnutar" );
    m_comperssionHash.insert( ISO, "iso" );
    m_comperssionHash.insert( ISO9660, "iso9660" );
    m_comperssionHash.insert( MTREE, "mtree" );
    m_comperssionHash.insert( MTREE_CLASSIC, "mtree-classic" );
    m_comperssionHash.insert( NEWC, "newc" );
    m_comperssionHash.insert( ODC, "odc" );
    m_comperssionHash.insert( OLDTAR, "oldtar" );
    m_comperssionHash.insert( PAX, "pax" );
    m_comperssionHash.insert( PAXR, "paxr" );
    m_comperssionHash.insert( POSIX, "posix" );
    m_comperssionHash.insert( RAW, "raw" );
    m_comperssionHash.insert( RPAX, "rpax" );
    m_comperssionHash.insert( SHAR, "shar" );
    m_comperssionHash.insert( SHARDUMP, "shardump" );
    m_comperssionHash.insert( USTAR, "ustar" );
    m_comperssionHash.insert( V7TAR, "v7tar" );
    m_comperssionHash.insert( V7, "v7" );
    m_comperssionHash.insert( WARC, "warc" );
    m_comperssionHash.insert( XAR, "xar" );
    m_comperssionHash.insert( ZIP, "zip" );

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

void Compressor::cancel()
{
    m_canceled.fetchAndStoreOrdered( 1 );
}

bool Compressor::create()
{
    bool ret = false;
    int i = 0;
    char buff[ 64 * 1024 ];
    long long len;
    int r;
    clearMemory();

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
            break;
        }

        QString fileName =  QString::fromUtf8( archive_entry_pathname( m_entry ) );

        QFileInfo fileInfo( fileName );
        if( !fileInfo.isDir()  )
        {
            QString testPath = QString( targetDir.relativeFilePath( fileInfo.absoluteFilePath() ) )
                    .prepend( targetDir.dirName().append("/") );

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
                    archive_write_data( m_archive, buff, static_cast<size_t>( len ) );
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
            archive_read_disk_descend( m_disk );
        }
    }

    if( m_hasPassword && m_passwd.size() > 0 )
    {
        // this does not seem to be working ...
        QString passwd = QString( "password=%1" ).arg( m_passwd );
        archive_write_set_passphrase( m_disk, passwd.toLatin1().data( ) );
    }

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


QString Compressor::getSuffix(const CompressionType &type )
{
    QString ret;
    switch ( type )
    {
    case SevenZip:
        ret = "7z";
        break;
    case AR :
    case ARBSD:
    case ARSVR4 :
    case ARGNU :
    case WARC:
        ret = "ar";
        break;
    case CPIO:
        ret = "cpio";
        break;
    case GNUTAR:
    case BSDTAR:
    case USTAR:
    case OLDTAR:
    case V7TAR:
    case V7:
        ret = "tar.gz";
        break;
    case ISO:
    case ISO9660:
    case CD9660:
        ret = "iso";
        break;
    case NEWC:
        ret = "nc";
        break;
    case PAX:
    case PAXR:
    case POSIX:
    case RPAX:
        ret = "pax";
        break;
    case RAW:
        ret = "raw";
        break;
    case SHAR:
    case SHARDUMP:
        ret = "shar";
        break;
    case XAR:
        ret = "xar";
        break;
    case MTREE:
    case MTREE_CLASSIC:
    case ODC:
    case ZIP:
        ret = "zip";
        break;
    }
    return ret;
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
                .arg( getSuffix( m_format ) );
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

void Compressor::clearMemory()
{
    m_entry = nullptr;
    m_archive = nullptr;
    m_disk = nullptr;
}
