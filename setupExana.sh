#! /bin/sh

EXANA_DIR=/home/$USER/ExanaPkg_R1; export EXANA_DIR
PIN_DIR=/home/$USER/pin-2.14-71313-gcc.4.4.7-linux; export PIN_DIR

export PATH=${PIN_DIR}/:${EXANA_DIR}/:$PATH
export RUBYOPT=-W0
