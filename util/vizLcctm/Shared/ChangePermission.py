# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys

# [Changing the user access permission]
def ChangePermission(self):
    # [Execute the generateDot program to create the lcctm.png file] 
    #subprocess.call(["ls", "-l", "lcctm.png"])
    #subprocess.call(["chown", "faisal:daemon", "lcctm.dot"])
    #subprocess.call(["chown", "faisal:daemon", "out.map"])
    #subprocess.call(["chown", "faisal:daemon", "lcctm.png"])
    subprocess.call(["chmod", "775", self._CWD+"/lcctm.dot"])
    subprocess.call(["chmod", "775", self._CWD+"/out.map"])
    subprocess.call(["chmod", "775", self._CWD+"/lcctm.png"])
    return
