#!/bin/sh


output () {

	echo `basename $1`$2

}


compile () {

	ls $2 | while read file

		do

		result=`output $file $3`

		echo "Compiling $file as $result using $1"

		$1 -c -o $result $file

	done

}


link () {

	echo "Linking $2 as $3 using $1"

	$1 -o $3 -l readline `ls $2`

}


clean () {

	echo "Deleting $@"

	rm $@ 2>/dev/null

}


options () {

	if [ "$1" == "clean" ]

	then
		clean terminal '*.o'

		exit
	else
		compile cc '*.c' '.o'

		link cc '*.o' terminal

		clean *.o
	fi

}


options $@
