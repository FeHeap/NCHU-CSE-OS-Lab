#!/bin/sh

if [ $# -eq 0  -o  $# -eq 1 ] ; then
	
	echo "Welcome to the Number Guessing Game!"
	
	if [ $# -eq 0 ] ; then
		echo "Please enter the target number(1~10):\c"
		read target
	else
		target=$1
	fi
	
	if [ $target -lt 1  -o  $target -gt 10 ] ; then
		echo "Error! $target is not between 1 and 10."
		exit 1
	fi
else
	echo "Error! The number of parameters is Wrong."
	exit 1
fi


askGuessing() {
	echo "Please Enter a number between $1 and $2:\c"
}

upper=10
lower=1

while true ; do

	askGuessing $lower $upper
	read number

	if [ $number -eq $target ] ; then
		break
	fi

	if [ $number -ge $lower  -a  $number -le $upper ] ; then

		if [ $lower -le $target  -a  $target -lt $number ] ; then
			upper=$(($number - 1))
		else
			lower=$(($number + 1))
		fi

	fi
done

echo "Bingo!"
