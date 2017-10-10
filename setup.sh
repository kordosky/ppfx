#!/bin/bash

setup(){

    # the default mode is NuMI
    export MODE="NUMI"

    . "/nusoft/app/externals/setup"
    #ROOT: 
    setup -q debug:e2 -f Linux64bit+2.6-2.5 root v5_34_05
    #BOOST:
    setup -q debug:e5 -f Linux64bit+2.6-2.5 boost v1_55_0
    export BOOSTROOT=${BOOST_DIR}/source/boost_1_55_0
    #DK2NU:
    setup -q debug:e5 -f Linux64bit+2.6-2.5 dk2nu v01_01_03
    export DK2NU_INC=${DK2NU}/include/dk2nu/tree
    export DK2NU_LIB=${DK2NU}/lib
    
    # setup for jobsub client
    # according to the prescription in Mike Kirby's talk
    # minerva doc-10551, Dec 2014 (same doc can be found for other experiments)
    source /grid/fermiapp/products/common/etc/setups.sh
    setup jobsub_client
    setup ifdhc
    #This is for MINERvA. Change accordingly. 
    export IFDH_BASE_URI="http://samweb-minerva.fnal.gov:20004/sam/minerva/api"
    
    # bash magic pulled off of stack exchange
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
    setup
fi

