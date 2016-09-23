#! /bin/bash

N=0
while [ $N -lt 52 ]; do
    export INFILES=`ls /minerva/data/users/afilkins/rwgt/me000z200i/$N/g4numi*.root`
    export OUTDIR="/minerva/data/users/afilkins/rwgt/me000z200i"
    export PPFX_DIR="/minerva/app/users/afilkins/Summer-2016/ppfx"
    
# create a file with a list of /pnfs/file/name local_subdir
# this will be used with the ifdh cp -f
    if [ -f "$OUTDIR/$N/rwgt_file_list.txt" ]
    then 
	rm $OUTDIR/$N/rwgt_file_list.txt
    fi
    
    for f in `ls $INFILES`; do
	echo $f ./inputs/`basename $f`
    done > $OUTDIR/$N/rwgt_file_list.txt



    export LOGFILE=rwgt$N.log

    jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e LOGFILE -e PPFX_DIR -f $OUTDIR/$N/rwgt_file_list.txt -dRWGT $OUTDIR file:///minerva/app/users/afilkins/Summer-2016/ppfx/scripts/rwgt_job.sh

    N=$[$N+1]
done



N=0
while [ $N -lt 15 ];do

    export INFILES=`ls /minerva/data/users/afilkins/rwgt/me000z-200i/$N/g4numi*.root`
    export OUTDIR="/minerva/data/users/afilkins/rwgt/me000z-200i"
    
    if [ -f "$OUTDIR/$N/rwgt_file_list.txt" ]
    then 
	rm $OUTDIR/$N/rwgt_file_list.txt
    fi
    
    for f in `ls $INFILES`; do
	echo $f ./inputs/`basename $f`
    done > $OUTDIR/$N/rwgt_file_list.txt

    export LOGFILE=rwgt$N.log

    jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e LOGFILE -e PPFX_DIR -f $OUTDIR/$N/rwgt_file_list.txt -dRWGT $OUTDIR file:///minerva/app/users/afilkins/Summer-2016/ppfx/scripts/rwgt_job.sh

    N=$[$N+1]
done