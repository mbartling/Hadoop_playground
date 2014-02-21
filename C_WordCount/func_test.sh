#!/bin/bash

#cat temp.txt | ./map.out | sort | ./reduce.out
cat test_docs/*.txt | ./map.out | sort | ./reduce.out > outputs/output.out
