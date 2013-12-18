package org.myorg;

import java.io.IOException;
import java.util.*;

import org.apache.hadoop.fs.Path;
import org.apache.hadoop.conf.*;
import org.apache.hadoop.io.*;
import org.apache.hadoop.mapred.*;
import org.apache.hadoop.util.*;

public class MinMaxNewAPI
{
    public class MinMaxMapper extends Mapper<LongWritable, Text, Text, IntWritable>
    {
        
    }

}

