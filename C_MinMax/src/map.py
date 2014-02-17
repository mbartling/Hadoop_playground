#!/usr/bin/env python
import sys
from operator import itemgetter

for line in sys.stdin:
    val = line.strip()
    val = val.split()
    #try:
    #    val = float(val)
    #except ValueError:
    #    continue
    maxVal = max(val)
    minVal = min(val)
    print "Max\t%s" % maxVal
    print "Min\t%s" % minVal
