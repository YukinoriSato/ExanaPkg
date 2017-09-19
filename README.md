# ExanaPkg

------------------------------------------------------------------------

ExanaPkg: Utility for Exana (EXecution-driven Application aNAlysis tool)

Copyright (C)   2014-2017,   Yukinori Sato
All Rights Reserved. 

------------------------------------------------------------------------

## Update:

* C2Sim (Advanced cache simulation with line-conflict detection, see [EuroPar 2017])


# Usage
How to analyze an application is in the file './HowToUse'


This code is for 64bit linux environment, 
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
* git clone https://github.com/YukinoriSato/ExanaPkg.git
* cd ExanaPkg
* Set the variables 'EXANA_DIR' and 'PIN_DIR' 

    Edit setupExana.sh(bash), setupExana.csh(csh) to the absolute paths for ExanaPkg and Pin tool kit directory if you install Exana other than '/home/$USER'.

* Install the following software

    Install graphviz, graphviz-devel, graphviz-gd for visualization
    Install php (5.4 or later) for interactive visualization


## Use the command 'Exana'

% source ExanaPkg/setupExana.sh
% Exana -mode C2Sim -- ./a.out

    This is an example for activating a cache simulator with line-conflict detector within Exana.

% Exana [options] -- ./a.out
 
    Exana is a script written by Python which invoke pin and pintool.
    This command eases the activation of pin tool set.

After the execution of the Exana completed, you obtain several output files in the <mmdd.pid> directory.  In "exana.out", a summary for the profiling is described.

More detail information and demos are in the file './HowToUse'

    You can use it with options (all of options can be found at "getOptions.cpp" in Exana source code;

    % ./Exana -mode {C2Sim, CCT, LCCT, LCCT+M} -- ./a.out


    -mode  [default LCCT]
        Specify a mode option, C2Sim, CCT, LCCT or LCCT+M
         C2Sim: A cache simulator with line-conflict detector
         CCT: Profiling by Call Context Tree (CCT)
         LCCT: Loop profiling by Loop and Call Context Tree (LCCT)
         LCCT+M: Data dependence profiling based on loop regions

    and so on....
    All of options are parsed in getOptions.cpp

## To modify the analysis

The main body of this tool is in 'pintool/Exana.so'.  If you need to
modify or update this, please see the original source code in
https://github.com/YukinoriSato/Exana and copy the outputted Exana.so
file under the source/tools/Exana/obj-intel64 directory into
ExanaPkg/pintool directory.


## Citation and Details for Exana

The canonical publication to cite Exana and ExanaPkg is:

* Yukinori Sato and Toshio Endo. “An Accurate Simulator of Cache-line Conflicts to Exploit the Underlying Cache Performance” 23rd International Europian Conference on Parallel and Distributed Computing (Euro-Par 2017), August 2017. (DOI: 10.1007/978-3-319-64203-1_9)

* Yukinori Sato, Shimpei Sato, and Toshio Endo. Exana: An Execution-driven Application Analysis Tool for Assisting Productive Performance Tuning. Proceedings of the 2nd Workshop on Software Engineering for Parallel Systems (SEPS 2015), held in conjunction with SPLASH2015, Pages 1-10, October 2015. (DOI: 10.1145/2837476.2837477)


Please read the following papers if you are interested in detail techniques behind Exana:


* Yukinori Sato, Yasushi Inoguchi, Tadao Nakamura. Identifying Program Loop Nesting Structures during Execution of Machine Code. IEICE Transaction on Information and Systems, Vol.E97-D, No.9, pp.2371-2385, Sep. 2014. (DOI:10.1587/transinf.2013EDP7455)

* Yukinori Sato, Yasushi Inoguchi and Tadao Nakamura. Whole Program Data Dependence Profiling to Unveil Parallel Regions in the Dynamic Execution. In Proceedings of 2012 IEEE International Symposium on Workload Characterization (IISWC 2012). (DOI:10.1109/IISWC.2012.6402902) 

