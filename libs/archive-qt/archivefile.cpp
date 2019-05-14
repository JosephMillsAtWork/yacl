#include "archivefile.h"


ArchiveFile::ArchiveFile(QObject *parent) :
    QObject( parent )
{}

int ArchiveFile::atime() const {
    return m_atime ;
}

void ArchiveFile::setAtime(const int &atime)
{
    if ( m_atime != atime )
    {
        m_atime = atime;
        emit atimeChanged(m_atime);
    }
}

int ArchiveFile::birthtime() const {
    return m_birthtime ;
}

void ArchiveFile::setBirthtime(const int &birthtime)
{
    if (m_birthtime != birthtime)
    {
        m_birthtime = birthtime;
        emit birthtimeChanged(m_birthtime);
    }
}

int ArchiveFile::ctime() const {
    return m_ctime ;
}

void ArchiveFile::setCtime(const int &ctime){
    if (m_ctime != ctime)
    {
        m_ctime = ctime;
        emit ctimeChanged(m_ctime);
    }
}

int ArchiveFile::mtime() const {
    return m_mtime ;
}

void ArchiveFile::setMtime(const int &mtime)
{
    if( m_mtime != mtime )
    {
        m_mtime = mtime;
        emit mtimeChanged(m_mtime);
    }
}

QString ArchiveFile::filetype() const {
    return m_filetype ;
}

void ArchiveFile::setFiletype(const QString &filetype)
{
    if (m_filetype == filetype)
    {
        m_filetype = filetype;
        emit filetypeChanged(m_filetype);
    }
}

QString ArchiveFile::fflags() const {
    return m_fflags ;
}

void ArchiveFile::setFflags(const QString &fflags)
{
    if( m_fflags != fflags )
    {
        m_fflags = fflags;
        emit fflagsChanged(m_fflags);
    }
}

int ArchiveFile::groupid() const {
    return m_groupid ;
}

void ArchiveFile::setGroupid(const int &groupid)
{
    if( m_groupid != groupid )
    {
        m_groupid = groupid;
        emit groupidChanged( m_groupid );
    }
}

QString ArchiveFile::groupname() const {
    return m_groupname ;
}

void ArchiveFile::setGroupname(const QString &groupname) {
    if( m_groupname != groupname )
    {
        m_groupname = groupname;
        emit groupnameChanged(m_groupname);
    }
}

QString ArchiveFile::pathname() const {
    return m_pathname ;
}

void ArchiveFile::setPathname(const QString &pathname){
    if( m_pathname != pathname )
    {
        m_pathname = pathname;
        emit pathnameChanged( m_pathname );
    }
}

QString ArchiveFile::permissions() const {
    return m_permissions;
}

void ArchiveFile::setPermissions(const QString &permissions) {
    if ( m_permissions != permissions )
    {
        m_permissions = permissions;
        emit permissionsChanged(m_permissions);
    }
}

QString ArchiveFile::username() const {
    return m_username ;
}

void ArchiveFile::setUsername(const QString &username) {
    if( m_username != username )
    {
        m_username = username;
        emit usernameChanged(m_username);
    }
}

int ArchiveFile::userid() const {
    return m_userid ;
}

void ArchiveFile::setUserid(const int &userid) {
    if( m_userid != userid )
    {
        m_userid = userid;
        emit useridChanged(m_userid);
    }
}

bool ArchiveFile::encrypted() const {
    return m_encrypted ;
}

void ArchiveFile::setEncrypted(const bool &encrypted) {
    if (m_encrypted != encrypted)
    {
        m_encrypted = encrypted;
        emit encryptedChanged(m_encrypted);
    }
}
