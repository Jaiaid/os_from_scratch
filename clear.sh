#!/bin/bash
files=`ls`

rm -vf *.bin

for file in $files
do
	if [ -d $file ];then
		rm -vf $file/*.bin $file/*.o
	fi
done
