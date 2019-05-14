#ifndef ARCHIVEFILE_H
#define ARCHIVEFILE_H
#include <QString>
#include <QObject>
class ArchiveFile : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int atime READ atime NOTIFY atimeChanged)
    Q_PROPERTY( int birthtime READ birthtime NOTIFY birthtimeChanged)
    Q_PROPERTY( int ctime READ ctime NOTIFY ctimeChanged)
    Q_PROPERTY( int mtime READ mtime NOTIFY mtimeChanged)
    Q_PROPERTY( QString filetype READ filetype NOTIFY filetypeChanged)
    Q_PROPERTY( QString fflags READ fflags NOTIFY fflagsChanged)
    Q_PROPERTY( int groupid READ groupid NOTIFY groupidChanged)
    Q_PROPERTY( QString groupname READ groupname NOTIFY groupnameChanged)
    Q_PROPERTY( QString pathname READ pathname NOTIFY pathnameChanged)
    Q_PROPERTY( QString permissions READ permissions NOTIFY permissionsChanged)
    Q_PROPERTY( QString username READ username NOTIFY usernameChanged)
    Q_PROPERTY( int userid READ userid NOTIFY useridChanged)
    Q_PROPERTY( bool encrypted READ encrypted NOTIFY encryptedChanged)
public:
    ArchiveFile( QObject *parent = Q_NULLPTR );

    int atime() const;
    void setAtime( const int &atime );

    int birthtime() const;
    void setBirthtime( const int &birthtime);

    int ctime() const;
    void setCtime( const int &ctime );

    int mtime() const;
    void setMtime( const int &mtime );

    QString filetype() const;
    void setFiletype( const QString &filetype );

    QString fflags() const;
    void setFflags( const QString &fflags );

    int groupid() const;
    void setGroupid( const int &groupid );

    QString groupname() const;
    void setGroupname( const QString &groupname );

    QString pathname() const;
    void setPathname( const QString &pathname );

    QString permissions() const;
    void setPermissions( const QString &permissions );

    QString username() const;
    void setUsername( const QString &username );

    int userid() const;
    void setUserid( const int &userid );

    bool encrypted() const;
    void setEncrypted( const bool &encrypted );

signals:
    void atimeChanged( int atime );
    void birthtimeChanged( int birthtime );
    void ctimeChanged( int ctime );
    void mtimeChanged( int mtime );
    void filetypeChanged( QString filetype );
    void fflagsChanged( QString fflags );
    void groupidChanged( int groupid );
    void groupnameChanged( QString groupname );
    void pathnameChanged( QString pathname );
    void permissionsChanged( QString permissions );
    void usernameChanged( QString username );
    void useridChanged( int userid );
    void encryptedChanged( bool encrypted );

private:
    int m_atime;
    int m_birthtime;
    int m_ctime;
    int m_mtime;
    QString m_filetype;
    QString m_fflags;
    int m_groupid;
    QString m_groupname;
    QString m_pathname;
    QString m_permissions;
    QString m_username;
    int m_userid;
    bool m_encrypted;
};
#endif // ARCHIVEFILE_H
