#!/bin/bash

echo
echo "======== cd to CONDOR_DIR_INPUT ========"
cd $CONDOR_DIR_INPUT
pwd

echo
echo "======== ls ========"
ls

echo
echo "======== UNTARRING... ========"
tar xvfz local_install.tar.gz -C ./ > /dev/null

echo
echo "======== Done untarring. ls ========"
ls

echo
echo "======== SETUP ROOT, BOOST and DK2NU ========"
echo "source setup.sh"
source setup.sh

echo
echo "======== UPDATE g4numi run number to select input ========"
NAME_FILE="g4numiv6_minervame_${BEAMCONFIG}_\${PROCESS}_0006.root"
INPUT_FILE="${IN_DIR}/${BEAMCONFIG}/${NAME_FILE}"
ifdh cp "${INPUT_FILE}" "$CONDOR_DIR_INPUT/"
ls

OUTPUT_FILE="ppfx_${BEAMCONFIG}_run\${PROCESS}.root"

echo "BEAMCONFIG=${BEAMCONFIG}"
echo "PROCESS=$PROCESS"
echo "OUTFILE=$OUTPUT_FILE"

echo
echo "======== EXECUTING ppfx ========"
echo "bin/doReweight_dk2nu_original ${NAME_FILE} ${OUTPUT_FILE} ${INPUT_OPTIONS} ${IDET}"
bin/doReweight_dk2nu_original "${NAME_FILE}" "${OUTPUT_FILE}" "${INPUT_OPTIONS}" "${IDET}"

echo
echo "Moving output to CONDOR_DIR_PPFX: "
echo "=> CONDOR_DIR_PPFX: $CONDOR_DIR_PPFX"
rm g4numi*root
mv ppfx*.root $CONDOR_DIR_PPFX
