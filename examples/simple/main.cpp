#include <QCoreApplication>
#include <QTemporaryFile>
#include <QStandardPaths>
#include <QDebug>

#include <compressor.h>

#include <decompressor.h>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);


    // WRITE EXAMPLE
    Compressor *com = new Compressor;
    com->setArchiveName( "test.zip" );
    com->setFormat( Compressor::ZIP  );
    com->setSourceDirectory( TEST_COMPRESS );
    com->setOutputDirectory( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) );

    if( com->compress() )
    {
        qDebug() << "ARCHIVE CREATED" << com->outputDirectory() << com->archiveName();

        Decompressor *dcom = new Decompressor;
        dcom->setCompressedFile( QString::fromLatin1( "%1/%2" )
                                 .arg( com->outputDirectory() )
                                 .arg(com->archiveName() ) );

        QObject::connect( dcom, &Decompressor::errorStringChanged ,
                           [=]( QString errorString ){
            qDebug() << "ERROR STRING: " << errorString ;
        });

        if( dcom->extractTo( QStandardPaths::writableLocation( QStandardPaths::TempLocation ) ) )
        {

            qDebug() << "AWESOME ! extracted" << dcom->compressedFile() <<  " compressed file" ;
        }
        else
        {
            qDebug() << "NOT GOOD !" << dcom->errorString();
        }

        dcom = nullptr;
        delete dcom;
    }
    else
    {
        qDebug() << "NOT GOOD !" << com->errorString();
    }

    com->deleteLater();
    com = Q_NULLPTR;

    return a.exec();
}
