#!/bin/bash

setup_for_grid(){
    echo "WARNING: This setup script is depreciated and will likely cause errors during build. Use setup.sh instead."

    # the default mode is NuMI
    export MODE="NUMI"

    # set package to be used
    . "/cvmfs/nova.opensciencegrid.org/externals/setup"
    #ROOT: 
    setup -q e6:nu:prof -f Linux64bit+2.6-2.5 root v5_34_20b
    #BOOST:
    setup -q debug:e5 -f Linux64bit+2.6-2.5 boost v1_55_0
    export BOOSTROOT=${BOOST_DIR}/source/boost_1_55_0
    #DK2NU:
    setup -q debug:e7 -f Linux64bit+2.6-2.5 dk2nu v01_01_03c

    export DK2NU_INC=${DK2NU}/include/dk2nu/tree
    export DK2NU_LIB=${DK2NU}/lib
    
    #GRID
    source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setups.sh
    setup jobsub_client
    setup ifdhc #v2_2_3
    export IFDH_GRIDFTP_EXTRA="-st 10" #set ifdh cp stall timeout to 10 sec
    export IFDH_CP_MAXRETRIES=2

    # gets the full path to the location of setup.sh
    export PPFX_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    echo "setting PPFX_DIR=${PPFX_DIR}"

    export LD_LIBRARY_PATH=$PPFX_DIR/lib:$LD_LIBRARY_PATH
    echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
}
HOST=$(hostname -f)
echo $HOST
if echo "$HOST" | grep 'dune';then
    echo "This is not a dune machine. Try setup_for_dune.sh <MODE>"
else 
    echo "executing for the $HOST"
    setup_for_grid
fi

