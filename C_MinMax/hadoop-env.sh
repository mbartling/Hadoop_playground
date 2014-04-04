# Set Hadoop-specific environment variables here.

# local settings

export TAMUSC_HADOOP_HOME=/g/software/hadoop
export TAMUSC_HADOOP_VERSION=1.2.1
export TAMUSC_JAVA_HOME=/g/software/java6/jdk1.6.0_33 # need to work on modules here
export TAMUSC_HADOOP_PATH=$TAMUSC_HADOOP_HOME/$TAMUSC_HADOOP_VERSION/bin:$TAMUSC_HADOOP_HOME/tamusc/bin

export TAMUSC_USER_WORK_DIR=/work/hadoop-$USER-$PBS_JOBID
export TAMUSC_USER_TMP_DIR=/tmp/hadoop-$USER-$PBS_JOBID
export TAMUSC_USER_HADOOP_HOME=$HOME/.hadoop
export TAMUSC_USER_CONF_DIR=$TAMUSC_USER_HADOOP_HOME/conf
export TAMUSC_USER_LOG_DIR=$TAMUSC_USER_HADOOP_HOME/logs

# Hadoop settings

# The only required environment variable is JAVA_HOME.  All others are
# optional.  When running a distributed configuration it is best to
# set JAVA_HOME in this file, so that it is correctly defined on
# remote nodes.

export JAVA_HOME=$TAMUSC_JAVA_HOME # The java implementation to use.  Required.
export HADOOP_CONF_DIR=$TAMUSC_USER_CONF_DIR # this is important
export TMP=$TAMUSC_USER_TMP_DIR
export PATH=$TAMUSC_HADOOP_PATH:$PATH

# Extra Java CLASSPATH elements.  Optional.
# export HADOOP_CLASSPATH=

# The maximum amount of heap to use, in MB. Default is 1000.
# export HADOOP_HEAPSIZE=2000
export HADOOP_HEAPSIZE=6000

# Extra Java runtime options.  Empty by default.
# export HADOOP_OPTS=-server

# Command specific options appended to HADOOP_OPTS when specified
# TAMUSC/jack - note: these are mostly from TACC and haven't been investigated
export HADOOP_NAMENODE_OPTS="-XX:+UseParallelGC -Dcom.sun.management.jmxremote $HADOOP_NAMENODE_OPTS"
export HADOOP_SECONDARYNAMENODE_OPTS="-Dcom.sun.management.jmxremote $HADOOP_SECONDARYNAMENODE_OPTS"
export HADOOP_DATANODE_OPTS="-Dcom.sun.management.jmxremote $HADOOP_DATANODE_OPTS"
export HADOOP_BALANCER_OPTS="-Dcom.sun.management.jmxremote $HADOOP_BALANCER_OPTS"
export HADOOP_JOBTRACKER_OPTS="-Dcom.sun.management.jmxremote $HADOOP_JOBTRACKER_OPTS"
# export HADOOP_TASKTRACKER_OPTS=
# The following applies to multiple commands (fs, dfs, fsck, distcp etc)
# export HADOOP_CLIENT_OPTS

# Extra ssh options.  Empty by default.
# export HADOOP_SSH_OPTS="-o ConnectTimeout=1 -o SendEnv=HADOOP_CONF_DIR"

# Where log files are stored.  $HADOOP_HOME/logs by default.
export HADOOP_LOG_DIR=$TAMUSC_USER_LOG_DIR

# File naming remote slave hosts.  $HADOOP_HOME/conf/slaves by default.
export HADOOP_SLAVES=$HADOOP_CONF_DIR/slaves

# host:path where hadoop code should be rsync'd from.  Unset by default.
# export HADOOP_MASTER=master:/home/$USER/src/hadoop

# Seconds to sleep between slave commands.  Unset by default.  This
# can be useful in large clusters, where, e.g., slave rsyncs can
# otherwise arrive faster than the master can service them.
export HADOOP_SLAVE_SLEEP=10 # TAMUSC/jack - eos GPS is slow... may need to adjust to make reliable

# The directory where pid files are stored. /tmp by default.
export HADOOP_PID_DIR=$TAMUSC_USER_WORK_DIR/pids # TAMUSC/jack - keep pids on non-shared filesystem

# A string representing this instance of hadoop. $USER by default.
# export HADOOP_IDENT_STRING=$USER

# The scheduling priority for daemon processes.  See 'man nice'.
# export HADOOP_NICENESS=0 # TAMUSC/jack - we allocate entire nodes... no niceities required

# EOF
