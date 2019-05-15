#pragma once

#include <QObject>
#include <QNetworkReply>
#include <QEventLoop>
#include <QByteArray>
#include <QCryptographicHash>

#include <archive.h>

#include "extractor.h"
#include "error.h"

class ExtractorPrivate : public QObject
{
    Q_OBJECT
public:
    ExtractorPrivate(Extractor *extractor, const QUrl &downloadUrl);
    Q_INVOKABLE void extract();
    static int copy_data(struct archive *ar, struct archive *aw)
    {
        int r;
        const void *buff;
        size_t size;
        int64_t offset;

        for (;;) {
            r = archive_read_data_block(ar, &buff, &size, &offset);
            if ( r == ARCHIVE_EOF )
            {
                return ( ARCHIVE_OK );
            }
            if ( r != ARCHIVE_OK)
            {
                qDebug() << archive_error_string(ar);
                return (r);
            }
            r = archive_write_data_block(aw, buff, size, offset);
            if (r != ARCHIVE_OK)
            {
                qDebug() << archive_error_string(ar);
                return (r);
            }
        }
    }
    void download(const QUrl &url);

private slots:
    void networkError(QNetworkReply::NetworkError);
    void handleRedirect();
    void downloadProgressChanged(qint64 downloaded, qint64 total);

private:
    void setError(Error errorCode, const QString &errorString);
    qint64 readTar(struct archive *ar, const void **archiveBuffer);
    void processMetaData(const QByteArray &metadata, QCryptographicHash &digest, bool isHeader) Q_DECL_NOEXCEPT_EXPR(false);

private:
    Extractor *q;
    QUrl m_url;
    QString m_destinationPath;
    QString m_lastArchiveDirPath;
    std::function<void(const QString &)> m_fileExtractedCallback;
    bool m_failed = false;
    QAtomicInt m_canceled;
    Error m_errorCode = Error::None;
    QString m_errorString;

    QEventLoop m_loop;
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply = nullptr;
    bool m_downloadingFromFIFO = false;
    QByteArray m_buffer;
    qint64 m_downloadTotal = 0;
    qint64 m_bytesReadTotal = 0;
    qint64 m_lastProgress = 0;
    friend class Extractor;
};
