/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "downloadmanager.h"
#include <QTextStream>
#include <cstdio>

using namespace std;

DownloadManager::DownloadManager(QObject *parent)
    : QObject(parent)
{
}

void DownloadManager::append(const QStringList &urls)
{
    for ( const QString &urlAsString : urls )
        append( QUrl::fromEncoded( urlAsString.toLocal8Bit() ) );

    if ( downloadQueue.isEmpty() )
        QTimer::singleShot( 0, this, SIGNAL( finished() ) );
}

void DownloadManager::append( const QUrl &url )
{
    if ( downloadQueue.isEmpty() )
        QTimer::singleShot( 0, this, SLOT( startNextDownload() ) );

    downloadQueue.enqueue( url );
    ++totalCount;
}

QString DownloadManager::saveFileName( const QUrl &url )
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if ( basename.isEmpty() )
        basename = "download";

    if ( QFile::exists( basename ) )
    {
        if( !QFile::remove( basename ) )
        {
            int i = 0;
            basename += '.';
            while (QFile::exists(basename + QString::number(i)))
                ++i;
            basename += QString::number(i);
        }
        else
        {
            qDebug() << "could not overwrite "<< QFileInfo(path).fileName() << "added a number to it";
        }
    }

    return basename;
}

void DownloadManager::startNextDownload()
{
    if ( downloadQueue.isEmpty())
    {
        printf("%d/%d files downloaded successfully\n", downloadedCount, totalCount);
        emit finished();
        return;
    }

    QUrl url = downloadQueue.dequeue();





    QString filename = saveFileName( url );
    output.setFileName( filename );
    if ( !output.open( QIODevice::WriteOnly ) )
    {
        fprintf(stderr, "Problem opening save file '%s' for download '%s': %s\n",
                qPrintable(filename), url.toEncoded().constData(),
                qPrintable(output.errorString()));

        startNextDownload();
        return;                 // skip this download
    }

    // make sure that we sanitize for local file
    if ( url.scheme().isEmpty()  )
    {
        url.setScheme("file");
        url.toLocalFile();
    }


    QNetworkRequest request(url);

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    currentDownload = manager.get(request);

    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)),
            SLOT(downloadProgress(qint64,qint64)));

    connect(currentDownload, SIGNAL(finished()),
            SLOT(downloadFinished()));

    connect(currentDownload, SIGNAL(readyRead()),
            SLOT(downloadReadyRead()));

    // prepare the output
    printf("Downloading %s...\n", url.toEncoded().constData());
    downloadTime.start();
}

void DownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    // calculate the download speed
    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
    QString unit;
    if (speed < 1024)
    {
        unit = "bytes/sec";
    }
    else if (speed < 1024*1024)
    {
        speed /= 1024;
        unit = "kB/s";
    }
    else
    {
        speed /= 1024*1024;
        unit = "MB/s";
    }
    qDebug() << QString::fromLatin1("%1MB of %2MB avg: %3 %4")
                .arg( bytesReceived / 1024*1024)
                .arg( bytesTotal / 1024*1024)
                .arg( speed, 3, 'f', 1 )
                .arg( unit );
}

void DownloadManager::downloadFinished()
{
    output.close();



    if ( currentDownload->error() )
    {
        qDebug() << currentDownload->url();
        fprintf( stderr, "Failed: %s\n", qPrintable( currentDownload->errorString() ) );

        qDebug() << currentDownload->error();

//        output.remove();
    }
    else
    {
        // let's check if it was actually a redirect
        if ( isHttpRedirect() )
        {
            reportRedirect();
            output.remove();
        } else {
            printf("Succeeded.\n");
            ++downloadedCount;
        }
    }
    currentDownload->deleteLater();
    startNextDownload();
}

void DownloadManager::downloadReadyRead()
{
    output.write( currentDownload->readAll() );
}

bool DownloadManager::isHttpRedirect() const
{
    int statusCode = currentDownload->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::reportRedirect()
{ 
        int statusCode = currentDownload->attribute( QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QUrl requestUrl = currentDownload->request().url();

        QTextStream(stderr) << "Request: " << requestUrl.toDisplayString()
                            << " was redirected with code: " << statusCode
                            << '\n';

        QUrl url = currentDownload->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if ( !url.isValid() )
        {
            return;
        }
        if( url.isRelative() )
        {
            url = requestUrl.resolved( url );
        }
        QTextStream(stderr) << "Redirected to: " << url.toDisplayString() << '\n';
}
