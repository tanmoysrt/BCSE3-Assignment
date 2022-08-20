#! /bin/bash

# Extend the shell script written in (6) to perform the following task: User is asked to enter two
# different patterns or words. The first pattern will have to be matched with the contents of the
# file and replaced by the second pattern if a match occurs. If the first pattern does not occur in
# the file, an appropriate error message will be displayed.


read -p "Enter the file name: " FILE_NAME

if [ -f $FILE_NAME ]; then
    echo "File found !"
else
    echo "File not found"
    exit
fi

read -p "Enter the word to search: " SEARCH_WORD
read -p "Enter the word to replace: " REPLACE_WORD

lines=1
total_occurrences=0

updatedContentFile=""
while read line; do
    occurrences=0
    for word in $line; do
        if [ $word == $SEARCH_WORD ]; then
            occurrences=$(($occurrences+1))
            updatedContentFile="$updatedContentFile$REPLACE_WORD "
        else
            updatedContentFile="$updatedContentFile$word "
        fi
    done
    updatedContentFile+="\n"
    total_occurrences=$(($total_occurrences+$occurrences))
    lines=$(($lines+1))
    if [ $occurrences -gt 0 ]; then
        echo "Line $lines: $occurrences"
    fi
done < $FILE_NAME

if [ $total_occurrences -gt 0 ]; then
    echo "Total occurrences of ->$SEARCH_WORD<- : $total_occurrences"
    echo -e "$updatedContentFile" > $FILE_NAME
    echo "File updated successfully"
else
    echo "->$SEARCH_WORD<- not found"
fi