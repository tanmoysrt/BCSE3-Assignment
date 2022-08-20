#! /bin/bash

DELETED_FOLDER_NAME="my-deleted-files"

#functions

# Get file name/folder name from filepath
function getFileName(){
    IFS='/'
    read -a SPLITTED_AT_DELIMETER <<<"$1" 
    unset IFS
    FILENAME=${SPLITTED_AT_DELIMETER[-1]}
    echo $FILENAME
}

# Get preferred version no to append to file name by looking in my-deleted-files folder
function getPreferredVersionNo(){
    IFS="."
    read -a SPLITTED_AT_DELIMETER <<<"$1"
    unset IFS
    EXTENSION=""
    FILENAME=${SPLITTED_AT_DELIMETER[0]}
    if [ ${#SPLITTED_AT_DELIMETER[@]} -gt 1 ]; then
        EXTENSION=".${SPLITTED_AT_DELIMETER[1]}"
    fi
    VERSION_NO=0
    while :
    do
        if [ -z $EXTENSION ]; then
            # It maybe a folder
            if [ -d "${DELETED_FOLDER_NAME}/${FILENAME}_${VERSION_NO}" ]; then
                VERSION_NO=$(($VERSION_NO+1))
            else
                break
            fi
        else
            # It maybe a file
            if [ -f "${DELETED_FOLDER_NAME}/${FILENAME}_${VERSION_NO}$EXTENSION" ]; then
                VERSION_NO=$(($VERSION_NO+1))
            else
                break
            fi
        fi
    done
    echo "${FILENAME}_${VERSION_NO}${EXTENSION}"
}


# MAIN PROGRAMM

# If found -c delete the folder
if (( $# == 2 )); then
    if (($2 == '-c' )); then
        echo "Deleting the folder"
        rm -r $DELETED_FOLDER_NAME
    fi
fi

# Check if folder exists
if [ -d $DELETED_FOLDER_NAME ]; then
    echo "Directory exists"
else
# If not found , create the folder
    echo "Directory does not exist, created a new one"
    mkdir $DELETED_FOLDER_NAME
fi

FILENAME=$(getFileName $1)
FILE_PATH_WITH_VERSION=$(getPreferredVersionNo $FILENAME)

# move the files/folder to the folder
mv $1 "$DELETED_FOLDER_NAME/$FILE_PATH_WITH_VERSION"