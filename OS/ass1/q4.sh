#! /bin/bash

# If found -c delete the folder

# Check if folder exists
if [ -d "my-deleted-files" ]; then
    echo "Directory exists"
else
# If not found , create the folder
    echo "Directory does not exist"
    echo "Creating directory"
    mkdir my-deleted-files
fi

