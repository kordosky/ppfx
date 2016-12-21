#!/bin/bash

setup(){
    . "/grid/fermiapp/products/minerva/etc/setups.sh"
    local TOP=${PWD}

    setup -q debug -f Linux+2.6-2.5 root v5_30_00
    export M32=-m32
    export MODE="NUMI"
    # setup for jobsub client
    # according to the prescription in Mike Kirby's talk
    # minerva doc-10551, Dec 2014
    source /grid/fermiapp/products/common/etc/setups.sh
    setup jobsub_client
    setup ifdhc
    export IFDH_BASE_URI="http://samweb-minerva.fnal.gov:20004/sam/minerva/api"
    export MODE="NUMI"

    export BOOSTROOT=/nusoft/app/externals/boost/v1_55_0/source/boost_1_55_0
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

