package org.myorg;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

/**
 * MinMax in Hadoop.
 * This Min Max uses a slighly older version of the Hadoop API because it was easier for me to understand the code in the old API vs the new API. 
 * The new API favors abstract classes over interfaces, and the documentation of these abstract classes is still a work in progress.
 *      For example, Mapper and Reducer are abstract classes in the new API
 * The new API combines JobConf, OutputCollector, and Reporter into a single class called Context.
 *
 * The old API is found in org.apache.hadoop.mapred
 * The new API is found in org.apache.hadoop.mapreduce
 *
 */
	
	public class MinMaxOldAPI {
	
	    public static class OldMapAPI extends MapReduceBase implements Mapper<LongWritable, Text, Text, IntWritable> {
	      private static final Text  maxWord = new Text("MAX");
          private static final Text  minWord = new Text("MIN");
	      
          // the mapping method just parses the input data for integers, emitting any integers it finds
          public void map(LongWritable key, Text value, OutputCollector<Text, IntWritable> output, Reporter reporter) throws IOException {
            
            //int localMax = Integer.MIN_VALUE;
            //int localMin = Integer.MAX_VALUE;

            //Split up the lines and parse for integers
	        String line = value.toString();
	        StringTokenizer tokenizer = new StringTokenizer(line);

	        while (tokenizer.hasMoreTokens()) {
	          //word.set(tokenizer.nextToken());
	          //output.collect(word, one);
              int parsedNumber = Integer.parseInt(tokenizer.nextToken());
                // Moved this to the reducer function
                //localMax = (parsedNumber > localMax) ? parsedNumber : localMax;
                //localMin = (parsedNumber < localMin) ? parsedNumber : localMin;


              //The key in this case is "dummy" and not completely necessary, 
              output.collect(new Text("dummy"), new IntWritable(parsedNumber));
	        }
            
            //I wanted the outputs to be grouped by min and max but then I realized the mapper should send different lines to the reducer
            //output.collect(maxWord, new IntWritable(localMax);
            //output.collect(minWord, new IntWritable(localMin);
            //output.collect(key, new IntWritable(localMax);
            //output.collect(key, new IntWritable(localMin);
            
	      }
	    }
	
        //The reducer actually finds the min and max of the data
	    public static class OldReduceAPI extends MapReduceBase implements Reducer<Text, IntWritable, Text, IntWritable> {
	      public void reduce(Text key, Iterator<IntWritable> values, OutputCollector<Text, IntWritable> output, Reporter reporter) throws IOException {

            int localMax = Integer.MIN_VALUE;
            int localMin = Integer.MAX_VALUE;
	        //int sum = 0;

	        while (values.hasNext()) {
	          //sum += values.next().get();
              //Grab the next value and see if it is greater than max or less than min
              int temp = values.next().get();
              localMax = Math.max(localMax, temp);
              localMin = Math.min(localMin, temp); 
	        }
	        //output.collect(key, new IntWritable(sum));
            //Output the results <Max/min, value>
	        output.collect(new Text("Maximum"), new IntWritable(localMax));
	        output.collect(new Text("Minimum"), new IntWritable(localMin));
	      }
	    }
	
    // The main MapReduce method
    // this method configures our code to run with hadoop
	    public static void main(String[] args) throws Exception {
          // Create a job configuration using our MinMax class
	      JobConf conf = new JobConf(MinMaxOldAPI.class);
          
          // nickname the job "minmax"
	      conf.setJobName("minmax");
	
          // The job will output a Text key ("Maximum" or "Minimum") and the value will be the maximum/minimum number
	      conf.setOutputKeyClass(Text.class);
	      conf.setOutputValueClass(IntWritable.class);
	
          // Configure the job to use our Mapper and Reducer
	      conf.setMapperClass(OldMapAPI.class);
	      //conf.setCombinerClass(OldReduceAPI.class); //In this case, we do not need a reducer
	      conf.setReducerClass(OldReduceAPI.class);
	
          // Use the default input and output Text classes (so we can write it to a human readable file)
	      conf.setInputFormat(TextInputFormat.class);
	      conf.setOutputFormat(TextOutputFormat.class);
	
          // The user will provide the path arguments for the input and output directories. ex)
            // hadoop jar $MY_HADOOP_URL/minmaxoldapi.jar org.myorg.MinMaxOldAPI $MY_HADOOP_URL/input $MY_HADOOP_URL/output
	      FileInputFormat.setInputPaths(conf, new Path(args[0]));
	      FileOutputFormat.setOutputPath(conf, new Path(args[1]));
	
	      JobClient.runJob(conf);
	    }
	}
	
