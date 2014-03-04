#!/usr/bin/env python

import sys

for line in sys.stdin:
    first,last = line.split()
    print last + ',' + first
