# ExanaPkg

------------------------------------------------------------------------------
ExanaPkg: Utility for Exana (EXecution-driven Application aNAlysis tool)

Copyright (C)   2014-2016,   Yukinori Sato
All Rights Reserved. 
------------------------------------------------------------------------------


How to analyze an application is in the file './HowToUse'


## This code is for 64bit linux environment, 
    "ia32_intel64-linux"
     Tested on 
     - CentOS 7.2
     - CentOS 6.4
     - Red Hat Enterprise Linux Server release 5.4
     - SUSE Linux Enterprise Server 11

## How to build
* Download Pin tool kit Intel64 linux

    For information about Pin tool kit, please check:
    	http://pintool.org/ 
    Here, Exana is verified using Pin tool kit rev 71313 for Intel64 linux on CentOS.

* Unpack the pin-2.14-71313-gcc.4.4.7-linux.tar.gz
* git clone https://github.com/YukinoriSato/Exana.git
* cd ExanaPkg
* Set the variables 'EXANA_DIR' and 'PIN_DIR' 
    Edit setupExana.sh(bash), setupExana.csh(csh) to the absolute paths for ExanaPkg and Pin tool kit directory if you install Exana other than '/home/$USER'.
* Install the following software
    Install graphviz, graphviz-devel, graphviz-gd for visualization
    Install php (5.4 or later) for interactive visualization


## Use the command 'Exana'
   % ./Exana -- ./a.out
   Exana is a script written by Ruby which invoke pin and pintool.
   Put this 'Exana' directory at your home directory,
   or modify the path specified by the Exana script (~/Exana-x.x-rYYY)
   This command eases the activation of pin tool set.

   After the execution of the Exana completed, you obtain three output
     files in the <mmdd.pid> directory.  

     1. prof.out file
     2. static.out file 
     3. lcct.dat (or lcctm.dat)

More detail information about how-to-use is in the file './HowToUse'

    You can use it with options;
    % ./Exana -mode {CCT, LCCT, LCCT+M} -cntMode {instCnt, cycleCnt} -loopID {n} -itr {a:b} -apr {c:d} -memtrace {0,1} -pageSize {64B, ..., 64kB} -- ./a.out

    -- Specify Region of Interests (ROI) --
    If you use with the following options, a ROI region (loopID=3, itr=1:2, apr=1) is analyzed based on dynamic instruction counts. 
    % ./Exana -cntMode instCnt -loopID 3 -itr 1:2 -apr 1 -- ./a.out

    If you wish to analyze memory behavior of the ROI region (loopID=3, itr=1:2, apr=1, please specify options like:
    % ./Exana -mode LCCT+M -loopID 3 -itr 1:2 -apr 1 -- ./a.out



    -mode  [default LCCT]
        Specify a mode option, CCT, LCCT or LCCT+M
         CCT: Profiling by Call Context Tree (CCT)
         LCCT: Loop profiling by Loop and Call Context Tree (LCCT)
         LCCT+M: Data dependence profiling based on loop regions

    -cntMode  [default cycleCnt]
       Specify a mode option, instCnt or cycleCnt.
         instCnt:  Execution ratios of each node are represented by 
                   the number of executed instructions.  Using this mode,
                   we can monitor the accumulated # of executed instructions
                   based on the classification of instruction types.
         cycleCnt: Execution ratios of each node are represented by 
                   the times obtained by the CPU cycle counter.

    -loopID  [default -1]
	 Specify a particular loopID if you focus only on it.
	 Please use the loopID obtained by LCCT profiling after you profile once
	 The default value '-1' means that all loops are analyzed.

    -apr  [default -1]
	 Specify a particular appear count or the range of loop appearances such
	 as N:M  (N: start appearCnt, M: end appearCnt).
	 The default value '-1' means that all appearance counts are analyzed.

    -itr  [default -1]
         Specify a particular tripCnt or the range of loop iterations such as
         N:M  (N: start tripCnt, M: end tripCnt)
	 The default value '-1' means that all iterations are analyzed.

    -memtrace  [default 0]
	Turn on memory trace (1) or turn off (0)

    -pageSize  [default 64kB]
	Specify a page size of the working data set analysise [64B, 128B,
	256B, 4kB, 64kB] (default, 64B)
	This option is useful when you analyze the working data set of
	particular loop appearances or iterations.

    -mempat  [default 0]
	Memory access pattern analysis. 0: Turn off.   1: Output binary
	[mempat.dat].  2: Output Text[result.mpat].   (default, off)

    and so on....
    All of options are parsed in getOptions.cpp

## To modify the analysis

The main body of this tool is in 'pintool/Exana.so'.  If you need to
modify or update this, please see the original source code in
https://github.com/YukinoriSato/Exana and copy the outputted Exana.so
file under the source/tools/Exana/obj-intel64 directory into
ExanaPkg/pintool directory.
