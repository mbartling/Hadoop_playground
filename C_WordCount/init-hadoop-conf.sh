#!/bin/sh
#
# initialize ~/.hadoop/conf for the user
#
if [ ! -d $HADOOP_CONF_DIR ] ; then
  mkdir -p $HADOOP_CONF_DIR
else
  echo "WARNING: $HADOOP_CONF_DIR already exists.  Only one Hadoop job can run at a time."
  echo "         Execution will continue but you may experience random failures/errors."
fi
# first, static files from current release
cp $TAMUSC_HADOOP_HOME/$TAMUSC_HADOOP_VERSION/conf/* $HADOOP_CONF_DIR 
# then overwrite with our static files
cp $TAMUSC_HADOOP_HOME/tamusc/conf/hadoop-env.sh $HADOOP_CONF_DIR
# then add our dynamic files
node1=`hostname`
cat $PBS_NODEFILE | sort | uniq | grep -v $node1 > $HADOOP_CONF_DIR/slaves
head -1 $HADOOP_CONF_DIR/slaves > $HADOOP_CONF_DIR/masters # for secondarynamenode
for file in `ls $TAMUSC_HADOOP_HOME/tamusc/conf/*.template` ; do
  hfile=`basename $file | sed 's/.template//g'`
  cat $file \
  | sed "s|USERNAME|hadoop-$USER-$PBS_JOBID|g" \
  | sed -e "s|localhost|$node1|g" $HADOOP_CONF \
  > $HADOOP_CONF_DIR/$hfile
done
# EOF
