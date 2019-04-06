#!/bin/bash
set -e
set -x
script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
package_name=libzip
url=https://libzip.org/download/libzip-1.5.1.tar.gz
archive_name=libzip-1.5.1.tar.gz
extracted_dir_name=libzip-1.5.1

cd $script_dir || return
echo "Removing old $package_name sources..."
rm -rf package_name $archive_name $extracted_dir_name
echo "downloading $package_name"
curl -OL $url
echo "Extracting new $package_name sources..."
tar -zxf $archive_name
cp -r $extracted_dir_name $package_name
rm -rf $archive_name $extracted_dir_name
cd - || return
