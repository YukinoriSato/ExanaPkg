
How to use Exana-C2Sim for profiling cache behaviors 
-------------------------------------------------------------

Examples are in the 'Sample.Exana' directory.  Please move to
'Sample.Exana' directory.  

     % cd Sample.Exana

Then, you can find three directories: 'LoopAndCall', 'Himeno', and
'C2Sim'.  To demonstrate on-line cache profiling to real application
code, we use an example code in C2Sim.

Here, let's move to the directory and start profiling.

     % cd C2Sim
     % export OMP_NUM_THREADS=1
     % Exana -mode C2Sim -- ./bmt.fixed 
     % less <mmdd.pid>/exana.out

exana.out:
```
CacheSim result:         
#cacheSim_eval = 5
  Cache Configuration:
  L1:  32 KB,  8 way
  L2: 256 KB,  8 way
  L3:  20 MB, 20 way
                Cache_miss_(per_memRef) 
  Mem_ref        833333221
  L1_cache_miss  216120586       25.93%
  L2_cache_miss   15143631        1.82% 
  L3_cache_miss   10508882        1.26% 
  Cache_miss_per_ref_in_each_level:   
          L1         25.93%
          L2          7.01%
          L3         69.39%
  Conflict_miss_per_miss_in_each_level: by FA sim   
          L1         92.99%
          L2          0.00%
          L3          2.18%
```

From exana.out, we can find how many cache misses occur and how much
the conflict misses are detected.  Note that we perform a sampling
based simulation.  Then, we can analyze the source of conflicts.

     % cd <mmdd.pid>
     % checkLineconf

```
Target execution binary: /home/ysato/ExanaPkg-git/Sample.Exana/C2Sim/bmt.fixed
missPC 4015e1  L1 cnt=12355108  memObj= malloc(1)@newMat:himenoBMTxpa_omp.c:254 [inline@main:120];96.6% loc=himenoBMTxpa_omp.c:354
  [inter-array] 4015dc cnt=11328332  memObj= malloc(7)@newMat:himenoBMTxpa_omp.c:254 [inline@main:126];32.3% loc=himenoBMTxpa_omp.c:354
  [inter-array] 4015ae cnt=810616  memObj= malloc(7)@newMat:himenoBMTxpa_omp.c:254 [inline@main:126];32.3% loc=himenoBMTxpa_omp.c:354
  [inter-array] 4015a0 cnt=27020  memObj= malloc(6)@newMat:himenoBMTxpa_omp.c:254 [inline@main:125];32.3% loc=himenoBMTxpa_omp.c:354
  [inter-array] 4015c5 cnt=189140  memObj= malloc(7)@newMat:himenoBMTxpa_omp.c:254 [inline@main:126];32.3% loc=himenoBMTxpa_omp.c:354
missPC 4014f5  L1 cnt=12353417  memObj= malloc(5)@newMat:himenoBMTxpa_omp.c:254 [inline@main:124];24.2% loc=himenoBMTxpa_omp.c:354
  [inter-array] 4015a0 cnt=11948117  memObj= malloc(6)@newMat:himenoBMTxpa_omp.c:254 [inline@main:125];32.3% loc=himenoBMTxpa_omp.c:354
  [inter-array] 40158f cnt=405300  memObj= malloc(1)@newMat:himenoBMTxpa_omp.c:254 [inline@main:120];96.6% loc=himenoBMTxpa_omp.c:354
missPC 40150d  L1 cnt=12353417  memObj= malloc(5)@newMat:himenoBMTxpa_omp.c:254 [inline@main:124];24.2% loc=himenoBMTxpa_omp.c:354
```

From the result outputted to stdout, we can analyze reason around the
line-conflicts.  For more details, please read the paper in [Sato,
Euro-Par 2017]



How to use Exana View for diagnosing performance bottlenecks with source code
-------------------------------------------------------------

You can start ExanaView using the following commands.

    % cd Sample.Exana/C2Sim
    % make clean; make
    % Exana -mode C2Sim -- ./bmt.fixed
    % cd <mmdd.pid>
    % traceConsol -j -b ../bmt.fixed > exanaview.json
    % ExanaView
    (MenuBar: File -> Open JSON file)
    (Select current dir -> Select exanaview.json)


[![Exana Demo Video](https://user-images.githubusercontent.com/22608651/226922798-286e15b0-22f2-447a-b121-0e01e1760d6c.png)](https://www.youtube.com/watch?v=Xlbx_XzdAQM)

Note that `Exana -mode C2Sim -- ./bmt.fixed` in the demo video was deprecated.  Please use the mode `Exana -mode C2Sim -- ./bmt.fixed`


