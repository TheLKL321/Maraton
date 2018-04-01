#!/bin/bash

for f in testy/*.in
do
	TMPFILE1="$(mktemp)"
	TMPFILE2="$(mktemp)"
	./main <$f >$TMPFILE1 2>$TMPFILE2

  	if diff ${f%in}out $TMPFILE1
	then
		echo "${f%.in} out passed"
	else
		echo "${f%.in} out failed"
	fi

	if diff ${f%in}err $TMPFILE2
	then
		echo "${f%.in} err passed"
	else
		echo "${f%.in} err failed"
	fi

  	echo "Kod: $?."
done