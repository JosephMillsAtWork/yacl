#ifndef DECOMPRESSIONOPTIONS_H
#define DECOMPRESSIONOPTIONS_H

#include <QObject>
#include <QMap>
#include <QStringList>
#include <QDebug>
#include <QList>

class DecompressionOptions : public QObject
{
    Q_OBJECT
    Q_PROPERTY( int flags READ flags NOTIFY flagsChanged )
    Q_PROPERTY( QStringList optionsList READ optionsList NOTIFY optionsListChanged )
    Q_PROPERTY( QStringList currentOptions READ currentOptions NOTIFY currentOptionsChanged )

public:
    explicit DecompressionOptions( QObject *parent = nullptr );

    enum Options{
        OWNER = 0x0001,
        PERM = 0x0002,
        TIME = 0x0004,
        NO_OVERWRITE = 0x0008,
        UNLINK = 0x0010,
        ACL = 0x0020,
        FFLAGS = 0x0040,
        XATTR = 0x0080,
        SECURE_SYMLINKS = 0x0100,
        SECURE_NODOTDOT = 0x0200,
        NO_AUTODIR = 0x0400,
        NO_OVERWRITE_NEWER = 0x0800,
        SPARSE = 0x1000,
        MAC_METADATA = 0x2000,
        NO_HFS_COMPRESSION = 0x4000,
        HFS_COMPRESSION_FORCED = 0x8000,
        SECURE_NOABSOLUTEPATHS = 0x10000,
        CLEAR_NOCHANGE_FFLAGS = 0x20000,
    };
    Q_INVOKABLE void addDecompressionFlag( const Options &opt )
    {
        if( !m_options.contains( opt ) )
        {
            m_options.append( opt );
            setCurrentOptions();
        }
    }
    Q_INVOKABLE void removeDecompressionOption( const Options &opt )
    {
        if( m_options.contains( opt ) )
        {
            for ( int i =0 ;i < m_options.count(); i++ )
            {
                if( m_options.at( i ) == opt )
                {
                    m_options.removeAt( i );
                }
            }
            setCurrentOptions();
        }
    }
    void setCurrentOptions()
    {
        m_currentOptions.clear();
        for (int i =0;i < m_options.count();i++)
        {
            m_currentOptions.append( m_allOptions.value( m_options[i] ) );
        }
        emit currentOptionsChanged();
    }

    void setOptionsList() {
        m_optionsList.clear();
        QMapIterator<Options, QString > iter = m_allOptions;
        while (iter.hasNext() )
        {
            m_optionsList.append( iter.value() );
        }
        emit optionsListChanged();
    }

    QStringList optionsList() const
    {
        return m_optionsList;
    }
    QStringList currentOptions()
    {
        setCurrentOptions();
        return m_currentOptions;
    }

    int flags() const
    {
        return m_flags;
    }

signals:
    void optionsListChanged();
    void currentOptionsChanged();
    void flagsChanged( int flags);

private:
    int m_flags;

    void setFlags() {
        m_flags = int();
        for( int i = 0; i < m_options.count(); i++ )
        {
            if( i == 0 )
            {
                m_flags = static_cast<int>( m_options[i] );
            }
            else
            {
                m_flags |= static_cast<int>( m_options[i] );
            }
        }
    }

    QList<Options> m_options;
    QMap<Options, QString> m_allOptions;
    QStringList m_optionsList;
    QStringList m_currentOptions;
};

#endif // DECOMPRESSIONOPTIONS_H
