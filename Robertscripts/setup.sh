#!/bin/bash

setup_for_nova(){
    echo "Sourcing NOvA setup script"
    source /cvmfs/nova.opensciencegrid.org/novasoft/slf7/novasoft/setup/setup_nova.sh "$@"
    setup fife_utils
    
    # setting up for the grid
    export IFDH_GRIDFTP_EXTRA="-st 10" #set ifdh cp stall timeout to 10 sec
    export IFDH_CP_MAXRETRIES=2
    
    # bash magic pulled off of stack exchange
    # gets the full path to the location of setup.sh
    export PPFX_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    echo "setting PPFX_DIR=${PPFX_DIR}"
    export LD_LIBRARY_PATH=$PPFX_DIR/lib:$LD_LIBRARY_PATH
    if [ -z "${_CONDOR_SCRATCH_DIR}" ]; then
      kx509; voms-proxy-init -rfc -valid 196:00 -noregen -voms nova:/nova/Role=Analysis
    fi
}

setup_for_dune(){
    echo "Sourcing DUNE setup script"
    source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
    setup ifdhc
    setup dk2nudata v01_10_00d -q e20:prof
    setup dk2nugenie v01_10_00d -q e20:prof
    export M32=-m64
    setup fftw v3_3_4 -q prof
    setup boost v1_75_0 -q e20:prof

    # setup for jobsub client
    # according to the prescription in Mike Kirby's talk
    # minerva doc-10551, Dec 2014
    # Setup grid submit tools if not on a grid node
    if [ -z "${_CONDOR_SCRATCH_DIR}" ]; then
        echo "_CONDOR_SCRATCH_DIR is not set... so I'm assuming we're not running on a grid node.... Setting up jobsub  tools."
        setup jobsub_client
	/cvmfs/dune.opensciencegrid.org/products/dune/duneutil/v09_42_00/bin/setup_fnal_security -f -b
    fi

    # bash magic pulled off of stack exchange
    # gets the full path to the location of setup.sh
    export PPFX_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    echo "setting PPFX_DIR=${PPFX_DIR}"
    export LD_LIBRARY_PATH=$PPFX_DIR/lib:$LD_LIBRARY_PATH
}

setup_for_other(){
    echo "Sourcing nusoft setup script"
    . "/nusoft/app/externals/setup"

    setup -q debug:e2 -f Linux64bit+2.6-2.5 root v5_34_05
    setup -q debug:e5 -f Linux64bit+2.6-2.5 boost v1_55_0
    export BOOSTROOT=${BOOST_DIR}/source/boost_1_55_0
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
}


HOST=$(hostname -f)
echo $HOST
if echo "$HOST" | grep 'nova'; then
    # the default mode is NuMI
    export MODE="NUMI"
    echo "Setting up for NOvA in MODE $MODE"
    setup_for_nova
elif echo "$HOST" | grep 'dune'; then
    export MODE=$1
    if [ "$MODE" = "REF" ] || [ "$MODE" = "OPT" ];then
        echo "Setting up for DUNE in MODE $MODE"
        setup_for_dune
    else
        echo "This is a dune machine. You must add 1 argument. REF for reference and OPT for optimized"
    fi
else 
    # the default mode is NuMI
    export MODE="NUMI"
    echo "HOST=$HOST"
    echo "WARNING: This setup script might be outdated if you are not on nova or dune machines."
    echo "Setting up for $HOST in MODE $MODE"
    #setup_for_other
    setup_for_nova
fi

