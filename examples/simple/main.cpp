#include <QCoreApplication>
#include <QUrl>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QDebug>

#include "compressor.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);


    // WRITE EXAMPLE
    Compressor *dcom = new Compressor;
    dcom->setArchiveName( "test.iso" );
    dcom->setFormat( Compressor::ISO );
    dcom->setSourceDirectory( TEST_COMPRESS );
    dcom->setOutputDirectory( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) );

    if( dcom->compress() )
    {
        qDebug() << "Archive created" << dcom->outputDirectory() << dcom->archiveName();
    }
    else
    {
        qDebug() << "Archive was not created" << dcom->errorString();
    }

    dcom->deleteLater();
    dcom = Q_NULLPTR;

    return a.exec();
}
