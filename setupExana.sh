#! /bin/sh

EXANA_DIR=~/ExanaPkg; export EXANA_DIR
PIN_DIR=~/pin-2.14-71313-gcc.4.4.7-linux; export PIN_DIR
export LD_LIBRARY_PATH=${PIN_DIR}/:${EXANA_DIR}/util/ExanaView/ext-lib:$LD_LIBRARY_PATH
export PATH=${PIN_DIR}/:${EXANA_DIR}/:$PATH
export RUBYOPT=-W0
