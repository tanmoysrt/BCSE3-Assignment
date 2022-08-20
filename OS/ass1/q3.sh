#! /bin/bash

BASE_PATH=$1

DIRECTORIES_COUNT=0
FILES_COUNT=0

if [ -d $BASE_PATH ]; then
    echo "Path exists"
else
    exit
fi

# echo $(ls $BASE_PATH)
function countFiles(){
    local filesCount
    filesCount=0

    for f in "$1"/*; do
        if [ -d "$f" ]; then
            v=$(countFiles "$f")
            filesCount=$(($filesCount+$v))
        else
            filesCount=$(($filesCount+1))
        fi
    done

    echo $filesCount
}

x=$(countFiles $BASE_PATH)
echo "Total no of files : $x"