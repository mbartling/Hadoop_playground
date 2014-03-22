---
layout: index
---

Hadoop_playground
=================

Hadoop_playground 

 - [**About Hadoop Playground**][1]

General Hadoop Tips on EOS
--------------------------

### Debugging Map and Reduce Separately

By default, the EOS system discards the Map-stage outputs automatically after Map-Reduce completes. Unfortunately, this makes it difficult to debug Hadoop Streaming programs. An easy way around this is to run the MR method 3 times, once with map and no reduce, once with reduce and no map, and once with both map and reduce. 

For example, to run streaming with no reduce:

	hadoop jar $HADOOP_STREAMING \
		-mapper "./map.out" \
		-reducer NONE \	
		-file map.out \
		-verbose \
		-input $TAMUSC_USER_WORK_DIR/inputs/ \
		-output $TAMUSC_USER_WORK_DIR/outputs/maponly 


**Note** By running this code with a C-based word count program, I found that Map generally outputs 8 files 
labeled _Part-0000 - Part-0007_. However, Reduce generally only outputs a single file. I wonder how this holds for larger files.

### Important Caveat
I found that using _hadoop -fs get_ does not overwrite files if they exist in the target directory nor does it tell us whether or not this condition occurs. I ran into the situation where I would run my code but no matter what changes I would make to my reduce program, the reduce-output files were empty. Verifying successful mapping was simple enough; I just used the method described in **Debugging Map and Reduce Separately** and at the map outputs directly. 

The Quick Fix for this issue with Hadoop fs -get/put is to always target a unique directory. For example, I used target folders with some form of **$PBS_JOBID** written in them (since the EOS PBS system gives us unique Job ID's).

http://sc.tamu.edu/help/eos/batch/


### More Advanced Debugging

According to the Hadoop Wiki, it should be possible to run _Clean-up_ scripts if either map and/or reduce fails. More information can be found at http://wiki.apache.org/hadoop/HowToDebugMapReducePrograms. 

http://srinathsview.blogspot.com/2012/05/debugging-hadoop-task-tracker-job.html

I will play around with some of these scripts and post my findings soon.

Something interesting I found (but have yet to try) is that the EOS PBS system lets us choose our std streams. I might be able to use this to create a named stderr stream for my C programs to write logs to. 


[1]: {{site.baseurl}}docs/about.md
