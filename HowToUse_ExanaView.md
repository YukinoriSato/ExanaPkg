
How to use Exana View for diagnosing performance bottlenecks with source code
-------------------------------------------------------------

You can start ExanaView using the following commands.

    % cd Sample.Exana/C2Sim
    % make clean; make
    % Exana -mode traceConsol -- ./bmt.fixed
    % cd <mmdd.pid>
    % traceConsol -j -b ../bmt.fixed > exanaview.json
    % ExanaView
    (MenuBar: File -> Open JSON file)
    (Select current dir -> Select exanaview.json)

Movie: [Youtube Link](https://www.youtube.com/watch?v=Xlbx_XzdAQM)


