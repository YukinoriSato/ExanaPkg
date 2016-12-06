#! /bin/tcsh

setenv EXANA_DIR /home/$USER/ExanaPkg
setenv PIN_DIR /home/$USER/pin-2.14-71313-gcc.4.4.7-linux

setenv LD_LIBRARY_PATH ${PIN_DIR}/:${EXANA_DIR}/util/ExanaView/ext-lib:$LD_LIBRARY_PATH
setenv PATH ${PIN_DIR}:${EXANA_DIR}:${PATH}
