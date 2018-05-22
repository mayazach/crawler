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

echo "Hello program"
