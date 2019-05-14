#ifndef ARCHIVEFILESMODEL_H
#define ARCHIVEFILESMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QList>
#include <archivefile.h>

class ArchiveFilesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY( int count READ count NOTIFY countChanged )

public:
    explicit ArchiveFilesModel( QObject *parent = Q_NULLPTR ) :
        QAbstractListModel( parent )
      , m_files{ new QList<ArchiveFile*>{ } }
    {
    }

    ~ArchiveFilesModel(){
        clear();
        m_files = Q_NULLPTR;
        delete m_files;
    }
    enum ArchiveFileRoles {
        ATime = 0,
        BirthTime = 1,
        CTime = 2,
        MTime = 3,
        FileType = 4,
        FFlags = 5,
        GroupID = 6,
        GroupName = 7,
        PathName = 8 ,
        Permissions = 9,
        UserName = 10,
        UserId = 11,
        Encrypted  = 12,
    };

    Q_INVOKABLE void clear(){
        if (!m_files->isEmpty() )
        {
            beginRemoveRows( QModelIndex(), 0, m_files->count () -1 );
            for( ArchiveFile *arc :  *m_files )
            {
                dereferenceItem( arc );
            }
            m_files->clear();
            updateCounter();
            endRemoveRows();
        }
    }
    Q_INVOKABLE void append( ArchiveFile *arc ) {
        if ( arc != Q_NULLPTR )
        {
            const int pos = m_files->count();
            beginInsertRows( QModelIndex(), pos, pos );
            m_files->append( arc );
            updateCounter();
            endInsertRows();
        }
    }
    Q_INVOKABLE void remove( int idx ) {
        if ( idx >= 0 && idx < m_files->size () )
        {
            beginRemoveRows( QModelIndex(), idx, idx );
            ArchiveFile *arc = m_files->takeAt( idx );
            dereferenceItem( arc );
            updateCounter();
            endRemoveRows();
        }
    }

    int size() const {
        return m_files->size();
    }
    int count() const {
        return m_files->count();
    }
    int length() const {
        return m_files->length();
    }
    bool isEmpty() const{
        return m_files->isEmpty ();
    }
    bool contains( ArchiveFile *arc ) const {
        return m_files->contains( arc );
    }
    int indexOf( ArchiveFile *arc ) const {
        return m_files->indexOf( arc );
    }

    Q_INVOKABLE void insertArchiveFile( ArchiveFile *arc ) {
        append( arc );
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const {
        Q_UNUSED(parent);
        return m_files->count();
    }

    QVariant data( const QModelIndex &index, int role = Qt::DisplayRole) const {
        if (index.row() < 0 || index.row() >= m_files->count() )
        {
            return QVariant();
        }
        const ArchiveFile *archiveFile = m_files->at( index.row() );
        if (      role == ATime         ){ return archiveFile->atime() ; }
        else if ( role == BirthTime     ){ return archiveFile->birthtime() ; }
        else if ( role == CTime         ){ return archiveFile->ctime() ; }
        else if ( role == MTime         ){ return archiveFile->mtime() ; }
        else if ( role == FileType      ){ return archiveFile->filetype() ; }
        else if ( role == FFlags        ){ return archiveFile->fflags() ; }
        else if ( role == GroupID       ){ return archiveFile->groupid() ; }
        else if ( role == GroupName     ){ return archiveFile->groupname() ; }
        else if ( role == PathName      ){ return archiveFile->pathname() ; }
        else if ( role == Permissions   ){ return archiveFile->permissions() ; }
        else if ( role == UserName      ){ return archiveFile->userid() ; }
        else if ( role == UserId        ){ return archiveFile->username() ; }
        else if ( role == Encrypted     ){ return archiveFile->encrypted() ; }
        else { return QVariant(); }
    }
signals:
    void countChanged();

protected:
    QHash<int, QByteArray> roleNames() const{
        QHash<int, QByteArray> roles;
        roles[ ATime       ] = "atime" ;
        roles[ BirthTime   ] = "birthtime" ;
        roles[ CTime       ] = "ctime" ;
        roles[ MTime       ] = "mtime" ;
        roles[ FileType    ] = "filetype" ;
        roles[ FFlags      ] = "fflags" ;
        roles[ GroupID     ] = "groupid" ;
        roles[ GroupName   ] = "groupname" ;
        roles[ PathName    ] = "pathname" ;
        roles[ Permissions ] = "permissions" ;
        roles[ UserName    ] = "username" ;
        roles[ UserId      ] = "userid" ;
        roles[ Encrypted   ] = "encrypted" ;
        return roles;
    }

    QList<ArchiveFile *>::const_iterator begin () const {
        return m_files->begin();
    }
    QList<ArchiveFile *>::const_iterator end (void) const {
        return m_files->end();
    }
    QList<ArchiveFile *>::const_iterator constBegin (void) const {
        return m_files->constBegin();
    }
    QList<ArchiveFile *>::const_iterator constEnd (void) const {
        return m_files->constEnd();
    }
    inline void updateCounter() {
        if ( m_count != m_files->count() )
        {
            m_count = m_files->count ();
            emit countChanged();
        }
    }

    void dereferenceItem(  ArchiveFile *arc ) {
        disconnect (this, Q_NULLPTR, arc, Q_NULLPTR);
        disconnect (arc, Q_NULLPTR, this, Q_NULLPTR);
        if ( arc != Q_NULLPTR )
        {
            arc = Q_NULLPTR;
            arc->deleteLater();
        }
    }

private:
    QList<ArchiveFile*> *m_files;
    int m_count;
};

#endif // ARCHIVEFILESMODEL_H
