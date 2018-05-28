#!/bin/bash

re='^[0-9]+$'
filenames=()
incomingLinks=()
lines=()

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

if [ $3 == 1 ] ;
then
    echo "w can't be 1 - we need external links"
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

readarray -t lines < $2

q=`expr $3 / 2 + 1`

f=`expr $4 / 2 + 1`

for((i=0; i < $3; i++))
do
    random=0
    for((j=0; j < $4; j++))
    do
	random=$(($random + $((1 + RANDOM % 10000))))
	filenames+=("$1/site$i/page$j_$random.html")
	incomingLinks+=(0)
    done
done

lineNum=$(cat "$2" | wc -l)
fileNum=`expr $4 \* $3`

for ((i=0; i < "$3"; i++))
do
    echo "#Creating web site $i"
    mkdir "$1/site$i"
    for ((j=0; j < "$4"; j++))
    do
	usedlinks=()
	farray=()
	qarray=()
	fsize=0
	minlink=`expr $4 \* $i`
        maxlink=`expr $4 \* $i + $4 - 1`
	while [ "$fsize" -lt "$f" ]
	do
	    temp=$((RANDOM % $4))
	    if [ $temp != $j ];
		then
			temp=`expr $minlink + $temp`
			new="true"
			for ((i2=0; i2<${#usedlinks[*]}; i2++));
			do
				if [ $temp == ${usedlinks[i2]} ];
				then
					new="false"
				fi
			done
			if [ $new == "true" ];
			then
				usedlinks+=("$temp")
				farray+=("$temp")
				incomingLinks[temp]=1
				let "fsize=fsize+1"
			fi
		elif [ $4 == 2 ];
		then
			temp=`expr $minlink + $temp`
			new="true"
			for ((i2=0; i2<${#usedlinks[*]}; i2++));
			do
				if [ $temp == ${usedlinks[i2]} ];
				then
					new="false"
				fi
			done
			if [ $new == "true" ];
			then
				usedlinks+=("$temp")
				farray+=("$temp")
				incomingLinks[temp]=1
				let "fsize=fsize+1"
			fi
		elif [ $4 == 1 ];
		then
			temp=`expr $minlink + $temp`
			farray+=("$temp")
			incomingLinks[temp]=1
			let "fsize=fsize+1"
		fi
	done
	qsize=0
	usedlinks=()
    while [ "$qsize" -lt "$q" ]
    do
        temp=$((RANDOM % $fileNum))
	    new="true"
        if [ $temp -lt $minlink ];
        then
			for ((i2=0; i2<${#usedlinks[*]}; i2++));
			do
				if [ $temp == ${usedlinks[i2]} ];
				then
					new="false"
				fi
			done
			if [ $new == "true" ];
			then
				usedlinks+=("$temp")
				qarray+=("$temp")
				let "qsize=qsize+1"
			elif [ $3 == 2 ]
			then
				if [ $4 == 1 ]
				then
					qarray+=("$temp")
					let "qsize=qsize+1"
				fi
			fi
	    elif [ $temp -gt $maxlink ];
	    then
            for ((i2=0; i2<${#usedlinks[*]}; i2++));
            do
                if [ $temp == ${usedlinks[i2]} ];
                then
                    new="false"
                fi
            done
            if [ $new == "true" ];
            then
                usedlinks+=("$temp")
                qarray+=("$temp")
                let "qsize=qsize+1"
			elif [ $3 == 2 ]
			then
				if [ $4 == 1 ]
				then
					qarray+=("$temp")
					let "qsize=qsize+1"
				fi
            fi
	    fi
        done
	k=$((1 + RANDOM % $(($lineNum - 2000))))
	m=$((1001 + RANDOM % 1000))
	index=`expr $4 \* $i + $j`
	echo "#  Creating page ${filenames[index]} with $m lines starting at line $k"
	touch ${filenames[index]}
	echo "<!DOCTYPE html>" >> ${filenames[index]}
	echo "<html>" >> ${filenames[index]}
	echo "    <body>" >> ${filenames[index]}
	fq=`expr $f + $q`
	mfq=`expr $m / $fq`

	for((i2=0;i2 < "$fq";i2++))
	do
	    for((j2=0;j2 < "$mfq";j2++))
	    do
		offset=`expr $k + $i2 \* $fq + $j2`
		echo "    ${lines[offset]}<br>" >> ${filenames[index]}
	    done
	    if [ $i2 -lt $f ];
	    then
		echo "#  Adding link to ${filenames[${farray[i2]}]}"
		echo "    <a href=\"${filenames[${farray[i2]}]}\">Link $i2</a><br>" >> ${filenames[index]}
	    else
		qlink=`expr $i2 - $f`
		echo "#  Adding link to ${filenames[${qarray[qlink]}]}"
                echo "    <a href=\"${filenames[${qarray[qlink]}]}\">Link $i2</a><br>" >> ${filenames[index]}
	    fi
	done
	echo "    </body>" >> ${filenames[index]}
	echo "</html>" >> ${filenames[index]}
    done
done

complete="true"
for ((i=0; i<${#incomingLinks[*]}; i++));
do
    if [ ${#incomingLinks[*]} == 0 ];
	then
		complete="false"
	fi
done

if [ $complete == "true" ];
	then
		echo "#All pages have at least one incoming link"
	fi

echo "#Done"
