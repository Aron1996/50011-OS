#!/bin/bash
content=$(<"$1")
length=${#content}
score=0
if [ $length -le 32 ] && [ $length -ge 6 ] ; then
	let score=length
	special=$(egrep [#$\+%@] $1)
	alpha=$(egrep [a-zA-Z] $1)
	number=$(egrep [0-9] $1)
	egrep "([a-zA-Z])\1+" $1 > outfile
	egrep "[a-z][a-z][a-z]" $1 > outfile2
	egrep "[A-Z][A-Z][A-Z]" $1 > outfile3
	egrep "[0-9][0-9][0-9]" $1 > outfile4
	if [ ! -z $special ] ;then
		let score=score+5
	fi
	if [ ! -z $alpha ] ;then
		let score=score+5
	fi
	if [ ! -z $number ] ;then
		let score=score+5
	fi

	if [ -s outfile ] ;then
		let score=score-10
	fi
	if [ -s outfile2 ] ;then
		let score=score-3
	fi
	if [ -s outfile3 ] ;then
		let score=score-3
	fi
	if [ -s outfile4 ] ;then
		let score=score-3
	fi
	echo "Password Score:" $score
else
	echo "Error:Password length invalid."
fi
rm -f outfile*
