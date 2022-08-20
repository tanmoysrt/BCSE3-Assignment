#! /bin/bash


# Functions
function logData(){
    echo "$USER % $1 % $(date)" >> "logfile.txt"
}

function greetingMessage(){
    username="$USER"
    message="Hello $username, good "
    hours=$(date +%H)
    if [ $hours -lt 12 ]; then
        message+="morning"
    else
        message+="evening"
    fi
    echo $message
}
function showLogFile(){
    FILE_NAME="logfile.txt"
    while read line; do
        echo $line
    done < $FILE_NAME;
}

function listLargeFileHelper(){
    for f in "$1"/*; do
        if [ -d "$f" ]; then
            listLargeFileHelper "$f" $2
        else
            if [ -f "$f" ]; then
                if [ $(stat -c%s "$f") -gt $2 ]; then
                    echo "$f"
                fi
            fi
        fi
    done
}

function listLargeFile(){
    read -p "enter file size in bytes : " fileSize
    read -p "enter folder path : " folderPath

    echo "Files with size greater than $fileSize bytes are : "
    listLargeFileHelper $folderPath $fileSize
}

# Main

echo "Choose one of the following options:"
echo "1. Display greeting"
echo "2. List large files"
echo "3. Disk usage"
echo "4. View log file"
echo "5. Exit"

read -p "Enter your choice: " choice

case $choice in
    1)
        greetingMessage
        logData "display greeting"
        ;;
    2)
        listLargeFile
        logData "list large files"
        ;;
    3)
        df -h
        logData "disk usage"
        ;;
    4)
        showLogFile
        logData "view log file"
        ;;
    5)
        exit
        echo "Exitting..."
        ;;
    *)
        echo "Invalid choice"
        ;;
esac