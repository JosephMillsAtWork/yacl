#include <QStringList>
#include <QThread>
#include <QAtomicInt>
#include <QFile>
#include <QDir>
#include <QRegExp>
#include <QDataStream>
#include <QUrl>
#include <QDebug>
#include <QCryptographicHash>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <archive.h>
#include <archive_entry.h>

#include "extractor.h"
#include "extractor_p.h"
#include "exception.h"
#include "error.h"

// these are not defined on all platforms
#ifndef S_IREAD
#  define S_IREAD S_IRUSR
#endif
#ifndef S_IEXEC
#  define S_IEXEC S_IXUSR
#endif



Extractor::Extractor( const QUrl &downloadUrl, const QDir &destinationDir, QObject *parent )
    : QObject( parent )
    , d( new ExtractorPrivate( this, downloadUrl ) )
{
    setDestinationDirectory( destinationDir );
}

QDir Extractor::destinationDirectory() const
{
    return QDir( d->m_destinationPath );
}
QString Extractor::lastArchiveDirPath() const
{
    return QString( d->m_lastArchiveDirPath );
}

void Extractor::setDestinationDirectory( const QDir &destinationDir )
{
    d->m_destinationPath = destinationDir.absolutePath() + qL1C( '/' );
}

void Extractor::setFileExtractedCallback( const std::function<void( const QString & )> &callback )
{
    d->m_fileExtractedCallback = callback;
}

bool Extractor::extract()
{
    if( !wasCanceled() )
    {
        d->m_failed = false;
        d->download( d->m_url );
        QMetaObject::invokeMethod( d, "extract", Qt::QueuedConnection );
        d->m_loop.exec();

        delete d->m_reply;
        d->m_reply = Q_NULLPTR;
    }
    return !wasCanceled() && !hasFailed();
}

bool Extractor::hasFailed() const
{
    return d->m_failed || wasCanceled();
}

bool Extractor::wasCanceled() const
{
    return d->m_canceled.load();
}

Error Extractor::errorCode() const
{
    return wasCanceled() ? Error::Canceled : ( d->m_failed ? d->m_errorCode : Error::None );
}

QString Extractor::errorString() const
{
    return wasCanceled() ? qSL( "canceled" ) : ( d->m_failed ? d->m_errorString : QString() );
}

/*! \internal
  This function can be called from another thread while extract() is running
*/
void Extractor::cancel()
{
    if( !d->m_canceled.fetchAndStoreOrdered( 1 ) )
    {
        if( d->m_loop.isRunning() )
        {
            d->m_loop.wakeUp();
        }
    }
}


/* * * * * * * * * * * * * * * * * * *
 *  vvv ExtractorPrivate vvv  *
 * * * * * * * * * * * * * * * * * * */

ExtractorPrivate::ExtractorPrivate( Extractor *extractor, const QUrl &downloadUrl )
    : QObject( extractor )
    , q( extractor )
    , m_url( downloadUrl )
    , m_nam( new QNetworkAccessManager( this ) )
{
    m_buffer.resize( 64 * 1024 );
}

qint64 ExtractorPrivate::readTar( struct archive *ar, const void **archiveBuffer )
{
    forever
    {
        // the event loop is gone
        if( !m_loop.isRunning() )
        {
            archive_set_error( ar, -1, "no eventlopp" );
            return -1;
        }
        // we have been canceled
        if( q->wasCanceled() )
        {
            archive_set_error( ar, -1, "canceled" );
            return -1;
        }
        qint64 bytesAvailable = m_reply->bytesAvailable();

        // there is something to read
        // (or this is a FIFO and we need this ugly hack - for testing only though!)
        if( ( bytesAvailable > 0 ) || m_downloadingFromFIFO )
        {
            qint64 bytesRead = m_reply->read( m_buffer.data(), m_buffer.size() );

            if( bytesRead < 0 )
            {
                // another FIFO hack: if the writer dies, we will get an -1 return from read()
                if( m_downloadingFromFIFO && m_reply->atEnd() )
                {
                    return 0;
                }
                else
                {
                    archive_set_error( ar, -1, "could not read from tar archive" );
                    return -1;
                }
            }

            m_bytesReadTotal += bytesRead;
            *archiveBuffer = m_buffer.constData();

            qint64 progress = m_downloadTotal ? ( 100 * m_bytesReadTotal / m_downloadTotal ) : 0;
            if( progress != m_lastProgress )
            {
                emit q->progress( qreal( progress ) / 100 );
                m_lastProgress = progress;
            }

            return bytesRead;
        }

        // got an error while reading
        if( m_reply->error() != QNetworkReply::NoError )
        {
            archive_set_error( ar, -1, "%s", m_reply->errorString().toLocal8Bit().constData() );
            return -1;
        }

        // we're done
        if( m_reply->isFinished() )
        {
            return 0;
        }

        m_loop.processEvents( QEventLoop::WaitForMoreEvents );
    }
}

// to do pass in flags and certian keywords
void ExtractorPrivate::extract()
{
    int r;
    int flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;
    struct archive *ar = nullptr;
    struct archive *ext = nullptr;
    try
    {
        ar = archive_read_new();

        ext = archive_write_disk_new();
        archive_write_disk_set_options( ext, flags );
        archive_write_disk_set_standard_lookup( ext );
        if( !ar )
        {
            qDebug() << archive_format_name( ar );
            throw Exception( "[libarchive] could not create a new archive object" );
        }
        if( archive_read_support_format_zip_streamable( ar ) != ARCHIVE_OK )
        {
            throw ArchiveException( ar, "could not enable format_zip_streamable support" );
        }
        if( archive_read_support_format_tar( ar ) != ARCHIVE_OK )
        {
            throw ArchiveException( ar, "could not enable TAR support" );
        }
        if( archive_read_support_filter_gzip( ar ) != ARCHIVE_OK )
        {
            throw ArchiveException( ar, "could not enable GZIP support" );
        }
        if( archive_read_support_filter_bzip2( ar ) != ARCHIVE_OK )
        {
            qDebug() << "there is no support for bzip2 ";
        }
        if( archive_read_support_filter_lzma( ar ) != ARCHIVE_OK )
        {
            qDebug() << "there is no support for lzma ";
        }
        if( archive_read_support_filter_xz( ar ) != ARCHIVE_OK )
        {
            qDebug() << "there is no support for xz ";
        }
        if( archive_read_support_filter_lzip( ar ) != ARCHIVE_OK )
        {
            qDebug() << "there is no support for zip ";
        }
        if( archive_read_support_format_cpio( ar )  != ARCHIVE_OK )
        {
            qDebug() << "there is no support for cpio  ";
        }
        if( archive_read_support_filter_zstd( ar )  != ARCHIVE_OK )
        {
            qDebug() << "there is no support for ZSTD  ";
        }
        //        if ( archive_read_support_filter_rpm( ar )!= ARCHIVE_OK )
        //        {
        //            qDebug() << "there is no support for rpm ";
        //        }
        //        if ( archive_read_support_filter_lzop( ar) != ARCHIVE_OK )
        //        {
        //            qDebug() << "there is no support for lzop ";
        //        }

#ifndef Q_OS_ANDROID
        if( archive_read_set_options( ar, "hdrcharset=UTF-8" ) != ARCHIVE_OK )
        {
            throw ArchiveException( ar, "could not set the HDRCHARSET option" );
        }
#endif

        //        int r = archive_read_open_filename(a, "archive.tar", 10240); // Note 1

        auto dummyCallback = []( archive *, void * )
        {
            return ARCHIVE_OK;
        };
        auto readCallback = []( archive * ar, void *user, const void **buffer )
        {
            return static_cast<__LA_SSIZE_T>(
                       static_cast<ExtractorPrivate *>( user )->readTar( ar, buffer )
                   );
        };


        if( archive_read_open( ar, this, dummyCallback, readCallback, dummyCallback ) != ARCHIVE_OK )
        {
            qDebug() << archive_format_name( ar ) ;
            qDebug() << archive_error_string( ar ) << archive_format( ar );
            throw ArchiveException( ar, "could not open archive" );
        }

        // Iterate over all entries in the archive
        int i = 0;
        for( bool finished = false; !finished; )
        {

            archive_entry *entry = Q_NULLPTR;
            // Try to read the next entry from the archive
            switch( archive_read_next_header( ar, &entry ) )
            {
                case ARCHIVE_EOF:
                {
                    finished = true;
                    continue;
                }
                case ARCHIVE_OK:
                {
                    QString entryPath = QString::fromWCharArray( archive_entry_pathname_w( entry ) )
                                        .normalized( QString::NormalizationForm_C ) ;
                    QString  finalPath = QString( "%1%2" ).arg( m_destinationPath ).arg( entryPath );

                    archive_entry_update_pathname_utf8( entry, finalPath.toUtf8().data() );

                    if( i == 0 )
                    {
                        m_lastArchiveDirPath = finalPath;
                        qDebug() << "WHERE ITS GOING " << finalPath;
                    }

                    r = archive_write_header( ext, entry );

                    if( r != ARCHIVE_OK )
                    {
                        throw ArchiveException( ar, archive_error_string( ar ) );
                    }
                    else
                    {
                        r = copy_data( ar, ext );
                        if( r != ARCHIVE_OK )
                        {
                            throw ArchiveException( ar, "could not copy data !" );
                        }
                    }
                    break;
                }
                default:
                {
                    archive_write_free( ext );
                    throw ArchiveException( ar, "could not read header" );
                }
            }
            i = i + 1;
            emit q->progress( 1 );
        }
        archive_read_close( ar );
    }
    catch( const Exception &e )
    {
        if( !q->wasCanceled() )
        {
            setError( e.errorCode(), e.errorString() );
        }
    }

    if( ar )
    {
        archive_read_free( ar );
    }
    if( ext )
    {
        archive_write_free( ext );
    }
    m_loop.quit();
}

void ExtractorPrivate::setError( Error errorCode, const QString &errorString )
{
    m_failed = true;
    // only the first error is the one that counts!
    if( m_errorCode == Error::None )
    {
        m_errorCode = errorCode;
        m_errorString = errorString;
    }
}


void ExtractorPrivate::download( const QUrl &url )
{
    qDebug() << url;
    QNetworkRequest request;
    if( url.isValid() )
    {
        request.setUrl( url );
        m_reply = m_nam->get( request );

        connect( m_reply, static_cast<void ( QNetworkReply::* )( QNetworkReply::NetworkError )>( &QNetworkReply::error ),
                 this, &ExtractorPrivate::networkError );

        connect( m_reply, &QNetworkReply::metaDataChanged,
                 this, &ExtractorPrivate::handleRedirect );

        connect( m_reply, &QNetworkReply::downloadProgress,
                 this, &ExtractorPrivate::downloadProgressChanged );

    }
}

void ExtractorPrivate::networkError( QNetworkReply::NetworkError )
{
    setError( Error::Network,
              qobject_cast<QNetworkReply *>( sender() )->errorString()
            );
    QMetaObject::invokeMethod( &m_loop, "quit", Qt::QueuedConnection );
}
// this should never happen
void ExtractorPrivate::handleRedirect()
{
    int status = m_reply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
    if( ( status >= 300 ) && ( status < 400 ) )
    {
        QUrl url = m_reply->attribute( QNetworkRequest::RedirectionTargetAttribute ).toUrl();
        m_reply->disconnect();
        m_reply->deleteLater();
        QNetworkRequest request( url );
        m_reply = m_nam->get( request );
    }
}

void ExtractorPrivate::downloadProgressChanged( qint64 downloaded, qint64 total )
{
    qDebug() << "reading compression:" << downloaded << " mm " << total;
    m_downloadTotal = total;
}

void ExtractorPrivate::processMetaData( const QByteArray &metadata, QCryptographicHash &digest,
        bool isHeader ) Q_DECL_NOEXCEPT_EXPR( false )
{
    Q_UNUSED( digest )
    qDebug() << metadata  << isHeader;
    // version and shit like that goes here
}

