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

    Q_PROPERTY(QString passwd READ passwd WRITE setPasswd NOTIFY passwdChanged)
    Q_PROPERTY(bool hasPassword READ hasPassword WRITE setHasPassword NOTIFY hasPasswordChanged)

public:
    explicit Compressor( QObject *parent = nullptr );
    ~Compressor();
    Q_INVOKABLE bool compress();
//    Q_INVOKABLE bool compressDirectory( const QString &dir );
    Q_INVOKABLE void cancel();

    bool hasFailed() const;
    bool wasCanceled() const;

    Error errorCode() const;
    QString errorString() const;

    enum CompressionType{
        SevenZip = 0, // p7zip
        AR = 1,
        ARBSD = 2,
        ARGNU = 3,
        ARSVR4 = 4,
        BSDTAR = 5,
        CD9660 = 6,
        CPIO = 7,
        GNUTAR = 8,
        ISO = 9,
        ISO9660 = 10,
        MTREE = 11,
        MTREE_CLASSIC = 12,
        NEWC = 13,
        ODC = 14,
        OLDTAR = 15,
        PAX = 16,
        PAXR = 17,
        POSIX = 18,
        RAW = 19,
        RPAX = 20,
        SHAR = 21,
        SHARDUMP = 22,
        USTAR = 23,
        V7TAR = 24,
        V7 = 25,
        WARC = 26,
        XAR = 27,
        ZIP = 28,
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

    QString passwd() const
    {
        return m_passwd;
    }
    void setPasswd( const QString &passwd )
    {
        if (m_passwd != passwd)
        {
            m_passwd = passwd;
            emit passwdChanged();
        }
    }

    bool hasPassword() const
    {
        return m_hasPassword;
    }
    void setHasPassword( const bool &hasPassword )
    {
        if (m_hasPassword != hasPassword)
        {
            m_hasPassword = hasPassword;
            emit hasPasswordChanged();
        }
    }

signals:
    void progress( qreal progress );
    void sourceDirectoryChanged();
    void archiveNameChanged();
    void formatChanged();
    void outputDirectoryChanged();
    void passwdChanged();
    void hasPasswordChanged();

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
    struct archive *m_disk;

    QSaveFile m_TemporaryFile;

    QString m_sourceDirectory;
    QString m_archiveName;
    CompressionType m_format;
    QString m_outputDirectory;

    QString m_archiveAbsoluteFilePath;

    QStringList m_vaildSuffix;

    QHash<CompressionType, QString> m_comperssionHash;
    QString m_passwd;
    bool m_hasPassword;
};
#endif // COMPRESSOR_H
