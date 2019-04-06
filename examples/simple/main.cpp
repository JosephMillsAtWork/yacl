#include <QCoreApplication>
#include <QUrl>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QDebug>
#include "downloadmanager.h"
#include "extractor.h"
#include "compressor.h"
int main(int argc, char *argv[])
{

    // Pass
    //    QString xz = "http://ftp.gnu.org/gnu/automake/automake-1.16.1.tar.xz";
    //    QString bz2 = "http://ftp.gnu.org/gnu/make/make-4.2.1.tar.bz2";

    //    QString tar = "http://ftp.gnu.org/gnu/bc/bc-1.07.1.tar.gz";
    //    QString tarL = "/Users/josephmills/build-untitled2-Desktop_Qt_5_11_2_Apple_Clang_64bit-Debug/package-example/bc-1.07.1.tar.gz";

    //    QString sz = "https://www.7-zip.org/a/7z1805-src.7z";

    //    QString zip = "https://github.com/libarchive/libarchive/archive/v3.3.3.zip";
    //    QString zipL = "/Users/josephmills/build-untitled2-Desktop_Qt_5_11_2_Apple_Clang_64bit-Debug/package-example/ziptest.zip";


    //    QString url = tar;

    //    QFileInfo fi( url );
    //    QString arc = fi.fileName();

    QCoreApplication a(argc, argv);

    //    QDir destDir( QCoreApplication::applicationDirPath().append("/test") );
    //    if( !destDir.exists( ) )
    //    {
    //        destDir.mkpath( QCoreApplication::applicationDirPath().append("/test") );
    //    }



    //    DownloadManager dman;

    //    QObject::connect( &dman, &DownloadManager::finished,
    //                      [=](){
    //        QUrl localUrl( QCoreApplication::applicationDirPath().append( "/" ).append( arc ) );
    //        localUrl.setScheme("file");
    //        PackageExtractor pkg( localUrl , destDir );

    //        if( pkg.extract() )
    //        {
    //            qDebug() << "sweet ";
    //        }
    //        else
    //        {
    //            qDebug() << "bad news" << qPrintable( pkg.errorString() );
    //        }
    //    });
    //    if ( fi.exists() )
    //    {
    //        emit dman.finished();
    //    }
    //    else
    //    {
    //        dman.saveFileName( QCoreApplication::applicationDirPath().append( "/" ).append( arc ) );
    //        dman.append( QUrl(url) );
    //    }

    ///////////////////////////////////
    /// WRITE EXAMPLE
    ////
    Compressor *dcom = new Compressor;
    dcom->setArchiveName( "TestPackage.tar.gz" );
    dcom->setSourceDirectory( "/home/jmills/Desktop/git/opensource/yacl/legal" );
//    dcom->setOutputDirectory("/tmp");

    //  pkg.setCompressionType( PackageCreator::GZIP );
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
