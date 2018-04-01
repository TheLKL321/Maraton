#!/bin/bash

for f in testy/*.in
do
	./main <$f >${f%.in}_wyn.out 2>${f%.in}_wyn.err

  	if diff ${f%in}out ${f%.in}_wyn.out
	then
		echo "${f%.in} out passed"
	else
		echo "${f%.in} out failed"
	fi

	if diff ${f%in}err ${f%.in}_wyn.err
	then
		echo "${f%.in} err passed"
	else
		echo "${f%.in} err failed"
	fi

  	echo "Kod: $?."
done