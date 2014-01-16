#! /bin/sh
rm *.txt
cp ../src/my* .
./myMap<file01 | sort -n | ./myCollect | sort -n | ./myReduce 
