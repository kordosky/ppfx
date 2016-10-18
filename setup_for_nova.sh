#!/bin/bash

setup(){

    local TOP=${PWD}

    export M32=-m64
    
    export BOOSTROOT=/nusoft/app/externals/boost/v1_55_0/source/boost_1_55_0
    export PPFX_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
    echo "setting PPFX_DIR=${PPFX_DIR}"
    export LD_LIBRARY_PATH=$PPFX_DIR/lib:$LD_LIBRARY_PATH
    echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
}
setup
