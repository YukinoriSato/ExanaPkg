# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
#import webkit

# [Showing the image display in the scrolled window]
def printHowToUse(ScriptName):
    
      print "Usage: ./"+ ScriptName + " [Python Flags] [FileName] [options]";
      print "[Python Flags]: ";
      print "	-u		Will show the PNG uninteractive display";
      print "	-i	 	Will show the HTML interactive display";
      print "OPTIONS: ";
      print "	-exefile [filename]";  
      return;
