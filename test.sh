#!/bin/bash

pass=0
fail=0

for f in testy/*.in
do
	TMPFILE1="$(mktemp)"
	TMPFILE2="$(mktemp)"

	./main <$f >$TMPFILE1 2>$TMPFILE2

  	if diff ${f%in}out $TMPFILE1
	then
		echo "${f%.in} out passed"
		let "pass += 1"
	else
		echo "${f%.in} out failed"
		let "fail += 1"
	fi

	if diff ${f%in}err $TMPFILE2
	then
		echo "${f%.in} err passed"
		let "pass += 1"
	else
		echo "${f%.in} err failed"
		let "fail += 1"
	fi

  	echo "Kod: $?."
done

echo "Passed files: $pass"
echo "Failed files: $fail"