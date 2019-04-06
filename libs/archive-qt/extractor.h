#pragma once
#include <QObject>
#include <functional>
#include "error.h"
#include <QUrl>
#include <QDir>
#include <QCryptographicHash>

class ExtractorPrivate;
class Extractor : public QObject {
    Q_OBJECT
public:
    Extractor( const QUrl &downloadUrl, const QDir &destinationDir, QObject *parent = nullptr );
    QDir destinationDirectory() const;
    QString lastArchiveDirPath() const;
    void setDestinationDirectory( const QDir &destinationDir );
    void setFileExtractedCallback( const std::function<void( const QString & )> &callback );
    bool extract();
    bool hasFailed() const;
    bool wasCanceled() const;
    Error errorCode() const;
    QString errorString() const;
public slots:
    void cancel();

signals:
    void progress( qreal progress );

private:
    ExtractorPrivate *d;
    friend class ExtractorPrivate;
};
