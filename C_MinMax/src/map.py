#! /usr/bin/python
import sys

for line in sys.stdin:
    val = line.strip()
    val = val.split()
    maxVal = max(val)
    minVal = min(val)
    print "Max\t%s" % maxVal
    print "Min\t%s" % minVal
