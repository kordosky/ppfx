#!/bin/bash

# source /grid/fermiapp/minerva/software_releases/v10r7p3/setup.sh

setup(){
    . "/grid/fermiapp/products/minerva/etc/setups.sh"
    local TOP=${PWD}

    setup -q debug -f Linux+2.6-2.5 root v5_30_00

    if [ -e "/grid/fermiapp/minerva/condor/setup.minerva.condor.sh" ]; then
	source /grid/fermiapp/minerva/condor/setup.minerva.condor.sh
    fi

    source /grid/fermiapp/products/minerva/etc/setups.sh
    source /grid/fermiapp/products/common/etc/setups.sh

#comments out the next lines for jobsub_clients:
    setup cpn -z /grid/fermiapp/products/common/db
    setup jobsub_tools v1_2q -z /grid/fermiapp/products/common/db
#uncomment out the enxt lines to use jobsub_client:setup jobsub_tools
    #setup jobsub_tools
    #setup jobsub_client -z /grid/fermiapp/products/common/db
    
    export BOOST=/nusoft/app/externals/boost/v1_55_0/source/boost_1_55_0
    
    
}
setup
