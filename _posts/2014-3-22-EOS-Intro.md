---
layout: post
title: Introduction to EOS Cluster
---

## Table of Contents
- EOS Background
- Running Super Computing Jobs using PBS
- Hadoop on EOS
- EOS Gotchas!
- Useful Links

## Brief EOS Background

[Eos][1] is a 3168-core IBM (iDataPlex) Linux cluster comprised of two different types of nodes: 324 are equiped with Nehalem and 48 with Westmere processors, both Intel products. Both implement 64-bit architectures and operate at 2.8GHz. The Nehalem is the quad-core X5550 processor, while the Westmere is the 6-core X5660. The Nehalem-based nodes are 2-socket 8-core SMP systems. Those equipped with Westmeres are 2-socket 12-core SMP's. Eos currently has 372 (324+48) nodes, all interconnected by a high-speed 4X Quad-Data Rate (QDR) Infiniband (IB) fabric. An IB link in the fabric supports full-duplex communication with bandwidth of 4GB/s in each direction. The interconnecting switch in this communication infrastructure is the Voltaire Grid Director 4700.


The EOS system allocations are as follows

Directory | Quota | Backed Up? | Enviornment Variable | Description
---------- | ------- | ------------ | ---------------------- | ------------
**/g/home/$USER** | 1 GB | Yes | $HOME | Upon login, you will be situated in /g/home/$USER, where $USER is your logon name. Store smaller files such as your source code, executables, and input files. It is also the only area that will be backed up nightly.
**/scratch/$USER** | 50 GB | No | $SCRATCH | This is a high performance filesystem, intended to temporarily hold rather large files for your current processing needs. It is not intended as long-term storage of any files.
The $TMPDIR environment variable is also defined as /scratch/$USER/tmp.

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


## Getting Started: Hadoop on EOS

To find information on EOS account requests as well as how to connect to EOS cluster, go [Here][3].

### Example Hadoop PBS script
The following is an example PBS script for the WordCount method in C.
[C_WordCount/hadoopWC.job](https://github.com/mbartling/Hadoop_playground/blob/master/C_WordCount/hadoopWC.job)

## EOS Gotchas!

Running a Hadoop on a cluster means we must have set up our public and private SSH keys. So if you find _invalid permission_ errors in your Hadoop logs, chances are you haven't initialized your SSH keys.

To initialize SSH keys run the command `init-ssh` :

```
user@eos> init-ssh
```

which should create the necessary files in your `~/.ssh` directory.


Also don not forget to add your modules! i.e

```
user@eos> modules add git
```
## Useful links
- [EOS: Basic Information][1]
- [EOS Documentation][2]
- [Connecting to EOS][3]


[1]: http://sc.tamu.edu/help/eos/sysinfo.php 
[2]: http://sc.tamu.edu/help/
[3]: http://sc.tamu.edu/help/access/ 
[4]: http://sc.tamu.edu/help/ams/
