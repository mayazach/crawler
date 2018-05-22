#!/bin/bash

re='^[0-9]+$'

if [ "$#" != 4 ]; then
	echo "Need input parameters: root_directory text_file w p"
	exit 1
fi

if [ ! -e "$1" ];
then
    echo "Root directory must exist"
    exit 1
fi

if [ ! -e "$2" ];
then
    echo "Text file must exist"
    exit 1
fi

if ! [[ "$3" =~ $re ]] ;
then
    echo "w must be a positive integer"
    exit 1
fi

if ! [[ "$4" =~ $re ]] ;
then
    echo "p must be a positive integer"
    exit 1
fi

if [ $(cat "$2" | wc -l) -lt 10000 ];
then
    echo "Text file must have 10000 lines or more"
    exit 1
fi

q=`expr $3 / 2 + 1`
echo "$q"

f=`expr $4 / 2 + 1`
echo "$f"

for ((i=1; i <= "$3"; i++))
do
    mkdir "$1/site$i"
done

echo "Hello program"
