#ifndef DECOMPRESSOR_H
#define DECOMPRESSOR_H

#include <QObject>
#include <QUrl>
#include <QDir>
#include <QCryptographicHash>
#include <QDebug>

#include <archive.h>
#include <archive_entry.h>

#include "decompressionoptions.h"

class Decompressor : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString compressedFile READ compressedFile WRITE setCompressedFile NOTIFY compressedFileChanged )
    Q_PROPERTY( QString errorString READ errorString WRITE setErrorString NOTIFY errorStringChanged )

    Q_PROPERTY( DecompressionOptions *decompressionOptions READ decompressionOptions WRITE setDecompressionOptions NOTIFY decompressionOptionsChanged )

public:

    explicit Decompressor( QObject *parent = nullptr );
    ~Decompressor();

    QString callBackCodeToString( ) const;
    Q_INVOKABLE bool extract();
    Q_INVOKABLE bool extractTo( const QString &destDir );

    QString destinationDirectory() const;
    void setDestinationDirectory( const QString &destinationDirectory );

    QString compressedFile() const;
    void setCompressedFile( const QString &compressedFile );

    QString errorString() const;
    void setErrorString( const QString &errorString );

    DecompressionOptions *decompressionOptions() const;
    void setDecompressionOptions( DecompressionOptions *decompressionOptions );

signals:
    void compressedFileChanged();
    void errorStringChanged( QString errorString );
    void decompressionOptionsChanged( DecompressionOptions *decompressionOptions );

private:
    bool decompress( const QString &destDir = QString() );
    int copyData( );
    void cleanMemory( const bool &cleanOptionsAlso = false);

    QString m_compressedFile;

    struct archive *m_archiveIn;
    struct archive *m_archiveOut;
    struct archive_entry *m_arcihveEntry;
    const void *m_buff;
    size_t m_size;
    int64_t m_offset;
    la_int64_t m_copyOffset;

    int m_callbackCode;

    QString m_errorString;
    DecompressionOptions *m_decompressionOptions;
};

#endif // DECOMPRESSOR_H
