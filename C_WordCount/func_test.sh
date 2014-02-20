#!/bin/bash

cat temp.txt | ./map.out | sort | ./reduce.out
