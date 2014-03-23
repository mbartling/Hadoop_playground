---
layout: post
title: Example PBS Breakdown, Hadoop C Wordcount on EOS
---

## Table of Contents
- Introduction
- PBS Reference
- PBS Breakdown
- Useful Links

## Introduction

## PBS: Portable Batch Scripting

**We will use Portable Batch Scripts (PBS) to submit a Hadoop job for processing on the EOS cluster.**

A batch request is expressed through a batch file. That is, a text file, a job script, so called, with appropriate directives and other specifications or commands. A batch file, say, sample.job, consists of the batch directives section (top part) and the (UNIX) commands section. In the latter you specify all the commands that need to be executed. All PBS directives start with the #PBS string. Here is the general layout of a common PBS jobfile.

```
#PBS directive(s)1
#PBS directive(s)2
#PBS ...

#UNIX commands section. From here on down "#" on col 1 starts a comment
#<-- at this point $HOME is the current working directory
cmd1
cmd2
...
```
### Submission Options

Below are the commonly useful submission options. These options can be specified as #PBS option in your job script (recommended) or also on the command line for the qsub command. The **qsub** man page describes the available submission options in more detail.


Option | Description
------- | ----------
**-l billto=acctno** | Specifies the billing account to use for this job. Please consult the [AMS documentation][4] for more information.
**-l nodes=X:ppn=Y:gpus=Z** | Number of nodes (X), cores per node (Y) and gpus per node (Z) to be assigned to the job. For Nehalem and Westmere nodes, the maximum value of Y is 8 and 12, respectively. When gpus are specified, X must be between 1 and 4, and Z must be set to 1 or 2
**-l walltime=HH:MM:SS** | The limit on how long the job can run.
**-W x=NACCESSPOLICY:SINGLEJOB** | Specifies that PBS will not schedule other jobs on any of the engaged nodes than the present job, regardless of the value of ppn. This is a useful option when, for issues of performance for example, the sharing of nodes with other jobs is undesirable. Usage per node will be assessed 8 (or 12) * wall_clock_time.
**-N jobname** | Name of the job. When used with the -j oe option, the job's output will be directed to a file named **jobname.oXXXX** where XXXX is the job id.
**-S shell** | Shell for interpreting the job script. Recommended shell is /bin/bash.
**-q queue_name** | Directs the submitted job to the queue_name queue. On Eos this option should be exercised only by contributors or when submitting jobs to the gpu nodes.
**-m [a,b,e,n]** | If not specified, mail is sent only when the job is aborted.
**-M email_address(es)** | The email addresses to send mail about the job. Using an external email address (eg. @tamu.edu, @gmail.com, etc.) is recommended.

- a: mail is sent when the job is aborted by the batch system.
- b: mail is sent when the job begins
- e: mail is sent when the job ends
- n: mail is never sent


In General, when we are running Hadoop jobs on the TAMU EOS cluster we will use the following PBS configuration:

```
# PBS settings
#
#  - nodes must be >= 2 (2 is best for interactive, larger jobs need batch mode)
#  - ppn must be >= 8
#  - mem should be == nodes X 20gb
#  - walltime should be adjusted as needed
#  - Hadoop requires the use of -W x=NACCESSPOLICY:SINGLEJOB

#PBS -l nodes=2:ppn=8,mem=40gb,walltime=1:00:00
#PBS -W x=NACCESSPOLICY:SINGLEJOB
#PBS -N Hadoop_CWordCount
#PBS -S /bin/bash
#PBS -m abe
#PBS -j oe
#PBS -M michael.bartling15+PBS@gmail.com
```
### Environment Variables

Below are some environment variables that can be accessed from a job script. See the qsub man page for the complete list of Torque environment variables.

Variable | Description
-------- | -----------
**$PBS_JOBID** | Contains the job id.
**$PBS_O_WORKDIR** | Contains the directory from which the job was submitted.
**$PBS_NODEFILE** | Stores the name of the file that contains the list of compute nodes assigned to the job. A useful command to capture the contents of the list may be, cat $PBS_NODEFILE.

### Useful Commands

Below we list commonly used commands to track and interact with the batch system. You will find fuller (may be) explanation using the corresponding man pages.

Command | Description
------- | -----------
**qsub jobfile** | Submits a job script (jobfile) or also can be used to start an interactive batch job.
**qstat -a** | View information about queued jobs.
**qstat -f jobid** | View detailed information (such as assigned nodes and resource usage) about queued jobs.
**showq** | View information about queued jobs. Useful options are -r (show running jobs), -i (show idle jobs that are eligible for scheduling, and -b (show blocked jobs that are ineligible for scheduling due to queue policies).
**checkjob jobid** | View status of your job with id = jobid. Use the -v option to view additional information for why a queued job is waiting.
**qdel jobid canceljob jobid (Maui)** | Terminates a job with id = jobid
**qlimit** | View the status of the queues.
**jrpt jobid** | Lists resource use information at the OS process level. An in-house command, it is useful in tracking the efficient (or inefficient) use of resources, especially for parallel jobs.The listed data is most accurate when jobs allocate whole nodes. Periodic use may also be required in order to obtain a realistic "sample" for the listed data. Important: To invoke jrpt in the form we show, you must first add the following command to your .bashrc control file on Eos: alias jrpt= **/g/software/PBSutils/jrpt/bin/jrpt** .


## PBS Breakdown: C-Wordcount example

The following is an example PBS script for the WordCount method in C.
C_WordCount/hadoopWC.job
https://github.com/mbartling/Hadoop_playground/blob/master/C_WordCount/hadoopWC.job

### PBS Configuration
As mentioned above, we will generally configure the Hadoop PBS scripts with

```
# PBS settings
#
#  - nodes must be >= 2 (2 is best for interactive, larger jobs need batch mode)
#  - ppn must be >= 8
#  - mem should be == nodes X 20gb
#  - walltime should be adjusted as needed
#  - Hadoop requires the use of -W x=NACCESSPOLICY:SINGLEJOB

#PBS -l nodes=2:ppn=8,mem=40gb,walltime=1:00:00
#PBS -W x=NACCESSPOLICY:SINGLEJOB
#PBS -N Hadoop_CWordCount
#PBS -S /bin/bash
#PBS -m abe
#PBS -j oe
#PBS -M michael.bartling15+PBS@gmail.com
```
### Batch mode vs Interactive mode

We have to options for how we will run Hadoop on EOS:
1. Batch Processing (Default)
2. Interactive Mode

```
export INTERACTIVE=0  # 0 = batch mode ; 1 = interactive VNC session
```

For the most part, we will operate in Batch processing mode. Part of the reason for this is I have, so far, been unsuccessful in getting interactive mode to work ~~reliably~~.

### The Meat of the PBS Script


## Useful links
- [EOS: Basic Information][1]
- [EOS Documentation][2]
- [Connecting to EOS][3]


[1]: http://sc.tamu.edu/help/eos/sysinfo.php 
[2]: http://sc.tamu.edu/help/
[3]: http://sc.tamu.edu/help/access/ 
[4]: http://sc.tamu.edu/help/ams/
