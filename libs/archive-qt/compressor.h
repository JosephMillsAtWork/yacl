#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include <QObject>
#include <QIODevice>
#include <QDir>
#include <QDirIterator>
#include <QStringList>
#include <QAtomicInt>
#include <QFile>
#include <QDebug>
#include <QCryptographicHash>
#include <QStandardPaths>
#include <qplatformdefs.h>
#include <QSaveFile>

#include "archive.h"
#include "archive_entry.h"
#include "archiveError.h"

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
    Q_INVOKABLE void cancel();

    bool hasFailed() const;
    bool wasCanceled() const;

    Error errorCode() const;
    QString errorString() const;

    enum CompressionType{
        SevenZip = 0,
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

    CompressionType format() const;
    void setFormat( const CompressionType &format );
    QString sourceDirectory() const;
    void setSourceDirectory( const QString &sourceDirectory);

    QString archiveName() const;
    void setArchiveName( const QString &archiveName );

    QString outputDirectory() const;

    void setOutputDirectory( const QString &outputDirectory );

    QString passwd() const;
    void setPasswd( const QString &passwd );

    bool hasPassword() const;
    void setHasPassword( const bool &hasPassword );

signals:
    void progress( qreal progress );
    void sourceDirectoryChanged();
    void archiveNameChanged();
    void formatChanged();
    void outputDirectoryChanged();
    void passwdChanged();
    void hasPasswordChanged();

private:
    void fillMetadata();
    bool create();
    void setError( Error errorCode, const QString &errorString );

    QString getSuffix( const CompressionType &type );

    bool sanitize();
    void clearMemory();
    bool m_failed;

    QAtomicInt m_canceled;

    Error m_errorCode;
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
