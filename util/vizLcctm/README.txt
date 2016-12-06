###########################################################################################
###                            Project on Lcctm                                        ####
###                     Version: v2.0     Date: 15/07/2014                             ####                               
###########################################################################################

----------------------------------
Command Line Aguments(generateDot)
----------------------------------

Example: ./generateDot lcctm.out -topN 5

   Usage: generateDot filename [Flags][Options]

   FLAGS:
	 -mem 	 memory footprint for each node
	 -ins 	 accumulated instructions executed in the node
	 -flop 	 accumulated floating point instructions executed in the node
	 -cycle  total clock cycles for the node
	 -cpi 	 Clock cycles Per Instruction for each node
	 -bpi 	 Bytes Per Instruction for each node
	 -bpc 	 Bytes Per Clock cycle for each node
	 -ibf 	 Bytes Per flop for each node
	 -dep 	 enable data dependence information
   
   OPTIONS:
	 -thr t Threshold  set the threshold value
	      t is percentage within a range from 0 to 100

	 -topN n Top Node   set the top number of nodes to display
	       n is integer and n>0, default value for topN: 8

	 -exefile filename
	       Specify the filename of the profiled binary executable
	       The filename will help to find inlined functions

	 -nodeID i
	       i is ID number of nodes, which can be found in dot file.
	       If you specify NodeID, the node is displayed at the top as a root node


-------------------------------------------------
Command Line Arguments(vizLcctm) ### [PNG Display]
-------------------------------------------------

Example:  ./vizLcctm.py lcctm.out
  
   Usage: ./vizLcctm.py [Python Flags] FileName [Options]

   [Python Flags]: 
	-u	 	Will show the PNG uninteractive display
	-i		Will show the HTML interactive display
   OPTIONS: 
	-exefile [filename]
 
------------------
Enviornment Setup
------------------

Recommended Softwares:
       - PHP 5.4 or later (Optional, required if you use -i mode) 
       - gcc 4.4.7
       - graphviz (graphviz, graphviz-devel, graphviz-gd) 2.26.0 or later
       - python 2.6.6

Recommended for using HTML flag:
 #  Please run the below command to start the php on the source path-
       % php -S localhost:8000 -t /home/user/Exana/vizlcctm/
    [Please use this command only once during running the program from different remote terminals]
 #  Please use -X option for ssh login to obtain the dynamic display.

-----------------------------------------------
Execution and DOT file generation(generatedDot)   
-----------------------------------------------

+ Running the generateDOT program will create the DOT file and also the PNG file for the generated DOT file.
+ The name for the generated DOT file is lcctm.dot and the png file is lcctm.png.
+ Opening the lcctm.png file one could see the graphical display of the lcctm.dot file.   

-------------------------------------------
Execution and DOT file generation(vizLcctm)   
-------------------------------------------

+ Running the vizLcctm.py script with the given filename, one could generate the DOT and PNG file in the same source location.
+ Running the vizLcctm.py script, the user can directly see the graphical display of lcctm.dot file through the lcctm.png file in a popup window.
+ This window is for performing the dyanmic operation on the DOT file generation.
+ User is allowerd to modify the threshold and topN value using the combobox option.
+ clicking the submit button in the graphical window will demonstrate the change in display for the modified values.
+ Running vizLcctm.py with the -u flag, will demonstate the graphical window from the lcctm.png file. User can see the zoom in or zoom out image from the +/- button.
+ Running vizLcctm.py with the -i flag, will demonstate the graphical window from the main.php file which actual obtains the image file from lcctm.png. In this window users can obtain the interactive display by clicking any node. Then, you can select a root node on demand by clicking the node. This will create the new root node and also will show the display equal to the fixed threshold value. If you check the checkbox on the top of window and push Submit button, the checked statistics are displayed arround the node as the interactive display. 
+ Running vizLcctm.py with the -i flag, a new checkbox option is EqTHR, which is used for making the equivalent threshold at the time of display. if user select EqTHR checkbox and select any execution node, the display will be rendered for the equivalent threshold. Now if he/she uncheck the EqTHR the display will be shown equivalent to the topN=8. 
+ If user would like to go back to initial state, push the Initial button.
+ All the checkbox are workable same as the input flags (i.e. -mem, -ins, -cycle etc.). So user can select the checkbox and an interactive display will be obtained upon the checked options. And the flag value will be appeared around the nodes.




