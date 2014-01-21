# Michael Bartling

# Compiler Information
CC = gcc

# Compiler Flags
#CFLAGS = -c -Wall
CFLAGS = -Wall -o
# Linker Flags
LDFLAGS = 

EXECUTABLES = geninputs

OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLES)

geninputs:
	$(CC) $(CFLAGS) geninputs geninputs.c

clean: 
	rm -rf *.o $(EXECUTABLES)

MY_HADOOP_NODE=~/dummyRL/Hadoop
MY_HADOOP_USER=${MY_HADOOP_NODE}/user/gwwood

run:
	mkdir -p inputs
	cp ~/src/hadoop/mbartling/C_MinMax/func_test/file01  inputs
	# Create a fake Hadoop node
	# URL path to central server - XML file - we simplify this
	mkdir -p ${MY_HADOOP_NODE}
	# Create a fake user on that node
	rm -ir ${MY_HADOOP_USER}
	hadoop fs -mkdir ${MY_HADOOP_USER}
	export MY_HADOOP_URL=${MY_HADOOP_USER}
	mkdir -p minmax_classes
	mkdir -p jars
	javac -classpath ${HADOOP_HOME}/hadoop-core-1.2.1.jar -d minmax_classes MinMaxOldAPI.java
	jar -cvf jars/minmax.jar -C minmax_classes/ .
	# Set up the files in HADOOP FS
	hadoop fs -mkdir ${MY_HADOOP_URL}/MinMax
	hadoop fs -copyFromLocal jars/minmax.jar ${MY_HADOOP_URL}/MinMax/.
	hadoop fs -ls ${MY_HADOOP_URL}/MinMax
	echo '----  Put some test vectors in the input folder'
	hadoop fs -copyFromLocal local_inputs ${MY_HADOOP_URL}/MinMax/inputs
	hadoop fs -ls ${MY_HADOOP_URL}/MinMax/inputs
	echo '---- Run the MapReduce method'
	hadoop jar ${MY_HADOOP_URL}/MinMax/minmax.jar org.myorg.MinMaxOldAPI ${MY_HADOOP_URL}/MinMax/inputs ${MY_HADOOP_URL}/MinMax/outputs
	echo '---- See the results'
	echo '--Note we should get a maximum of 1073730233 '
	hadoop fs -cat ${MY_HADOOP_URL}/MinMax/outputs/*
	echo '---- Bring the results back to the user'
	hadoop fs -copyToLocal  ${MY_HADOOP_URL}/MinMax/outputs fetched_outputs
	echo '---- AND hopefully everything worked!'
