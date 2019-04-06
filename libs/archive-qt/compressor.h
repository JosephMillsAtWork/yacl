#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <QObject>
#include <QIODevice>
#include <QDir>
#include <QStringList>
#include <QAtomicInt>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include <qplatformdefs.h>
#include <QSaveFile>

#include <error.h>
class Compressor : public QObject
{
    Q_OBJECT
    Q_PROPERTY( QString sourceDirectory READ sourceDirectory WRITE setSourceDirectory NOTIFY sourceDirectoryChanged )
    Q_PROPERTY( QString archiveName READ archiveName WRITE setArchiveName NOTIFY archiveNameChanged )
    Q_PROPERTY( QString outputDirectory READ outputDirectory WRITE setOutputDirectory NOTIFY outputDirectoryChanged)
    Q_PROPERTY( CompressionType format READ format WRITE setFormat NOTIFY formatChanged)
    Q_ENUMS( CompressionType )

public:
    explicit Compressor( QObject *parent = nullptr );
    ~Compressor();
    Q_INVOKABLE bool compress();
    Q_INVOKABLE void cancel();


    bool hasFailed() const;
    bool wasCanceled() const;

    Error errorCode() const;
    QString errorString() const;

    enum CompressionType{
            GZIP = 0,
            XZ = 1,
            ZSTD = 2,
            BZIP = 3,
            ZIP = 4,
            LZMA = 5
    };
    CompressionType format() const
    {
        return m_format;
    }
    void setFormat( const CompressionType &format )
    {
        if (m_format != format)
        {
            m_format = format;
            emit formatChanged();
        }

    }
    QString sourceDirectory() const
    {
        return m_sourceDirectory;
    }
    void setSourceDirectory( const QString &sourceDirectory)
    {
        if (m_sourceDirectory != sourceDirectory)
        {
            m_sourceDirectory = sourceDirectory;
            emit sourceDirectoryChanged();
        }
    }

    QString archiveName() const
    {
        return m_archiveName;
    }
    void setArchiveName( const QString &archiveName )
    {
        if (m_archiveName != archiveName)
        {
            m_archiveName = archiveName;
            emit archiveNameChanged();
        }
    }

    QString outputDirectory() const
    {
        return m_outputDirectory;
    }

    void setOutputDirectory( const QString &outputDirectory )
    {
        if (m_outputDirectory != outputDirectory)
        {
            m_outputDirectory = outputDirectory;
            emit outputDirectoryChanged();
        }
    }

signals:
    void progress( qreal progress );
    void sourceDirectoryChanged();
    void archiveNameChanged();
    void formatChanged();
    void outputDirectoryChanged();

private:
    bool create();
    void setError( Error errorCode, const QString &errorString );

    QString getSuffix( const QFileInfo &file );
    bool sanitize();

    bool m_failed = false;


    QAtomicInt m_canceled;

    Error m_errorCode = Error::None;
    QString m_errorString;


    struct archive_entry *m_entry;
    struct archive *m_archive;
    QSaveFile m_TemporaryFile;

    QString m_sourceDirectory;
    QString m_archiveName;
    CompressionType m_format;
    QString m_outputDirectory;

    QString m_archiveAbsoluteFilePath;

    QStringList m_vaildSuffix;

};
#endif // COMPRESSOR_H