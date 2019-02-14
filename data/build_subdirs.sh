#!/usr/bin/env bash
set -e

if [ "$#" -ne 1 ]; then
    echo "Usage: ./build_subdirs.sh <new_root_dir>"
    exit 0
fi

mkdir $1

cd $1

tail -n +2 ../net_dirs.txt | while read i; 
do
    mkdir ${i#\.\/}
done
cd ../
