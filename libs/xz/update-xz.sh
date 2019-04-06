#!/bin/bash
set -e
set -x
echo "Removing old xz sources..."
rm -rf xz xz-5.2.4.tar.gz xz-5.2.4
curl -OL https://sourceforge.net/projects/lzmautils/files/xz-5.2.4.tar.gz
script_dir=$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )
xz_archive=xz-5.2.4.tar.gz
xz_extracted_dir_name=xz-5.2.4
xz_parent_dir=$script_dir/

cd $xz_parent_dir
echo "Extracting new xz sources..."
tar -zxf $xz_archive
cp -r $xz_extracted_dir_name lzma
rm -rf $xz_archive $xz_extracted_dir_name
