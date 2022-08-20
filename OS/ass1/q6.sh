#! /bin/bash

# Write a shell script that accepts a file name as an input and performs the following activities
# on the given file. The program asks for a string of characters (that is, any word) to be provided
# by the user. The file will be searched to find whether it contains the given word. If the file
# contains the given word, the program will display (a) the number of occurrences of the word.
# The program is also required to display (b) the line number in which the word has occurred
# and no. of times the word has occurred in that line (Note: the word may occur more than once
# in a given line). If the file does not contain the word, an appropriate error message will be
# displayed.

read -p "Enter the file name: " FILE_NAME

if [ -f $FILE_NAME ]; then
    echo "File found !"
else
    echo "File not found"
    exit
fi

read -p "Enter the word to search: " SEARCH_WORD

lines=1
total_occurrences=0

while read line; do
    occurrences=0
    for word in $line; do
        if [ $word == $SEARCH_WORD ]; then
            occurrences=$(($occurrences+1))
        fi
    done
    total_occurrences=$(($total_occurrences+$occurrences))
    lines=$(($lines+1))
    if [ $occurrences -gt 0 ]; then
        echo "Line $lines: $occurrences"
    fi
done < $FILE_NAME

if [ $total_occurrences -gt 0 ]; then
    echo "Total occurrences of ->$SEARCH_WORD<- : $total_occurrences"
else
    echo "->$SEARCH_WORD<- not found"
fi