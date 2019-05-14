#pragma once
#ifndef ARCHIVEQTPLUGIN_H
#define ARCHIVEQTPLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlEngine>
#include <QJSEngine>
#include <qqml.h>

#include "archivefile.h"
#include "archivefilesmodel.h"

#include "compressor.h"

#include "decompressor.h"
#include "decompressionoptions.h"


class ArchiveQtPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes( const char *uri )
    {
        // @uri ArchiveQt
        qmlRegisterUncreatableType<ArchiveFile>(uri, 1, 0, "ArchiveFile", "ArchiveFile classs is not a creatoable type.  You can get access via ArchiveFilesModel");
        qmlRegisterType<ArchiveFilesModel>( uri, 1, 0, "ArchiveFilesModel" );

        qmlRegisterSingletonType<Compressor>(
                    uri, 1, 0, "Compressor",
                    []( QQmlEngine*, QJSEngine* )-> QObject* { return new Compressor{}; }
        );

        qmlRegisterType<DecompressionOptions>( uri , 1 , 0 , "DecompressionOptions" );
        qmlRegisterSingletonType<Decompressor>(
                    uri, 1, 0, "Decompressor",
                    []( QQmlEngine*, QJSEngine* )-> QObject* { return new Decompressor{}; }
        );
    }
};
#endif // ARCHIVEQTPLUGIN_H
