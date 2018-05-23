#!/bin/bash

re='^[0-9]+$'
filenames=()

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

if [ "$(ls -A websites)" ]; then
     echo "#Warning: directory is full, purging..."
     rm -rf websites/*
fi

q=`expr $3 / 2 + 1`

f=`expr $4 / 2 + 1`

for((i=0; i < $3; i++))
do
    random=0
    for((j=0; j < $4; j++))
    do
	random=$(($random + $((1 + RANDOM % 10000))))
	filenames+=("$1/site$i/page$j_$random.html")
    done
done

for ((i=0; i < "$3"; i++))
do
    mkdir "$1/site$i"
    for ((j=0; j < "$4"; j++))
    do
	index=`expr $4 \* $i + $j`
	page=${filenames[index]}
	touch page
    done
done

echo "Hello program"
