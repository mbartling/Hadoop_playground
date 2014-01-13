#HADOOP_HOME = /usr/share/hadoop/mapreduce/
#hadoop jar $HADOOP_HOME/hadoop-streaming.jar \
hadoop jar /usr/share/hadoop/mapreduce/hadoop-streaming.jar \
	-input inputs \
	-output outputs \
	-mapper src/myMap \
	-combiner src/myCollect \
	-reducer src/myReduce 
