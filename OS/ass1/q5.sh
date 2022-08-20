#! /bin/bash

# Write a script called birthday_match.sh that takes two birthdays of the form DD/MM/YYYY
# (e.g., 15/05/2000) and returns whether there is a match if the two people were born on the
# same day of the week (e.g., Friday). And then find out the age/s in years/months/days.

function formatDate(){
    # DD/MM/YYYY to YYYY/MM/DD
    IFS='/'
    read -a SPLITTED_AT_DELIMETER <<<"$1"
    unset IFS
    echo "${SPLITTED_AT_DELIMETER[2]}/${SPLITTED_AT_DELIMETER[1]}/${SPLITTED_AT_DELIMETER[0]}"
}

function convertSecondsToHumaizedFormat(){
    # Seconds to humanized format
    SECONDS=$1
    YEARS=$(($SECONDS / (86400*365) ))
    SECONDS=$(($SECONDS % (86400*365) ))
    MONTHS=$(($SECONDS / (86400*30) ))
    SECONDS=$(($SECONDS % (86400*30) ))
    DAYS=$(($SECONDS / 86400 ))
    SECONDS=$(($SECONDS % 86400 ))
    echo "$YEARS years, $MONTHS months, $DAYS days"
}

echo -n "Enter the first birthday : "
read FIRST_BIRTHDATE
echo -n "Enter the second birthday : "
read SECOND_BIRTHDATE

IFS="/"
read -a FIRST_BIRTHDATE_ARRAY <<<"$FIRST_BIRTHDATE"
read -a SECOND_BIRTHDATE_ARRAY <<<"$SECOND_BIRTHDATE"
unset IFS

FIRST_BIRTHDATE_FORMATTED=$(formatDate $FIRST_BIRTHDATE)
SECOND_BIRTHDATE_FORMATTED=$(formatDate $SECOND_BIRTHDATE)

# Required format -> YYYY/MM/DD
FIRST_BIRTHDATE_DAY=$(date -d "$FIRST_BIRTHDATE_FORMATTED" '+%A')
SECOND_BIRTHDATE_DAY=$(date -d "$SECOND_BIRTHDATE_FORMATTED" '+%A')

if [ $FIRST_BIRTHDATE_DAY == $SECOND_BIRTHDATE_DAY ]; then
    echo "Birthday matches !!"
else
    echo "Birthday does not match !!"
fi

# Todays date
TODAYS_DATE_FORMATTED=$(date '+%Y/%m/%d')

# Difference in seconds
FIRST_AGE_SECONDS=$(( $(date -d "$TODAYS_DATE_FORMATTED" '+%s') - $(date -d "$FIRST_BIRTHDATE_FORMATTED" '+%s') ))
SECOND_AGE_SECONDS=$(( $(date -d "$TODAYS_DATE_FORMATTED" '+%s') - $(date -d "$SECOND_BIRTHDATE_FORMATTED" '+%s') ))

# Convert seconds to humanized format
FIRST_AGE_HUMANIZED=$(convertSecondsToHumaizedFormat $FIRST_AGE_SECONDS)
SECOND_AGE_HUMANIZED=$(convertSecondsToHumaizedFormat $SECOND_AGE_SECONDS)

echo "first person age : $FIRST_AGE_HUMANIZED"
echo "second person age : $SECOND_AGE_HUMANIZED"