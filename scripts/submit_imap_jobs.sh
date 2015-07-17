#! /bin/bash

export INFILES="/pnfs/minerva/mc_generation_flux/mc-flux/mc/g4numiv5/00/00/00/05/g4numiv5_dk2nu_minerva1_le010z185i_04[0-9][0-9]_0005.root"
export OUTDIR="/minerva/data/users/kordosky/imap"

# create a file with a list of /pnfs/file/name local_subdir
# this will be used with the ifdh cp -f
if [ -f "$OUTDIR/imap_file_list.txt" ]
then 
    rm $OUTDIR/imap_file_list.txt
fi

for f in `ls $INFILES`; do
    echo $f ./inputs/`basename $f`
done > $OUTDIR/imap_file_list.txt

export ELOW=3
export EHIGH=4
export INU=14
export CUTNA49=0
export CUTMIPP=0
export OUTFILE=all_3_4.root
export LOGFILE=all_3_4.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=3
export EHIGH=4
export INU=14
export CUTNA49=0
export CUTMIPP=1
export OUTFILE=nomipp_3_4.root
export LOGFILE=nomipp_3_4.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


export ELOW=3
export EHIGH=4
export INU=14
export CUTNA49=1
export CUTMIPP=0
export OUTFILE=nona49_3_4.root
export LOGFILE=nona49_3_4.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=3
export EHIGH=4
export INU=14
export CUTNA49=1
export CUTMIPP=1
export OUTFILE=nomipp_nona49_3_4.root
export LOGFILE=nomipp_nona49_3_4.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


########################### 6-8 GeV ###############################

export ELOW=6
export EHIGH=8
export INU=14
export CUTNA49=0
export CUTMIPP=0
export OUTFILE=all_6_8.root
export LOGFILE=all_6_8.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=6
export EHIGH=8
export INU=14
export CUTNA49=0
export CUTMIPP=1
export OUTFILE=nomipp_6_8.root
export LOGFILE=nomipp_6_8.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


export ELOW=6
export EHIGH=8
export INU=14
export CUTNA49=1
export CUTMIPP=0
export OUTFILE=nona49_6_8.root
export LOGFILE=nona49_6_8.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=6
export EHIGH=8
export INU=14
export CUTNA49=1
export CUTMIPP=1
export OUTFILE=nomipp_nona49_6_8.root
export LOGFILE=nomipp_nona49_6_8.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

############################## 15-20 ###################################

export ELOW=15
export EHIGH=20
export INU=14
export CUTNA49=0
export CUTMIPP=0
export OUTFILE=all_15_20.root
export LOGFILE=all_15_20.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=15
export EHIGH=20
export INU=14
export CUTNA49=0
export CUTMIPP=1
export OUTFILE=nomipp_15_20.root
export LOGFILE=nomipp_15_20.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


export ELOW=15
export EHIGH=20
export INU=14
export CUTNA49=1
export CUTMIPP=0
export OUTFILE=nona49_15_20.root
export LOGFILE=nona49_15_20.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=15
export EHIGH=20
export INU=14
export CUTNA49=1
export CUTMIPP=1
export OUTFILE=nomipp_nona49_15_20.root
export LOGFILE=nomipp_nona49_15_20.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


############################## 30-40 ###################################

export ELOW=30
export EHIGH=40
export INU=14
export CUTNA49=0
export CUTMIPP=0
export OUTFILE=all_30_40.root
export LOGFILE=all_30_40.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=30
export EHIGH=40
export INU=14
export CUTNA49=0
export CUTMIPP=1
export OUTFILE=nomipp_30_40.root
export LOGFILE=nomipp_30_40.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


export ELOW=30
export EHIGH=40
export INU=14
export CUTNA49=1
export CUTMIPP=0
export OUTFILE=nona49_30_40.root
export LOGFILE=nona49_30_40.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=30
export EHIGH=40
export INU=14
export CUTNA49=1
export CUTMIPP=1
export OUTFILE=nomipp_nona49_30_40.root
export LOGFILE=nomipp_nona49_30_40.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

############################## 0-120 ###################################

export ELOW=0
export EHIGH=120
export INU=14
export CUTNA49=0
export CUTMIPP=0
export OUTFILE=all_0_120.root
export LOGFILE=all_0_120.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=0
export EHIGH=120
export INU=14
export CUTNA49=0
export CUTMIPP=1
export OUTFILE=nomipp_0_120.root
export LOGFILE=nomipp_0_120.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 


export ELOW=0
export EHIGH=120
export INU=14
export CUTNA49=1
export CUTMIPP=0
export OUTFILE=nona49_0_120.root
export LOGFILE=nona49_0_120.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

export ELOW=0
export EHIGH=120
export INU=14
export CUTNA49=1
export CUTMIPP=1
export OUTFILE=nomipp_nona49_0_120.root
export LOGFILE=nomipp_nona49_0_120.log

jobsub_submit -G minerva -M --OS=SL6 --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC --role=Analysis -e ELOW -e EHIGH  -e INU  -e CUTNA49 -e CUTMIPP -e OUTFILE -e LOGFILE -e PPFX_DIR -f $OUTDIR/imap_file_list.txt -dIMAP $OUTDIR file://$PPFX_DIR/scripts/imap_job.sh 

