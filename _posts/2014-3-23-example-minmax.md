---
layout: post
title: Example MapReduce in C, MinMax
---

## Table of Contents
- Introduction
- MinMax in Python
- MinMax in C
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

To run this on the super computer follow the steps in 
[Example PBS Breakdown](../22/pbs-breakdown.html)


## C Implementation

Here we will port our python code over to C and call it **map_simple.c** and **reduce.c**. I provided a more advance mapping method (map.c) to show a sneaky way of using Hadoop [Collect](http://hadoop.apache.org/docs/r0.18.3/mapred_tutorial.html#OutputCollector) to pre-process the Map outputs before sending the data off to the remote suffle sort. **Note:** I found out the hard way that **Collect methods must be written in the Hadoop Java API**.

So lets jump in!
### Map in C

To make key parsing easier lets define some max and min keys:

```c
enum
{
	key_max=1,
	key_min
};
```

Next lets define a function, **myMapLines** to fill in for the mapping. We will use doubles just in case we decide not to use integer values in the future. 

```c
void myMapLines(void)
{
	char line[1024], *pos, *end;
	int key;
	double val;
	double min_val;
	double max_val;

}
```

First off, lets get lines of data from STDIN using `while(fgets(line, sizeof(line), stdin))` . 
Every time we start working on a new line, we should set min_val and max_val to the **Maximum** and **Minimum** possible doubles respectively. This way we will always get a valid min/max number. Next we will process each element in the line, first converting the string to a double and placing it in *val*, followed by a comparison with the lines local min/max. 

```c
/** Get arbitrary number of numbers from the line */
while (fgets(line, sizeof(line), stdin)) 
{

	/* Set the limits so min and max are always set properly */
	min_val = DBL_MAX;
	max_val = DBL_MIN;

	/* Process each line separately */
	pos = line;
	/* position ourselves at the start of each number and move forward*/
	for (pos = line; ; pos = end) 
    {
		val = strtod(pos, &end);
		if (pos == end)
			break;
		// process val here

		min_val = (val < min_val) ? val: min_val;
		max_val = (val > max_val) ? val: max_val;

    }//end for
}//end While
```

Finally lets emit the key value pairs as previously discussed, add a main function, and include the necessary headers.

```c
printf("%d\t%lf\n", key_max, max_val);
printf("%d\t%lf\n", key_min, min_val);
```

The final code should look like this:

```c
/**
 * map.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Mike
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>

enum
{
	key_max=1,
	key_min
};

/**
 * Simple map function which reads in lines from STDIN and outputs the max and min of each line
 */
void myMapLines(void)
{
	char line[1024], *pos, *end;
	int key;
	double val;
	double min_val;
	double max_val;

	/** Get arbitrary number of numbers from the line */
	while (fgets(line, sizeof(line), stdin)) {

		/* Set the limits so min and max are always set properly */
		min_val = DBL_MAX;
		max_val = DBL_MIN;

		/* Process each line separately */
		pos = line;
		/* position ourselves at the start of each number and move forward*/
		for (pos = line; ; pos = end) 
        {
			val = strtod(pos, &end); //convert to double
			if (pos == end)
				break;
			// process val here

			min_val = (val < min_val) ? val: min_val;
			max_val = (val > max_val) ? val: max_val;

		} //End For


		printf("%d\t%lf\n", key_max, max_val);
		printf("%d\t%lf\n", key_min, min_val);
	}//end while(fgets)

}// End myMapLines

int main(void)
{

	myMapLines();
	return 0;
}
```

### Reduce in C
The reduce is much simpler than the mapper since it will always receive the same sized lines. In other words it will always see an int (the key) a tab and then a double (the value). Hence we can use `scanf("%d\t%lf\n", &key, &val)`.

So lets jump right in. We will start off defining the available keys and creating a myReduceLines function like we did with map. We need to add a few more variables to keep track of our max and min values.

```c
enum
{
	key_max=1,
	key_min
};

void myReduceLines(void)
{

	int32_t key;

	double val;
	double local_max_val;
	double local_min_val;

	/* Set the limits so min and max are always set properly */
	local_min_val = DBL_MAX;
	local_max_val = DBL_MIN;

}
```

Next we process each line, first by comparing the keys followed by comparing the value against our locally saved min and max values.

```c
while (scanf("%d\t%lf\n",&key, &val) > 0)
{
	switch(key)
	{
		case key_max:
		{
			local_max_val 	= (val > local_max_val) ? val: local_max_val;
			break
		}
		case key_min:
		 {
		 	local_min_val 	= (val < local_min_val) ? val: local_min_val;
		 	break;
		 }
		default: break; //silently ignore
	}
}// End While

printf("Max\t%lf\n", local_max_val);
printf("Min\t%lf\n", local_min_val);
```

The resulting C file should look something like this.

```c
/*
 * reduce.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Mike
 *
 *      Essentially the same thing as the collect function
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>
//#include <time.h>
#include <stdlib.h>


enum
{
	key_max=1,
	key_min
};

void myReduceLines(void)
{

	int32_t key;

	double val;
	double local_max_val;
	double local_min_val;

	/* Set the limits so min and max are always set properly */
	local_min_val = DBL_MAX;
	local_max_val = DBL_MIN;
	
	while (scanf("%d\t%lf\n",&key, &val) > 0)
	{

		switch(key)
		{
			case key_max:
			{
				local_max_val 	= (val > local_max_val) ? val: local_max_val;
				break
			}
			case key_min:
			 {
			 	local_min_val 	= (val < local_min_val) ? val: local_min_val;
			 	break;
			 }
			default: break; //silently ignore
		}

	}// End While

    //We can write the max and min out like this, however it is safer if we use our previous convention. 
    // This will let us "chain" reduce methods.
	//printf("Max\t%lf\n", local_max_val);
	//printf("Min\t%lf\n", local_min_val);
	
    printf("%d\t%lf\n", key_max, local_max_val);
    printf("%d\t%lf\n", key_min, local_min_val);
	
}// End myReduceLines

int main(void)
{
	myReduceLines();
	return 0;
}

```

Simply compile the code 

```
$ gcc -Wall -o map map_simple.c
$ gcc -Wall -o reduce reduce.c
```

Run the functional tests like we did with python

```
$ cat test_inputs.dat | ./map.py | sort -n | ./reduce.py
```

Compare the results, and then try running on the super computer!
**Be SURE to modify the PBS script before running** i.e. `#PBS -M youremail.notmine.@gmail.com`

```
$ qsub cminmax.job
```

After you get the email confirming job completion, check the outputs directory for successful results.


## Useful links
- [Map Reduce Collect Method](http://hadoop.apache.org/docs/r0.18.3/mapred_tutorial.html#OutputCollector)
- [Hadoop Streaming options][1]

[1]: http://hadoop.apache.org/docs/r1.0.4/streaming.html#More+usage+examples 
