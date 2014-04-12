---
layout: post
title: Example MapReduce in C, Word Count
---

## Table of Contents
- Introduction
- Word Count in Python
- Word Count in C
- Useful Links

## Introduction
The word count example is basically the *Hello World!* of Hadoop. Essentially the goal is to find the number of times a specific word shows up in a large text. This so called Word Count example belongs to a class of map reduce schemes called accumulators. Basically, we feed accumulators blocks of data (usually as line-by-line chunks) to perform some sort of arithmetic operation on the data. For example, the Word count algorithm basically says

```
# Map
for each word:
    emit word, 1

# Reduce
for each (word,count):
    sum += count

emit word, sum

```

If, for instance, we are given a book then we can break the book up into chapters or pages and operate on those smaller chunks in parallel. In min max, we only had 2 keys (min, and max). However, wordcount has an arbitrary set of keys so we have to be a little bit more careful with our design.

Now lets see how this looks in python before we jump into the C based version.

## Algorithm in Python
This example and all associated Python code was originally from [Michael Noll's tutorial on Hadoop](http://www.michael-noll.com/tutorials/writing-an-hadoop-mapreduce-program-in-python).
All credit goes to [Michael Noll](http://www.michael-noll.com/tutorials/writing-an-hadoop-mapreduce-program-in-python/ "Mr. Nolls' tutorial is easily one of the best Hadoop tutorials online").

### Map Word Count in Python

The mapping code will read data from STDIN, split it into words and output a list of lines mapping words to their (intermediate) counts to STDOUT. The Map script will not compute an (intermediate) sum of a word’s occurrences though. Instead, it will output <word> 1 tuples immediately – even though a specific word might occur multiple times in the input. In our case we let the subsequent Reduce step do the final sum count. Of course, you can change this behavior in your own scripts as you please, but we will keep it like that in this tutorial because of didactic reasons.

```python
#!/usr/bin/env python

import sys

# input comes from STDIN (standard input)
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()
    # split the line into words
    words = line.split()
    # increase counters
    for word in words:
        # write the results to STDOUT (standard output);
        # what we output here will be the input for the
        # Reduce step, i.e. the input for reducer.py
        #
        # tab-delimited; the trivial word count is 1
        print '%s\t%s' % (word, 1)

```

Hadoop will automatically shuffle sort the <key,value> pairs, grouping by key, before running the reduce method.

### Reduce MinMax in Python
Reduce will read the results of mapper.py from STDIN (so the output format of mapper.py and the expected input format of reducer.py must match) and sum the occurrences of each word to a final count, and then output its results to STDOUT.

```python
#!/usr/bin/env python

from operator import itemgetter
import sys

current_word = None
current_count = 0
word = None

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parse the input we got from mapper.py
    word, count = line.split('\t', 1)

    # convert count (currently a string) to int
    try:
        count = int(count)
    except ValueError:
        # count was not a number, so silently
        # ignore/discard this line
        continue

    # this IF-switch only works because Hadoop sorts map output
    # by key (here: word) before it is passed to the reducer
    if current_word == word:
        current_count += count
    else:
        if current_word:
            # write result to STDOUT
            print '%s\t%s' % (current_word, current_count)
        current_count = count
        current_word = word

# do not forget to output the last word if needed!
if current_word == word:
    print '%s\t%s' % (current_word, current_count)
```

### Functional Test
Hadoop streaming is designed to behave like Unix Pipes, so what better way to test our code then to run it on a small local example! To do this, I ran the code on several files. When I tested this code on the super computer I used books taken from [gutenberg,org](http://www.gutenberg.org/ebooks/1661) local test used [this text](https://github.com/mbartling/Hadoop_playground/blob/master/C_WordCount/test_docs/hadoop_overview.txt) from the Hadoop website.

First thing we will want to do is `chmod +x` our map.py and reduce.py files. Next, lets test map.

```
$ cat hadoop_overview.txt | ./map.py
```

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/wc-functest-map.png)


Now lets pretend we are Hadoop and sort the outputs.

```
$ cat hadoop_overview.txt | ./map.py | sort -n
```

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/wc-functest-sort.png)

Finally, we run the reduce step

```
$ cat hadoop_overview.txt | ./map.py | sort -n | ./reduce.py
```

Results in:

![alt text](https://github.com/mbartling/Hadoop_playground/raw/gh-pages/images/wc-functest-reduce.png "That feeling when something does exactly what it is supposed to do! :P ")

Now that we have a model working, we can either test our python code on the super computer or dive into the C method!

To run this on the super computer follow the steps in 
[Example PBS Breakdown](../22/pbs-breakdown.html)


## C Implementation

Here we will port our python code over to C and call it **map_words.c** and **reduce_words.c**. 

So lets jump in!
### Map in C

first lets define a function, **myMapLines** to fill in for the mapping. We will process lines much the same way we did for word count, except now we need to throw out any punctuation.

```c
#define MAX_LINE_LEN 	200
void myMapLines(void)
{
	char line[MAX_LINE_LEN];
	char word[MAX_LINE_LEN];
    char *result;
	int n;
    int idx;
}
```

First off, lets get lines of data from STDIN using `while(fgets(line, sizeof(line), stdin) != NULL)` . 
Next we will remove any punctuation
```c
while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
{
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }
    //While char exists
    while(line[idx])
    {
        //Make everything lower case
        line[idx] = tolower(line[idx]);
        //Replace all punctuation with space
        if(ispunct(line[idx]) || isdigit(line[idx]))
        {
            line[idx] = ' ';
        }
        idx++;
    }
    //More Stuff Below
}
```

Finally lets reset the index then emit the key value pairs as previously discussed, add a main function, and include the necessary headers.

```c
idx = 0;
while( sscanf(&line[idx] , "%s%n", word, &n) > 0)
{
    idx += n;
    printf("%s\t%d\n",word, 1);
}
```

The final code should look like this:

```c
/*
 * map_words.c
 *
 *  Created on: Feb 20, 2014
 *      Author: bart4128
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 	200

void myMapLines(void);
void emit_word(char *line);

extern int errno;
int main(int argc, const char * argv[])
{
    myMapLines();

    exit(EXIT_SUCCESS);
}


void myMapLines( void )
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char *result;
    int n = 0;
    int idx = 0;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        //While char exists
        while(line[idx])
        {
            //Make everything lower case
            line[idx] = tolower(line[idx]);
            //Replace all punctuation with space
            if(ispunct(line[idx]) || isdigit(line[idx]))
            {
                line[idx] = ' ';
            }
            idx++;
        }
        
        //Reset the index
        idx = 0;
        while( sscanf(&line[idx] , "%s%n", word, &n) > 0)
        {
            idx += n;
            printf("%s\t%d\n",word, 1);
        }
        idx = 0;
    }
    //For safety
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }
}
```

### Reduce in C
Unlike the min max code, word count has arbitrary keys. Luckily, Hadoop automagically sorts the <key,value> pairs by key before passing them to reduce. This means we will process groups of the same key before moving to the next keys.

```c
//if this is the same word as the last one then increment count
//Otherwise reset count
if(strcmp(prev_word,word) == 0)
{
    word_count += n;
}else
{
    if(word_count != 0)
    {
        printf("%s\t%d\n",prev_word,word_count);
    }
        strcpy(prev_word,word);
        word_count = n;
}// end else words not equivalent
```

The over all code should look like this

```c
/*
 * reduce_words.c
 *
 *  Created on: Feb 20, 2014
 *      Author: bart4128
 */

#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 	200

void myReduceLines(void);

extern int errno;
int main(int argc, const char * argv[])
{

    myReduceLines();

    exit(EXIT_SUCCESS);
}


void myReduceLines( void )
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char prev_word[MAX_LINE_LEN];
    char *result;
    int n = 0;
    int word_count = 0;

    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        if( sscanf(line , "%s\t%d", word, &n) > 0)
        {
            //if this is the same word as the last one then increment count
            //Otherwise reset count
            if(strcmp(prev_word,word) == 0)
            {
                word_count += n;
            }else
            {
                if(word_count != 0)
                {
                    printf("%s\t%d\n",prev_word,word_count);
                }
                    strcpy(prev_word,word);
                    word_count = n;
            }// end else words not equivalent
        }
    }
    printf("%s\t%d\n",prev_word,word_count);
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }
}
```

Simply compile the code 

```
$ gcc -Wall -o map map_words.c
$ gcc -Wall -o reduce reduce_words.c
```

Run the functional tests like we did with python

```
$ cat hadoop_overview.txt | ./map | sort -n | ./reduce
```

Compare the results, and then try running on the super computer!
**Be SURE to modify the PBS script before running** i.e. `#PBS -M youremail.notmine.@gmail.com`

```
$ qsub hadoopWC.job
```

After you get the email confirming job completion, check the outputs directory for successful results.


## Useful links
- [Map Reduce Collect Method](http://hadoop.apache.org/docs/r0.18.3/mapred_tutorial.html#OutputCollector)
- [Hadoop Streaming options][1]
- [Michael Noll Hadoop Streaming Tutorial](http://www.michael-noll.com/tutorials/writing-an-hadoop-mapreduce-program-in-python)

[1]: http://hadoop.apache.org/docs/r1.0.4/streaming.html#More+usage+examples 
