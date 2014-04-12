---
layout: post
title: Example MapReduce in C, Friends of Friends
---

## Table of Contents
- Introduction
- Friends of Friends in Python
- Friends of Friends in C
- Useful Links

## Introduction
It is common for social networking sites to provide mutual friends lists. However, performing this computation every time we visit someone elses profile would be vastly inefficient. Facebook, and other social networking sites, circumvent this computational nightmare by running Friends of Friends (FoF) map reduction programs a couple times per week. Social networks can get away with these infrequent computations since even the most active users only add a few friends on ocassion. 

Now the FoF algorithm is a little different from word count and min max (word count and min max are accumulators). Steve Krenzel posted a good description of how the FoF algorithm works, which can be found [Here](http://stevekrenzel.com/finding-friends-with-mapreduce)

### Steve Krenzels FoF explanation
Facebook has a list of friends (note that friends are a bi-directional thing on Facebook. If I'm your friend, you're mine). They also have lots of disk space and they serve hundreds of millions of requests everyday. They've decided to pre-compute calculations when they can to reduce the processing time of requests. One common processing request is the "You and Joe have 230 friends in common" feature. When you visit someone's profile, you see a list of friends that you have in common. This list doesn't change frequently so it'd be wasteful to recalculate it every time you visited the profile (sure you could use a decent caching strategy, but then I wouldn't be able to continue writing about mapreduce for this problem). We're going to use mapreduce so that we can calculate everyone's common friends once a day and store those results. Later on it's just a quick lookup. We've got lots of disk, it's cheap.

Assume the friends are stored as `Person->[List of Friends]`, our friends list is then:

```
A -> B C D
B -> A C D E
C -> A B D E
D -> A B C E
E -> B C D
```

Each line will be an argument to a mapper. For every friend in the list of friends, the mapper will output a key-value pair. The key will be a friend along with the person. The value will be the list of friends. The key will be sorted so that the friends are in order, causing all pairs of friends to go to the same reducer. This is hard to explain with text, so let's just do it and see if you can see the pattern. After all the mappers are done running, you'll have a list like this:

For map(A -> B C D) :

```
(A B) -> B C D
(A C) -> B C D
(A D) -> B C D
```

For map(B -> A C D E) : (Note that A comes before B in the key)

```
(A B) -> A C D E
(B C) -> A C D E
(B D) -> A C D E
(B E) -> A C D E
```


For map(C -> A B D E) :

```
(A C) -> A B D E
(B C) -> A B D E
(C D) -> A B D E
(C E) -> A B D E
```

For map(D -> A B C E) :

```
(A D) -> A B C E
(B D) -> A B C E
(C D) -> A B C E
(D E) -> A B C E
```

And finally for map(E -> B C D):

```
(B E) -> B C D
(C E) -> B C D
(D E) -> B C D
```

Before we send these key-value pairs to the reducers, we group them by their keys and get:

```
(A B) -> (A C D E) (B C D)
(A C) -> (A B D E) (B C D)
(A D) -> (A B C E) (B C D)
(B C) -> (A B D E) (A C D E)
(B D) -> (A B C E) (A C D E)
(B E) -> (A C D E) (B C D)
(C D) -> (A B C E) (A B D E)
(C E) -> (A B D E) (B C D)
(D E) -> (A B C E) (B C D)
```

Each line will be passed as an argument to a reducer. The reduce function will simply intersect the lists of values and output the same key with the result of the intersection. For example, reduce((A B) -> (A C D E) (B C D)) will output (A B) : (C D) and means that friends A and B have C and D as common friends.

The result after reduction is:

```
(A B) -> (C D)
(A C) -> (B D)
(A D) -> (B C)
(B C) -> (A D E)
(B D) -> (A C E)
(B E) -> (C D)
(C D) -> (A B E)
(C E) -> (B D)
(D E) -> (B C)
```

Now when D visits B's profile, we can quickly look up (B D) and see that they have three friends in common, (A C E).

Please be sure you understand Steve's explanation of FoF before moving on.


## Algorithm in Python
### Map FoF in Python
The FoF python map takes an input "personOfInterest TAB friendslist\n" as the arguments. First we split the friends list and form pairs of the personOfInterest and their friends; this will become the keys. Next we make sure the key has the first name with the lowest lexigraphical order first. Finally we emit the <key, friendsList>

```python
#! /usr/bin/env python

import sys

for line in sys.stdin:
    line = line.strip()
    #we get main person TAB list of friends
    person, friendlist = line.split('\t')

    # Split the friends list into a list of names
    namelist = friendlist.split()
    
    # pair the person with someone in their list of friends. 
    # Output the first name as the one with the lower lexigraphical order. 
    for name in namelist:
        if person < name:
            print '%s %s\t%s' % (person, name, friendlist)
        else:
            print '%s %s\t%s' %  (name, person, friendlist)

```

Hadoop will automatically shuffle sort the <key,value> pairs, grouping by key, before running the reduce method.

### Reduce FoF in Python
Under construction, Consider it a Homework problem :).


### Functional Test
I provided a script, **create_random_friends.py**, which will create groups of friends in the form

```
PersonOfInterest TAB FriendList\n
```

which can be used via

```
$ cat sorted_names.txt | ./create_random_friends.py > test_inputs.txt
```

or 

```
$ cat sorted_names.txt | ./create_random_friends.py | ./map.py | sort | ./reduce.py
```

Dont forget to test each functional blocks outputs!

To run this on the super computer follow the steps in 
[Example PBS Breakdown](../22/pbs-breakdown.html)


## C Implementation

As strange as it may sound, the C code and the python code take close to the same number of lines to write.
So lets jump in!

### Map in C

Just like with Word count and min max, lets start by defining a myMapLines function and configure it to read from STDIN.


```c
void myMapLines( void )
{
    char line[MAX_LINE_LEN];
    char friend[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char output[2*MAX_LINE_LEN];
    char *result;
    int n = 0;
    int idx = 0;
    int comparison;
    int friend_offset;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        /* Convert everything to lower case and remove any numbers */
        while(line[idx])
        {
            if(line[idx] == '\n')
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;

        // We will put stuff here later
    }
}
```

Next, lets scan the line, counting the number of characters in the substring (before we hit whitespace). This is a neat little trick with `scanf(&line[idx], "%s%n", friend, &n);`. We will use this to get the key and to help iterate over the friends list.

```c
/*Read in string and count number of characters read*/
sscanf(&line[idx] , "%s%n", key, &n);
idx += n;
friend_offset = idx;
while( sscanf(&line[idx] , "%s%n", friend, &n) > 0)
{
    // Stuff will be here
}
```

Finally lets add in the lexigraphical ordering of keys, and continue with the <key,value> emission.

```c
idx += n;
comparison = strcmp(key, friend);
if( comparison > 0)
{
    //strcat(output, friend);
    strcpy(output, friend);
    strcat(output, " ");
    strcat(output, key);
    strcat(output, "\t");
    strcat(output, &line[friend_offset]);

}
if( comparison < 0)
{
    //strcat(output, key);
    strcpy(output, key);
    strcat(output, " ");
    strcat(output, friend);
    strcat(output, "\t");
    strcat(output, &line[friend_offset]);

}
puts(output);
```

The final map method should look like this:

```c
/*
 * map_friends.c
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

extern int errno;
int main(int argc, const char * argv[])
{

    myMapLines();
    exit(EXIT_SUCCESS);
}


void myMapLines( void )
{
    char line[MAX_LINE_LEN];
    char friend[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char output[2*MAX_LINE_LEN];
    char *result;
    int n = 0;
    int idx = 0;
    int comparison;
    int friend_offset;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        /* Convert everything to lower case and remove any numbers */
        while(line[idx])
        {
            if(line[idx] == '\n')
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;

        /*Read in string and count number of characters read*/
        sscanf(&line[idx] , "%s%n", key, &n);
        idx += n;
        friend_offset = idx;
        while( sscanf(&line[idx] , "%s%n", friend, &n) > 0)
        {
            //Clear the output string
            //memset(output, '\0', 2*MAX_LINE_LEN);

            idx += n;
            comparison = strcmp(key, friend);
            if( comparison > 0)
            {
                //strcat(output, friend);
                strcpy(output, friend);
                strcat(output, " ");
                strcat(output, key);
                strcat(output, "\t");
                strcat(output, &line[friend_offset]);

            }
            if( comparison < 0)
            {
                //strcat(output, key);
                strcpy(output, key);
                strcat(output, " ");
                strcat(output, friend);
                strcat(output, "\t");
                strcat(output, &line[friend_offset]);

            }
            puts(output);
        }
        idx = 0;
    }
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }
} //End myMapLines

```
### Reduce in C
The reduce in C is very similar to the map algorithm, except we now have to add a intersect friends function.

Here is the meat of the myReduceLines method:


```c
//Do some simple pattern matching
if( sscanf(line , "%[^\t]%n", key, &idx) > 0)
{
    strcpy(friends, &line[idx]);
    //Error log
    FPRINTF(stderr, "\tK: %s\n", key);
    FPRINTF(stderr, "\tN: %d\n", idx);
    FPRINTF(stderr, "\tFA: %s\n", friends);
    //Compare the keys like we did with the word count
    if(strcmp(prev_key,key) == 0) //If Keys are equal
    {
        key_count += 1;
        //Intersect
        intersect_friends(mutual_friends, friends);
    }else //Keys are not the same
    {
        if(key_count != 0)
        {
            strcpy(outputs, prev_key);
            strcat(outputs, "\t");
            strcat(outputs, mutual_friends);
            //printf("%s\t%d\n",prev_key,key_count);
            puts(outputs);
        }
            strcpy(prev_key,key);
            strcpy(mutual_friends,friends);
            key_count = 0;
    }// end else words not equivalent
}

```

The friend intersection method will do a rudimentary comparison.

```c
/* Todo: Optimize this later*/
void intersect_friends(char * str1, char * str2)
{
    char temp[MAX_LINE_LEN];
    char friend1[MAX_LINE_LEN];
    char friend2[MAX_LINE_LEN];

    int n1;
    int n2;
    int idx1 = 0;
    int idx2 = 0;

    while(sscanf(&str1[idx1] ,"%s%n", friend1, &n1) > 0)
    {
       idx1 += n1;
       idx2 = 0;

       while(sscanf(&str2[idx2], "%s%n", friend2, &n2) > 0)
       {
            idx2 += n2;
               // FPRINTF(stderr, "\tF1: %s\n",friend1);

            if(strcmp(friend1, friend2) == 0)
            {
                strcat(temp, friend2);
                strcat(temp, " ");

                FPRINTF(stderr, "\tF2: %s\n",friend2);
            }
       }

    }
    FPRINTF(stderr, "\tT: %s\n",temp);
    strcpy(str1, temp);
}
```

The resulting C file should look something like this.

```c
/*
 * reduce_words.c
 *
 *  Created on: Feb 20, 2014
 *      Author: bart4128
 */

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 	400
#define ZERO_BYTE 	    0

#define DEBUG_MODE 0

#if DEBUG_MODE
#define FPRINTF(...) fprintf(__VA_ARGS__)
#else
#define FPRINTF(...) /*Dummy*/
#endif

void myReduceLines(void);
void intersect_friends(char *, char *);
extern int errno;
int main(int argc, const char * argv[])
{

    myReduceLines();

    exit(EXIT_SUCCESS);
}

//http://www.daniweb.com/software-development/c/threads/206198/intersection-of-two-strings
void myReduceLines( void )
{
    char line[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char prev_key[MAX_LINE_LEN];
    char friends[MAX_LINE_LEN];
    char mutual_friends[MAX_LINE_LEN];
    char outputs[MAX_LINE_LEN];
    char *result;
    int key_count = 0;
    int idx;
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        while(line[idx])
        {
            if(line[idx] == '\n')
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;

        if( sscanf(line , "%[^\t]%n", key, &idx) > 0)
        {
            strcpy(friends, &line[idx]);
            FPRINTF(stderr, "\tK: %s\n", key);
            FPRINTF(stderr, "\tN: %d\n", idx);
            FPRINTF(stderr, "\tFA: %s\n", friends);
            if(strcmp(prev_key,key) == 0) //If Keys are equal
            {
                key_count += 1;
                //Intersect
                intersect_friends(mutual_friends, friends);
            }else //Keys are not the same
            {
                if(key_count != 0)
                {
                    strcpy(outputs, prev_key);
                    strcat(outputs, "\t");
                    strcat(outputs, mutual_friends);
                    //printf("%s\t%d\n",prev_key,key_count);
                    puts(outputs);
                }
                    strcpy(prev_key,key);
                    strcpy(mutual_friends,friends);
                    key_count = 0;
            }// end else words not equivalent
        }
    }
        //puts(outputs);
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }


}

/* Todo: Optimize this later*/
void intersect_friends(char * str1, char * str2)
{
    char temp[MAX_LINE_LEN];
    char friend1[MAX_LINE_LEN];
    char friend2[MAX_LINE_LEN];

    int n1;
    int n2;
    int idx1 = 0;
    int idx2 = 0;

    while(sscanf(&str1[idx1] ,"%s%n", friend1, &n1) > 0)
    {
       idx1 += n1;
       idx2 = 0;

       while(sscanf(&str2[idx2], "%s%n", friend2, &n2) > 0)
       {
            idx2 += n2;
         // FPRINTF(stderr, "\tF1: %s\n",friend1);

            if(strcmp(friend1, friend2) == 0)
            {
                strcat(temp, friend2);
                strcat(temp, " ");

                FPRINTF(stderr, "\tF2: %s\n",friend2);
            }
       }

    }
    FPRINTF(stderr, "\tT: %s\n",temp);
    strcpy(str1, temp);
}
```

Simply compile the code 

```
$ gcc -Wall -o map map_friends.c
$ gcc -Wall -o reduce reduce_friends.c
```

Run the functional tests like we have previously

```
$ cat sorted_names | ./create_random_friends.py | ./map | sort | ./reduce
```

Compare the results, and then try running on the super computer!
**Be SURE to modify the PBS script before running** i.e. `#PBS -M youremail.notmine.@gmail.com`

```
$ qsub hadoopFoF.job
```

After you get the email confirming job completion, check the outputs directory for successful results.


## Useful links
- [Map Reduce Collect Method](http://hadoop.apache.org/docs/r0.18.3/mapred_tutorial.html#OutputCollector)
- [Hadoop Streaming options][1]
- [Intersection of Strings](http://www.daniweb.com/software-development/c/threads/206198/intersection-of-two-strings)
- [Steve Krenzels FoF Explanation](http://stevekrenzel.com/finding-friends-with-mapreduce)

[1]: http://hadoop.apache.org/docs/r1.0.4/streaming.html#More+usage+examples 
