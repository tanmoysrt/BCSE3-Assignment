#! /bin/bash

FILENAME=$1

# Check if file exists
if [ -f $FILENAME ]; then
    echo "File exists"
else
    echo "File does not exist"
    echo "Creating file"
    touch $FILENAME
fi

lines=0
contentFile=$(<$FILENAME)
# Count lines
while read line; do
    lines=$(($lines+1))
done < $FILENAME

echo "Number of lines in file: $lines"