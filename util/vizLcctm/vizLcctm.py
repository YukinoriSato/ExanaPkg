#!/usr/bin/python -W ignore::DeprecationWarning
#!/usr/bin/python -W ignore::GtkWarning 

# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
import shutil
import time
#sys.path.append('Shared')

# [READING THE BASE DIRECTORY PATH]
base_dir = os.path.dirname(os.path.abspath(__file__)) or '.'
#print 'Base directory: ', base_dir

# [READING THE CURRENT DIRECTORY PATH]
cwd = os.getcwd()
#print 'current Directory: ' + cwd

# [READING THE PACKAGE DIRECTORY PATH]
package_dir = os.path.join(base_dir, 'Shared')
#print 'package_dir: ', package_dir

# [ADD THE PACKAGE DIRECTORY PATH]
sys.path.insert(0,package_dir)

# [READING THE INCLUDE LIB FILE]
from vizPNGLcctm import vizPNGLcctm
#from vizHTMLLcctm import vizHtmlLcctm
from printHowToUse import printHowToUse

global wsAnaFlag

# [Execute the generateDot program to create the lcctm.png file]
def exegenerateDot(filename):

    print "exeGenerateDot"
    try:
        infile = open(filename, 'rb')
    except:
        print sys.exit("Error:   file cannot be opened")
        
    b=infile.read(6)
    b=infile.read(4)
    b=infile.read(6)
    #print b
    wsAnaFlag=0
    if b=="wsAna\0":
        wsAnaFlag=1
    infile.close()

    # [Remove the png file from the base directory]
    rmpng = os.path.dirname(filename) +  '/lcctm.png'
    if os.path.isfile(rmpng):
       print "Removing old png file..." 
       subprocess.call(["rm","-f",rmpng])
    else:
       print 'No PNG file found to be removed...'
    time.sleep(1)

    depFlag=0;
    if os.path.basename(filename).find("lcctm.dat") > -1:
        depFlag=1;

    #print "hello",filename, os.path.basename(filename), os.path.dirname(filename), depFlag

    # [Execute the generateDot program to create the lcctm.png file] 
    generateDot= base_dir + '/generateDot'
    if depFlag==0:
        if wsAnaFlag==1:
            Exstatus = subprocess.call([generateDot,filename,"-mem -wsPage -wsRead -wsWrite","-topN", "8"])
        else:
            Exstatus = subprocess.call([generateDot,filename,"-mem -ins -flop","-topN", "8"])
    else:
        if wsAnaFlag==1:
            Exstatus = subprocess.call([generateDot,filename,"-mem -wsPage -wsRead -wsWrite -dep","-topN", "8"])
        else:
            Exstatus = subprocess.call([generateDot,filename,"-mem -ins -flop -dep","-topN", "8"])

    # subprocess.call(["ls", "-l", "lcctm.png"])
    #print 'Ex Status' , Exstatus 

    dotFile=os.path.dirname(filename) +  '/lcctm.dot'
    command="dot -Tpng " + dotFile + " -o " + rmpng
    #command="/usr/bin/dot -V"
    #print command
    #subprocess.call(["/usr/bin/dot -Tpng", dotFile, "-o", rmpng], shell=True)
    subprocess.call(command, shell=True)
	 
    return Exstatus


# [Execute the generateDot program to create the lcctm.png file]
def execgenerateDot(filename, exefile):
    print "exeCGenerateDot"
    try:
        infile = open(filename, 'rb')
    except:
        print sys.exit("Error:   file cannot be opened")
        
    b=infile.read(6)
    b=infile.read(4)
    b=infile.read(6)
    print b
    wsAnaFlag=0
    if b=="wsAna\0":
        wsAnaFlag=1
    infile.close()

    # [Remove the png file from the base directory]
    rmpng = base_dir +  '/lcctm.png'
    if os.path.isfile(rmpng):
       print "Removing old png file..." 
       subprocess.call(["rm","-f",rmpng])
    else:
       print 'No PNG file found to be removed...'
    time.sleep(1)

    # [Execute the generateDot program to create the lcctm.png file] 
    generateDot= base_dir + '/generateDot'
    if wsAnaFlag==1:
        Exstatus = subprocess.call([generateDot,filename,"-mem -wsPage -wsRead -wsWrite","-topN", "8", "-exefile",exefile])
    else:
        Exstatus = subprocess.call([generateDot,filename,"-mem -ins -flop","-topN", "8", "-exefile",exefile] )

    #Exstatus = subprocess.call([generateDot,filename,"-mem -ins -flop","-topN", "8", "-exefile",exefile])

    #subprocess.call(["ls", "-l", "lcctm.png"])
    dotFile=base_dir +  '/lcctm.dot'
    subprocess.call(["dot -Tpng", dotFile, "-o", rmpng])

    return Exstatus


def Main(argv):
    # [Declare global variable for accessing the vizLcctm class]
    global GlobalvizLcctm
    
    # [Declare the window container] 
    window = gtk.Window()
    
    #  [Change the current working directory]
    CWD = cwd
    # print "Current working directory", CWD
    
    if len(sys.argv) == 2:
       if cwd != os.path.dirname(os.path.realpath(sys.argv[1])):
          CWD =  os.path.dirname(os.path.realpath(sys.argv[1])) 
    elif len(sys.argv) >= 3 and (sys.argv[1] != "-i" and sys.argv[1] != "-u"):
       if cwd != os.path.dirname(os.path.realpath(sys.argv[1])):
          CWD =  os.path.dirname(os.path.realpath(sys.argv[1]))    
    elif len(sys.argv) >= 3 and (sys.argv[1] == "-i" or sys.argv[1] == "-u"):
       if cwd != os.path.dirname(os.path.realpath(sys.argv[2])):
          CWD =  os.path.dirname(os.path.realpath(sys.argv[2]))    
    else:
       printHowToUse(ScriptName = "vizLcctm.py")
       sys.exit(0)     
    
    print "Current working directory", CWD 
    # [Get default window size with respect to monitor display]
    size = window.get_screen()
    if size.get_active_window() != None:
        m = size.get_monitor_at_window(size.get_active_window())  
        monitor = size.get_monitor_geometry(m)
    else:
        monitor =  size.get_monitor_geometry(0)
        #print "None m", monitor.width, monitor.height
        monitor.width = 1920
        monitor.height = 1200
        #print "None m", monitor.width, monitor.height 
        print "default screen are not found" 
        #sys.exit(0)
    
    # [Create function for the action on destroy of window]
    def ActionDestroy(args,window):
        if sys.argv[1] == "-i" and base_dir != CWD:
           shutil.copy(base_dir+'/lcctm.png',CWD)
           shutil.copy(base_dir+'/lcctm.dot',CWD)
           shutil.copy(base_dir+'/out.map',CWD)   
        window.destroy()
        gtk.main_quit()
        return
    
    # [Calling the ActionDestroy function on window quit option]
    window.connect("destroy", ActionDestroy,window)
 
    # [Set the title for display window]
    window.set_title('Display of Graph')
 
    # [Set the border width and color for display window]
    window.set_border_width(0)
    #window.modify_bg(gtk.STATE_NORMAL, gtk.gdk.Color("GREEN"))


    # [Check the total number of input argument(>= 3)]      
    if len(sys.argv) >= 6:
       printHowToUse(ScriptName = "vizLcctm.py")
       sys.exit(0)
    
    # [Check the total number of input argument(== 1)]      
    if len(sys.argv) == 1:
       printHowToUse(ScriptName = "vizLcctm.py")
       sys.exit(0)

    # [Check the input argument(== &) ]      
    if sys.argv[1] == "&":
       printHowToUse(ScriptName = "vizLcctm.py")
       sys.exit(0)
    # [Check the total number of input argument(> 2) and also the existance of the file]
    elif len(sys.argv) >= 2 and len(sys.argv) <= 6 :
       if (sys.argv[1] != "-i" and sys.argv[1] != "-u"):
          if os.path.isfile(sys.argv[1]) == True:
             FPD =  os.path.dirname(os.path.realpath(sys.argv[1])) + '/' +  os.path.basename(sys.argv[1])
             print 'full path: ', FPD   
             
             if len(sys.argv) == 4 and sys.argv[2] == "-exefile":
                FEPD =  os.path.dirname(os.path.realpath(sys.argv[3])) + '/' +  os.path.basename(sys.argv[3]) 
                Exstatus = execgenerateDot(FPD, FEPD)
                GlobalvizLcctm = vizPNGLcctm(CWD,base_dir,monitor,FPD,Exstatus,exefile=FEPD)
             elif len(sys.argv) == 2: 
                Exstatus = exegenerateDot(FPD)
                GlobalvizLcctm = vizPNGLcctm(CWD,base_dir,monitor,FPD,Exstatus,exefile=None) 
             else:
                printHowToUse(ScriptName = "vizLcctm.py")
                sys.exit(0)            
          else:
             printHowToUse(ScriptName = "vizLcctm.py")
             sys.exit(0)

       elif os.path.isfile(sys.argv[2]) == True and (sys.argv[1] == "-i" or sys.argv[1] == "-u"):
          FPD =  os.path.dirname(os.path.realpath(sys.argv[2])) + '/' +  os.path.basename(sys.argv[2])
          print 'full path: ', FPD 
          
          if len(sys.argv) == 5 and sys.argv[3] == "-exefile":
             FEPD =  os.path.dirname(os.path.realpath(sys.argv[4])) + '/' +  os.path.basename(sys.argv[4])   
             Exstatus =  execgenerateDot(FPD, FEPD)
             if sys.argv[1] == "-i":
                # [Copy the files to the SRC path]
                if base_dir != CWD:
                   shutil.copy(CWD + '/lcctm.png',base_dir)
                   shutil.copy(CWD + '/lcctm.dot',base_dir)
                   shutil.copy(CWD + '/out.map',base_dir)
                   shutil.copy(FPD,base_dir)
                #print 'os.path.basename(sys.argv[2]): ', os.path.basename(sys.argv[2]) 
                
                GlobalvizLcctm = vizHtmlLcctm(CWD,base_dir,monitor, FPD,Exstatus,exefile=FEPD)
             if sys.argv[1] == "-u":
                GlobalvizLcctm = vizPNGLcctm(CWD,base_dir,monitor, FPD,Exstatus,exefile=FEPD)
          elif len(sys.argv) == 3:
             Exstatus = exegenerateDot(FPD)
             if sys.argv[1] == "-i":
                # [Copy the files to the SRC path]
                if base_dir != CWD:
                   shutil.copy(CWD + '/lcctm.png',base_dir)
                   shutil.copy(CWD + '/lcctm.dot',base_dir)
                   shutil.copy(CWD + '/out.map',base_dir)
                   shutil.copy(FPD,base_dir)
             	GlobalvizLcctm = vizHtmlLcctm(CWD,base_dir,monitor, FPD,Exstatus,exefile=None)
             if sys.argv[1] == "-u":
                GlobalvizLcctm = vizPNGLcctm(CWD,base_dir,monitor, FPD,Exstatus,exefile=None)
          else:
             printHowToUse(ScriptName = "vizLcctm.py")
             sys.exit(0)

       else:
          print "Invalid filename or flag options..."
          printHowToUse(ScriptName = "vizLcctm.py") 
          sys.exit(0)    
    else:
       printHowToUse(ScriptName = "vizLcctm.py")
       sys.exit(0)
 
    # [Get the window widget from the get_widget function]
    widget = GlobalvizLcctm.get_widget()

    # [Add the widget data in the window variable] 
    window.add(widget)
   
    # [Perform the show option for window]
    window.show()
    
    # [Set the size of the display according to the monitor dynamically]
    window.set_usize( int(monitor.width/1.5), int(monitor.height/1.5))
        
    # [Calling the main function of gtk to run the main loop until the gtk.main_quit() function is called]
    try:
       gtk.main()
    except KeyboardInterrupt:
       if sys.argv[1] == "-i" and base_dir != CWD:
           shutil.copy(base_dir+'/lcctm.png',CWD)
           shutil.copy(base_dir+'/lcctm.dot',CWD)
           shutil.copy(base_dir+'/out.map',CWD)   
           sys.exit(0)

    return 0

#######################################################
## [Python Script checks the below statement first to make 
## sure which module of code will be executed first]
#######################################################
if __name__ == "__main__":
    Main(sys.argv[1:])

#######################################################
## eof
#######################################################

