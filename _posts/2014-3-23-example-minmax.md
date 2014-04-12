---
layout: post
title: Example MapReduce in C, MinMax
---

## Table of Contents
- Introduction
- Useful Links

## Introduction
Imagine we have a massive file ( ~10+ TB) and we want to know the maximum and minimum values in the file. This so called MinMax example belongs to a class of map reduce schemes called accumulators. Basically, we feed accumulators blocks of data (usually as line-by-line chunks) to perform some sort of arithmetic operation on the data. For example, the MinMax algorithm below could be written in Matlab as 

```
maxVal = max(max(dataFile));
minVal = min(min(dataFile));

```

Of course, Matlab would crash very quickly if we tried to run this simple Matlab script on a multi-(giga/tera)byte file. So lets break up the MinMax problem and map reduce it!

## General Approach
By default, the Hadoop streaming interface processes <key,value> pairs in the following format "KEY TAB values(s)NEWLINE". This actually works out well with the min max algorithm. Basically we will make,


```
maxVal = max(max(dataFile));
minVal = min(min(dataFile));

```
into

```
% Map
maxVals = max(dataFile);
minVals = min(dataFile);

% Reduce
maxVal = max(maxVals);
minVal = min(minVals);

```

Easy right? Now lets see how this looks in python before we jump into the C based version.

## Algorithm in Python
### Map MinMax in Python
Given lines of integers, map will emit 2 <key, value> pairs per line: The first is "Max" followed by the maximum value on the line, and the second is "Min" followed by the minimum value on the line.

```python
#!/usr/bin/env Python
import sys

for line in sys.stdin:
    # Strip off any extra white space and tokenize the line
    val = line.strip()
    val = val.split()
    
    # Find the max and min of each input line
    maxVal = max(val)
    minVal = min(val)

    # We have 2 Keys, 1. Max and 2. Min per line
    print "Max\t%s" % maxVal
    print "Min\t%s" % minVal
```

Hadoop will automatically shuffle sort the <key,value> pairs, grouping by key, before running the reduce method.

### Reduce MinMax in Python
Reduce operates in much the same way as map, look for the max-of-maxes and min-of-mins then emit these values.

```python
#!/usr/bin/env python

import sys

# Set the global max and min
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

### Functional Test
Hadoop streaming is designed to behave like Unix Pipes, so what better way to test our code then to run it on a small local example! To do this, I provided a simple executable called "geninputs" which creates an arbitrary number of lines of integer numbers. 

First thing we will want to do is `chmod +x` our map.py and reduce.py files. Next, lets generate some inputs to play with

```
$ ./geninputs 5 > test_inputs.dat
```
This will generate 5 lines of integers to feed into map.
Next lets see if map works as expected (map should produce 2 lines for each input line).

```
$ cat test_inputs.dat | ./map.py 
```
The outputs should look something like this (But the numbers will be different):

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/minmax-functest-map.png "Its a good practice to run this twice and make sure you get the same results")

Now lets pretend we are Hadoop and sort the outputs.

```
$ cat test_inputs.dat | ./map.py | sort -n
```

Results in:

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/minmax-functest-sort.png "Now things are starting to come together!")

Finally, we run the reduce step

```
$ cat test_inputs.dat | ./map.py | sort -n | ./reduce.py
```

Results in:

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/minmax-functest-reduce.png "That feeling when something does exactly what it is supposed to do! :P ")

Now that we have a model working, we can either test our python code on the super computer or dive into the C method!


## C Implementation

## Useful links
- [EOS: Basic Information][1]
- [EOS Documentation][2]
- [Connecting to EOS][3]
- [Hadoop Distributed File System][5]
- [Hadoop Streaming options][6]

