#include "decompressionoptions.h"

DecompressionOptions::DecompressionOptions(QObject *parent) :
    QObject(parent)
    , m_flags( -1 )
{
    if( !m_allOptions.isEmpty() )
    {
        m_allOptions.clear();
    }
    m_allOptions.insert( OWNER, "Keep Ownership" );
    m_allOptions.insert( PERM, "Keep Permissions");
    m_allOptions.insert( TIME, "Keep Time" );
    m_allOptions.insert( NO_OVERWRITE, "Don't Overwrite" );
    m_allOptions.insert( UNLINK, "Unlink softlinks" );
    m_allOptions.insert( ACL, "Keep ACLs" );
    m_allOptions.insert( FFLAGS, "Keep fflags" );
    m_allOptions.insert( XATTR, "Keep xattrs"  );
    m_allOptions.insert( SECURE_SYMLINKS, "Keep redirected links" );
    m_allOptions.insert( SECURE_NODOTDOT, "Keep Dot folders" );
    m_allOptions.insert( NO_AUTODIR, "Don't create parent directories" );
    m_allOptions.insert( NO_OVERWRITE_NEWER, "Dont't Overwrite files" );
    m_allOptions.insert( SPARSE, "Write holes" );
    m_allOptions.insert( MAC_METADATA, "Restore Mac metadata" );
    m_allOptions.insert( NO_HFS_COMPRESSION, "No HFS+ compression" );
    m_allOptions.insert( HFS_COMPRESSION_FORCED, "Keep HFS+ compression" );
    m_allOptions.insert( SECURE_NOABSOLUTEPATHS, "Keep Absolute Paths" );
    m_allOptions.insert( CLEAR_NOCHANGE_FFLAGS, "Clear no changed fflags" );


    // setup default options
    if( !m_options.isEmpty() )
    {
        m_options.clear();
    }
    m_options.append( TIME );
    m_options.append( PERM );
    m_options.append( ACL );
    m_options.append( FFLAGS );
    setFlags( );
}
