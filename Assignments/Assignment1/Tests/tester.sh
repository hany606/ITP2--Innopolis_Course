#!/bin/bash
for i in {1..36}; do
	cp $i input.txt
	../a.exe
	var=$(diff -w HanyHamedOutput.txt $i.clue)
 	if [[ $var ]]; then
 		echo Error on test $i:
 		echo $var
 	# else	
 		# echo Passed test $i:
 	fi	
done
