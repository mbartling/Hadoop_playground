#! /usr/bin/python

import sys

minVal = sys.float_info.max
maxVal = sys.float_info.min

for line in sys.stdin:
    (key, value) = line.strip().split("\t")
    if key == "Max":
        maxVal = max(maxVal, value)
    elif key == "Min":
        minVal = min(minVal, value)

print "Max\t%s" % maxVal
print "Min\t%s" % minVal
