#include "decompressor.h"

Decompressor::Decompressor(QObject *parent) :
    QObject(parent)
  , m_archiveIn( nullptr )
  , m_archiveOut( nullptr )
  , m_arcihveEntry( nullptr )
  , m_callbackCode( -1 )
  , m_errorString( "NONE" )
  , m_decompressionOptions{ new DecompressionOptions{ this  } }
{

}

Decompressor::~Decompressor(){
    cleanMemory( true );
}

QString Decompressor::callBackCodeToString() const {
    QString ret = "Unknown";

    if( m_callbackCode == ARCHIVE_EOF)
    {
        ret = "Found end of archive";
    }
    else if( m_callbackCode == ARCHIVE_OK )
    {
        ret = "Operation was successful";
    }
    else if( m_callbackCode == ARCHIVE_RETRY )
    {
        ret = "Retry might succeed." ;
    }
    else if( m_callbackCode == ARCHIVE_WARN )
    {
        ret = "Partial success.";
    }
    else if( m_callbackCode == ARCHIVE_FAILED )
    {
        ret = "Current operation cannot complete.";
    }
    else if( m_callbackCode == ARCHIVE_FATAL)
    {
        ret = "No more operations are possible.";
    }
    return  ret;
}

bool Decompressor::extract(){
    return decompress();
}

bool Decompressor::extractTo( const QString &destDir )
{
    return decompress( destDir );
}

QString Decompressor::compressedFile() const
{
    return m_compressedFile;
}

void Decompressor::setCompressedFile( const QString &compressedFile )
{
    if ( m_compressedFile != compressedFile )
    {
        m_compressedFile = compressedFile;
        emit compressedFileChanged();
    }
}

QString Decompressor::errorString() const
{
    return m_errorString;
}

void Decompressor::setErrorString( const QString &errorString )
{
    if ( m_errorString != errorString )
    {
        m_errorString = errorString;
        emit errorStringChanged( m_errorString );
    }
}

DecompressionOptions *Decompressor::decompressionOptions() const
{
    return m_decompressionOptions;
}

void Decompressor::setDecompressionOptions( DecompressionOptions *decompressionOptions )
{
    m_decompressionOptions = nullptr;
    m_decompressionOptions = decompressionOptions;
    emit decompressionOptionsChanged( m_decompressionOptions );
}

bool Decompressor::decompress( const QString &destDir )
{
    m_archiveIn = archive_read_new();
    archive_read_support_format_all( m_archiveIn );

    archive_read_support_filter_all( m_archiveIn );

    m_archiveOut = archive_write_disk_new( );
    archive_write_disk_set_options( m_archiveOut, m_decompressionOptions->flags()  );
    archive_write_disk_set_standard_lookup( m_archiveOut );
    if( ( m_callbackCode = archive_read_open_filename( m_archiveIn, m_compressedFile.toLatin1().data(), 10240 ) ) )
    {
        setErrorString( "failed to init opening of archive" );
        cleanMemory();
        return false;
    }
    for (;;)
    {
        m_callbackCode = archive_read_next_header(m_archiveIn, &m_arcihveEntry);
        if ( m_callbackCode == ARCHIVE_EOF)
        {
            // We did it !
            break;
        }
        if ( m_callbackCode < ARCHIVE_OK)
        {
            setErrorString( QString::fromLatin1( archive_error_string( m_archiveIn ) ) );
            // keep going
        }
        if ( m_callbackCode < ARCHIVE_WARN )
        {
            setErrorString( "Failed to read the next header in the archive" );
            cleanMemory();
            return false;
        }

        if( !destDir.isEmpty() )
        {
            // Fixme make sure that there is infact this dir.
            const char *currentFile = archive_entry_pathname( m_arcihveEntry );
            const char *dDir =  QString( "%1/%2" ).arg( destDir ).arg(currentFile).toLatin1().data();
            archive_entry_set_pathname( m_arcihveEntry, dDir );
        }

        m_callbackCode = archive_write_header( m_archiveOut, m_arcihveEntry );
        if ( m_callbackCode < ARCHIVE_OK )
        {
            setErrorString( QString::fromLatin1( archive_error_string( m_archiveOut ) ) );
            // keep going
        }
        else if ( archive_entry_size( m_arcihveEntry )  > 0 )
        {
            m_callbackCode = copyData( );
            if ( m_callbackCode < ARCHIVE_OK )
            {
                setErrorString( QString::fromLatin1( archive_error_string( m_archiveOut ) ) );
                // keep going
            }
            if ( m_callbackCode < ARCHIVE_WARN )
            {
                setErrorString( "Failed to copy the data to disk" );
                cleanMemory();
                return false;
            }
        }
        m_callbackCode = archive_write_finish_entry( m_archiveOut );
        if ( m_callbackCode < ARCHIVE_OK )
        {
            setErrorString( QString::fromLatin1( archive_error_string( m_archiveOut ) ));
            cleanMemory( );
            return true;
        }
        if ( m_callbackCode < ARCHIVE_WARN )
        {
            setErrorString( "failed to finish writing the to disk");
            cleanMemory();
            return false;
        }
    }

    cleanMemory();
    return true;
}

int Decompressor::copyData( )
{
    const void *buff;
    size_t size;
    int64_t offset;
    if( m_archiveIn != nullptr && m_archiveOut != nullptr )
    {
        for (;;)
        {
            m_callbackCode = archive_read_data_block( m_archiveIn, &buff, &size, &offset);
            if( m_callbackCode == ARCHIVE_EOF )
            {
                return ( ARCHIVE_OK );
            }
            if( m_callbackCode < ARCHIVE_OK )
            {
                return ( m_callbackCode );
            }
            m_callbackCode = static_cast<int>(  archive_write_data_block( m_archiveOut
                                                                          , buff
                                                                          , size
                                                                          , offset
                                                                          )
                                                );
            if( m_callbackCode < ARCHIVE_OK)
            {
                setErrorString( QString::fromLatin1( archive_error_string( m_archiveOut ) ) );
                return ( m_callbackCode );
            }
        }
    }
    else
    {
        setErrorString( "Error in pointers when coping data ");
        return ( ARCHIVE_FAILED );
    }
}

void Decompressor::cleanMemory( const bool &cleanOptionsAlso ){
    archive_read_close( m_archiveIn );
    archive_read_free( m_archiveIn );
    archive_write_close( m_archiveOut );
    archive_write_free( m_archiveOut );

    m_archiveIn = nullptr;
    m_archiveOut = nullptr;
    m_arcihveEntry = nullptr;
    if( cleanOptionsAlso )
    {
        m_decompressionOptions = nullptr;
    }
}

