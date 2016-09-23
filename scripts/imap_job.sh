#! /bin/bash

echo "PPFX_DIR= $PPFX_DIR"
cd $PPFX_DIR
source ./setup.sh
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"

echo "_CONDOR_SCRATCH_DIR=$_CONDOR_SCRATCH_DIR"
cd $_CONDOR_SCRATCH_DIR

echo "ELOW= $ELOW"
echo "EHIGH= $EHIGH"
echo "OUTFILE= $OUTFILE"
echo "INFILES= $INFILES"
echo "INU= $INU"
echo "CUTMIPP= $CUTMIPP"
echo "CUTNA49= $CUTNA49"

echo "Now preparing to copy files"
echo "ls of the local working directory:"
echo ls -l ./

echo "cat of the input file list:"
cat $CONDOR_DIR_INPUT/imap_file_list.txt

#echo "List of INFILES"
#ls $INFILES

#mkdir inputs
#ifdh cp -D $INFILES $_CONDOR_SCRATCH_DIR/inputs
echo "Making an inputs subdirectory and copying into it"
mkdir inputs

ifdh cp -f $CONDOR_DIR_INPUT/imap_file_list.txt

#for f in `cat imap_file_list.txt`; do
#    echo "copying $f"
#    ifdh cp $f ./inputs
#done


echo "List of the inputs subdirectory=$_CONDOR_SCRATCH_DIR/inputs"
echo "The list follows: "
echo ls -l $_CONDOR_SCRATCH_DIR/inputs

$PPFX_DIR/bin/CreateIMapHists $OUTFILE "inputs/*.root" $ELOW $EHIGH $INU $CUTNA49 $CUTMIPP


echo "ls -lh on working directory:"
ls -lh ./
echo "Copying files to $CONDOR_DIR_IMAP"
cp $OUTFILE $CONDOR_DIR_IMAP

echo "Contents of CONDOR_DIR_IMAP=$CONDOR_DIR_IMAP are"
ls $CONDOR_DIR_IMAP

