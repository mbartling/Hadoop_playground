---
layout: post
title: Example MapReduce in C, MinMax
---

## Table of Contents
- Introduction
- Useful Links

## Introduction

## Algorithm in Python

### Map MinMax in Python
```python
#!/usr/bin/env Python
import sys

for line in sys.stdin:
    val = line.strip()
    val = val.split()
    
    # Find the max and min of each input line
    maxVal = max(val)
    minVal = min(val)

    # We have 2 Keys, 1. Max and 2. Min
    print "Max\t%s" % maxVal
    print "Min\t%s" % minVal
```

### Reduce MinMax in Python

```python
#!/usr/bin/env python

import sys

minVal = sys.maxint
maxVal = -sys.maxint -1

# Note: Hadoop automagically shuffle sorts data by Key before handing it to the reduce tasks.

for line in sys.stdin:
    # The lines are coming in as "KEY -TAB- VALUE_LIST"
    (key, value) = line.strip().split('\t')

    # If we see something we dont understand, ignore it
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
```

## C Implementation

## Useful links
- [EOS: Basic Information][1]
- [EOS Documentation][2]
- [Connecting to EOS][3]
- [Hadoop Distributed File System][5]
- [Hadoop Streaming options][6]

