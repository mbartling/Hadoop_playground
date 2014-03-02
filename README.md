Hadoop_playground
=================

Hadoop_playground 


General Hadoop Tips on EOS
--------------------------

By default, the EOS system discards the Map-stage outputs automatically after Map-Reduce completes. Unfortunately, this makes it 
difficult to debug Hadoop Streaming programs. An easy way around this is to run the MR method 3 times, once with map and no reduce,
once with reduce and no map, and once with both map and reduce. This can be accomplished using

For example, to run streaming with no reduce:

hadoop jar $HADOOP_STREAMING \
	-mapper "./map.out" \
	-reducer NONE \
    -file map.out \
    -input $TAMUSC_USER_WORK_DIR/inputs/ \
	-output $TAMUSC_USER_WORK_DIR/outputs/maponly 


**Note** By running this code with a C-based word count program, I found that Map generally outputs 8 files 
labeled _Part-0000 - Part-0007_. However, Reduce generally only outputs a single file. I wonder how this holds for larger files.

 