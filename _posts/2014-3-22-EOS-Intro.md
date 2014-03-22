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
/g/home/$USER | 1 GB | Yes | $HOME | Upon login, you will be situated in /g/home/$USER, where $USER is your logon name. Store smaller files such as your source code, executables, and input files. It is also the only area that will be backed up nightly.
/scratch/$USER | 50 GB | No | $SCRATCH | This is a high performance filesystem, intended to temporarily hold rather large files for your current processing needs. It is not intended as long-term storage of any files.
The $TMPDIR environment variable is also defined as /scratch/$USER/tmp.

## PBS: Portable Batch Scripting
## Getting Started: Hadoop on EOS

To find information on EOS account requests as well as how to connect to EOS cluster, go [Here][3].



## EOS Gotchas!


## Useful links
- [EOS: Basic Information][1]
- [EOS Documentation][2]
- [Connecting to EOS][3]


[1]: http://sc.tamu.edu/help/eos/sysinfo.php 
[2]: http://sc.tamu.edu/help/
[3]: http://sc.tamu.edu/help/access/ 
