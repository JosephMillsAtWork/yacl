## Topic of compression and decompression in Qt/Qml. 
#### Goal
----
Have a qt qml plugin that is cross platform for decompression and compression. 
Keeping something 100% cross-platform and also supporting the wide numbers of compression and decompression.  Is not the easiest thing to do. So here is what I have come up with.
#### Step 0 (working on)
----
libarchive uses the following lib's and they would need to be linked. I think that setting up the build system to statically link these into libarchive would be the way to go. 

* bzip2 
* unzip
* lzo
* xz 
* libz 
* 7zip/p7zip
* zstd

For a deeper understaning of objective 0 please see [this](https://github.com/JosephMillsAtWork/yacl/libs/README.md)

#### Step 1 (todo) 
----
Use libarchive api and expose this as a qt and qml plugin library.

#### Step 2 (todo)
----
Make a example program and link that into the qt qml lib talked about in step two.

#### Step 3 (todo)
----
Documents and maybe CI/CD ?

#### Step 4 (todo)
----
Upgrades ?

    

