#! /bin/bash


cat 1-2.cnf | ./generateKeys.py >mapKeyList.txt

cat 1-2.cnf | ./mapCNF.py | sort -n
