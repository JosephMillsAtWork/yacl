## Libs

#### GOAL
----
Have a qt qml plugin that is cross platform for decompression and compression. 

Libarchive allows the following
* Reads a variety of formats, including tar, pax, cpio, zip, xar, lha, ar, cab, mtree, rar, and ISO images.
* Writes tar, pax, cpio, zip, xar, ar, ISO, mtree, and shar archives.
* Automatically handles archives compressed with gzip, bzip2, lzip, xz, lzma, or compress.

So it would seem that writing against the api in libarchive would be swell because of how much it handles and different lib's for compression and also decompression.

So ... We are going to need libarchive. (cross-platform) well libarchive depends on
* bzip2 
* unzip
* lzo
* xz 
* libz 
* 7zip/p7zip
* zstd

#### Issues
So we are going to have to have a system where the we build the above libs first. 
Understanding that we come to some choices.
* **Compiler:** All the above libs are known to compile with gcc mingw/msys2 
* **Static vs Dynamic:** Maybe the build system should have a varible to set all this in a .qmake.conf or a pri file 
* **Storage:** Are we going to store all this code in our git. most of them we can get from come sorta url, Maybe it is worth looking into what it would take to make as git submods or even just use cUrl or wget. 
* **Build systems:** Most of the above libs are using autotools. Making qmake run autotools is possible and could be cross platform. Setup would be less coding. 
* **Bloat:**  Alot of these libs come with command line tools and docs and all in all good things. But no the less these are not things we need
* **Deplucation of work**: Looking outside of this project to fill some of its needs I have found some great projects that I feel we can use to make the development of this easier,
    * [Qt installer framework uses 7zip/pzip](https://github.com/qtproject/installer-framework/tree/master/src/libs/7zip) for there compression and decompression. Mebe we can borrow from there build system
    * [Qt automotive's Application manager uses libarchive in certian places.](http://code.qt.io/cgit/qt/qtapplicationmanager.git/tree/3rdparty/libarchive) Maybe this can be usefull to look at that source code and see if we can use it. 
    * [Qt core uses libz](http://code.qt.io/cgit/qt/qtbase.git/tree/src/3rdparty/zlib) maybe that can be usefull.
