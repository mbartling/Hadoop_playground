#!/usr/bin/env python

from operator import itemgetter
import sys

#minVal = sys.float_info.max
#maxVal = sys.float_info.min
minVal = sys.maxint
maxVal = -sys.maxint -1
for line in sys.stdin:
    (key, value) = line.strip().split('\t')
    try:
        value = int(value)
    except ValueError:
        continue

    if key == "Max":
        maxVal = max(maxVal, value)
    elif key == "Min":
        minVal = min(minVal, value)

print 'Max\t%s' % maxVal
print 'Min\t%s' % minVal
