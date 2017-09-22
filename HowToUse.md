
How to use Exana for analyzing an execution of applications
-------------------------------------------------------------

Examples are in the 'Sample.Exana' directory.  Please move to
'Sample.Exana' directory.  

     % cd Sample.Exana

Then, you can find three directories: 'LoopAndCall', 'Himeno', and 'C2Sim'.  Using these sample code, you can have experiences of application profiling.  If you are interested only in C2Sim's cache profiling, please check [How To Use C2Sim](HowToUse_C2Sim.md)

Here, let's start with a simple example that visualize nests of loops and calls.

     % cd LoopAndCall
     % ./SampleMain
     % Exana -- ./SampleMain
     % less <mmdd.pid>/exana.out
     % vizLcctm <mmdd.pid>/lcct.dat.0 &

     Note: Please set up your environment with 'setupExana' script.
     	   Further information is in the INSTALL file of ExanaPkg.

After an analysis, Exana generates the <mmdd.pid> directory which contains analysis result files.  In that directory, there are three files, exana.out,
static.out, lcct.dat (or lcctm.dat).

* exana.out represents dynamic profiling results.
* static.out represents static analysis results.
* lcct.dat (or lcctm.dat) is a binary file that represent loop call
  context tree respresentation of the executed binary.  When you use
  -mode LCCT+M, then lcctm.dat is generated.  The LCCT+M contains
  loop-level data dependence information.

Also, You can visualize lcct.dat (or lcctm.dat) using the following
command, vizLcctm.  Then, you can visualize the LCCT graph with nodes
that are greater than threshold.  Here, the function 'main' becomes a
root node of the tree.  Selecting the checkbox at the top of window,
we can observe statistics obtained during the analysis interactively.
Also, you can specify thresholds for visualing a particular region by
putting the value and push the submit bottom.  TopN means that we try
to visualize the top N nodes and their parents.

Based on LCCT graph, you can observe dynamic loop structures in the
actual execution, and locations of them in the actual source code.  To
obtain information of source code locations, you need to specify debug
option '-g' when you compile your executable code.

If you would like to analyze data dependences among loops and calls,
then please specify an option for LCCT+M analysis:

     % Exana -mode LCCT+M -- ./SampleMain
     % vizLcctm <mmdd.pid>/lcctm.dat.0 &

If you would like to obtain actual memory traces during the execution,
then please specify an option for memtrace, and the result is
outputted to "memTrace.out.X" file.  

     % Exana -memtrace 1 -- ./SampleMain
     % less  <mmdd.pid>/memTrace.out.1
         Here, the memory trace result is written to automatically 
	 roteted files starting from 1.  

      The basic format of the memTrace (-memtrace 1) is 
          [R]ead/[W]rite_size@MemInstAddress, memoryAddress

      If you specify -memtrace 2 option; 
      	  cycle count, rtnName/loopID + basic format 


Now, you can try Himeno Benchmark to evaluate feasibility for parallel
programs.

      % cd Himeno
      % export OMP_NUM_THREADS=1
      % ./bmt S
      % Exana -- ./bmt S
      % vizLcctm mmdd.pid/lcctm.dat.0 &
      % Exana -mode LCCT+M -- ./bmt S
      % vizLcctm mmdd.pid/lcctm.dat.0 &

If you would like to execute a multithread application, currently we
only support analyzing LCCT for the primary thread, which is running
at the begining of the main function before other threads are forked.

      % export OMP_NUM_THREADS=4
      % ./bmt S
      % Exana -- ./bmt S
      % vizLcctm mmdd.pid/lcctm.dat.3 &

If you would like to execute an MPI application, please specify mpirun
before Exana command.  Then, you can obtain LCCTs for each process.

      % export OMP_NUM_THREADS=1
      % mpirun -np 4 ./bmt S
      % mpirun -np 4 Exana -- ./bmt S
      % vizLcctm mmdd.pid/lcctm.dat.0 &


Next, we will show how to use C2Sim's cache profiling in another file [How To Use C2Sim](HowToUse_C2Sim.md)  Please open the file and check how to use it.
