#!/bin/bash

export run_exe="/minerva/app/users/laliaga/NEW_RW_OUTSIDE_MINERVA2/run_fluxpack.sh" 
#use this executable for jobsub_client:
#export run_exe="file:///minerva/app/users/laliaga/NEW_RW_OUTSIDE_MINERVA2/run_fluxpack.sh"

export GRID="--OS=SL6 -g --opportunistic --use_gftp"
export NJOBS="500"

export OUTDIR="/minerva/data/users/laliaga/FLUX_PACK/GRID"
mkdir -p $OUTDIR
chmod 01775 $OUTDIR

#BEAMS=( "le010z185i" "le010z-185i" "le010z-185i" )
#MODS=( "mediumBYDET" "noCorr" "mediumBYDET" )

BEAMS=( "le010z185i" "le010z-185i" )
MODS=( "uhighBYDET" "uhighBYDET" )

len=${#BEAMS[*]}

for (( i=0; i<len; i++ )) 
do
  export BEAMCONFIG=${BEAMS[${i}]}
  export MOD=${MODS[${i}]}
  
  export NTUPLEDIR="${OUTDIR}/${BEAMCONFIG}/${MOD}"
  export LOGDIR="${NTUPLEDIR}/log"

  export LOGFILE="${LOGDIR}/fluxpack_${BEAMCONFIG}_${MOD}_\${PROCESS}.log"
  
  mkdir -p $NTUPLEDIR
  chmod 01775 $NTUPLEDIR
  mkdir -p $LOGDIR
  chmod 01775 $LOGDIR
  
  jobsub $GRID -N $NJOBS -dLEO $NTUPLEDIR -L $LOGFILE ${run_exe} $MOD $BEAMCONFIG
#use next line for jobsub_client:
#jobsub_submit.py -M --resource-provides=usage_model=DEDICATED --OS=SL6 --group=minerva -dLEO $NTUPLEDIR -L $LOGFILE ${run_exe} $MOD $BEAMCONFIG 

done 



