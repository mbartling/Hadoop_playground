#! /bin/sh
rm *.txt
cp ../src/geninputs .
./geninputs 2000 >> file01
cp ../src/my* .
./myMap<file01 | sort -n | ./myCollect | sort -n | ./myReduce 
