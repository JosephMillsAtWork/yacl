#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
#include <QtNetwork>
#include <QtCore>
#include <QUrl>
#include <QQueue>
#include <QTime>

class DownloadManager: public QObject
{
    Q_OBJECT
public:
    explicit DownloadManager(QObject *parent = nullptr);

    void append(const QUrl &url);
    void append(const QStringList &urls);
    static QString saveFileName(const QUrl &url);

signals:
    void finished();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    bool isHttpRedirect() const;
    void reportRedirect();

    QNetworkAccessManager manager;
    QQueue<QUrl> downloadQueue;
    QNetworkReply *currentDownload = nullptr;
    QFile output;
    QTime downloadTime;

    int downloadedCount = 0;
    int totalCount = 0;
};

#endif
