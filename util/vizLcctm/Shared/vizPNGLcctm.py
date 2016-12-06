# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
#sys.path.append('Shared')
from Display import DisplayImage
from Display import DisplayScaledImage 
from GenerateDot import GenerateDot
from printHowToUse import printHowToUse

#import Submit


    

# [Declare the class vizPNGLcctm]
class vizPNGLcctm:
    wsAnaFlag=0
    # [Define the DisplayModes for combobox]
    DisplayModes = ['TopN','Threshold']
  
    # [Declare the constructor for vizLcctm_unscaled class]
    def __init__(self,cwd,base_dir,monitor,filename,exstatus,exefile=None):

            try:
                infile = open(filename, 'rb')
            except:
                print sys.exit("Error:   file cannot be opened")
        
            b=infile.read(6)
            b=infile.read(4)
            b=infile.read(6)
            wsAnaFlag=0
            if b=="wsAna\0":
                wsAnaFlag=1
            infile.close()
            #print "wsAnaFlag=%d" % wsAnaFlag
            # [Define the constructor variables]
	    self._wsAnaFlag = wsAnaFlag
	    self._widget = None	
	    self._status = None
	    self._image = None
	    self._combo = None	
	    self._thr = ""
            self._hbox = None 	
            self._topN = "" 
            self._mode = "TopN"
  	    self._filename = filename
            if wsAnaFlag==1:
                #print "hoge"
                self._mem = "1"  
                self._ins = "0"
                self._cycle = "0"
                self._cpi = "0"  
                self._bpi = "0"
                self._bpc = "0"
                self._ibf = "0"  
                self._flop = "0"
                self._dependency = "0"
                self._eqTHR = "0"
                self._memRW = "0"  
                self._wsPage = "1"  
                self._wsRead = "1"  
                self._wsWrite = "1"  
            else:
                self._mem = "1"  
                self._ins = "1"
                self._cycle = "0"
                self._cpi = "0"  
                self._bpi = "0"
                self._bpc = "0"
                self._ibf = "0"  
                self._flop = "1"
                self._dependency = "0"
                self._eqTHR = "0"
                self._memRW = "0"  
                self._wsPage = "0"  
                self._wsRead = "0"  
                self._wsWrite = "0"  
            #print self._wsPage
            print self._filename.replace(cwd,"")
            if (self._filename.replace(cwd,"") == "/lcct.dat"):
                self._dep = "0"
            else:
                self._dep = "1"
            #print self._dep

            self._initial = "0"
            self._hbox_Scaled = gtk.HBox()
            self._hbox_UnScaled = gtk.HBox()
            self._base_DIR = base_dir
            self._CWD = cwd
            self._SCR = "UNINTERACTIVE"
            self._monitor = monitor
            self._exefile = exefile
            self._exstatus = exstatus #'0' for cycleCnt mode & '1' for instCnt mode
            
            # print 'filename',self._filename 
            #print 'exstatus',  self._exstatus       
            #print "execfile name",self._exefile    
	  #  print 'Base directory: ', self._base_DIR
            #print 'Current directory: ', self._CWD  
            
            # [Define the vertical box for the display window and add the show option for display]
       	    vbox = gtk.VBox()
            vbox.show()
          
            # [Add the vbox container in widget]
            self._widget = vbox     
        	
            # [Define the horizontal box for the display window and add the show option for display] 
            self._hbox = gtk.HBox()
            self._hbox.show()
 
            # [Add the hbox container in the vertical box]             
            vbox.pack_start(self._hbox,0,0)
        
            # [Define the gtk scrolledwindow method for displaying the scrolled window]
	    self.Scrolled_Window = gtk.ScrolledWindow() 	    
	    self.Scrolled_Window.show()
          
            # [Read the genenrated svg file into the pixbuf] 
	    pixbuf = gtk.gdk.pixbuf_new_from_file(  self._CWD + "/lcctm.png") 
          
	    # [Define the image container to hold the image for display]
  	    self._image = gtk.Image()
	    ###self._image.set_from_pixbuf(scaled_buf) 
	    self._image.set_from_pixbuf(pixbuf)
              
            # [Add the image container with the scrolled_window variable to show the image]	   
	    self.Scrolled_Window.add_with_viewport(self._image)
            self._image.show()	
            
            # [Vertical box container ends with the scrolled_window variable] 
            vbox.pack_end(self.Scrolled_Window,True, True, 8) 

            # [Create frame for create a separate part for dynamic inputs and add the frame with the vbox]               
	    frame = gtk.Frame("Change of Threshold & TopN")
            frame.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("RED"))
            frame.show()
            colorbox = gtk.EventBox()
            colorbox.add(frame) 
            colorbox.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("GREEN"))
            colorbox.show()
            vbox.pack_start(colorbox,0,0)

            # [Create a new horizontal box for dynamic input options and add it into the frame container]
            hbox = gtk.HBox()
            colorbox = gtk.EventBox()
            colorbox.add(hbox)
            hbox.show()
            colorbox.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("GREEN"))
            frame.add(colorbox)
            colorbox.show()	
            
            # [Create button to unscaled the display]
            self._hbox_UnScaled.show() 
            button3 = gtk.Button("+")
            # [Make a gdk.color for BLUE]
            map = button3.get_colormap()
            color = map.alloc_color("orange")
            # [copy the current style and replance the background]
            style = button3.get_style().copy()
            style.bg[gtk.STATE_NORMAL] = color
            button3.set_style(style)
            button3.show()
            button3.connect("clicked", self.UnScaled)
            self._hbox_UnScaled.pack_end(button3)
            
            # [Create button to scaled the display]
            self._hbox_Scaled.show() 
            button4 = gtk.Button("-")
            # [Make a gdk.color for BLUE]
            map = button4.get_colormap()
            color = map.alloc_color("Yellow")
            # [copy the current style and replance the background]
            style = button4.get_style().copy()
            style.bg[gtk.STATE_NORMAL] = color
            button4.set_style(style) 
            button4.show()
            button4.connect("clicked", self.Scaled)
            self._hbox_Scaled.pack_end(button4)   

            # [Set the configuration to display the UnScaled button and hide the Scaled button] 
            self._hbox_UnScaled.hide()
            self._hbox_Scaled.show()     
            
            # [Create button for Submit display to show the changes in display for the dynamic inputs]  
            button2 = gtk.Button("Submit")
            # [Make a gdk.color for BLUE]
            map = button2.get_colormap()
            color = map.alloc_color("White")
            # [copy the current style and replance the background]
            style = button2.get_style().copy()
            style.bg[gtk.STATE_NORMAL] = color
            button2.set_style(style) 
            button2.show()
            button2.connect("clicked", self.SubmitButton)
         
            # [Create button for Initial display to back to default display]  
            button5 = gtk.Button("Initial")
            # [Make a gdk.color for BLUE]
            map = button5.get_colormap()
            color = map.alloc_color("RED")
            # [copy the current style and replance the background]
            style = button5.get_style().copy()
            style.bg[gtk.STATE_NORMAL] = color
            button5.set_style(style) 
            button5.show()
            button5.connect("clicked", self.InitialButton)
 	            
            # [Getting the dynamic input for topN box]
	    def topN_changed(widget):
                self._topN = widget.get_text()
                self._thr = ""
                return
	
            # [Getting the dynamic input for the Threshold box]
	    def threshold_changed(widget):
                self._thr = widget.get_text()
                self._topN = ""
                return
            
            # [Set the horizontal Box with the entry textArea to display the TopN option and ask the user for the dynamic input of topN value]  
            hbox_TopN = gtk.HBox()
            hbox_TopN.show()  
            label = gtk.Label()
    	    label.set_markup(" TopN(#): ")
    	    label.show()
    	    hbox_TopN.pack_start(label,0,0)
            self._entry_TopN = gtk.Entry()
            self._entry_TopN.set_text("8")
            self._entry_TopN.show()
            self._entry_TopN.set_width_chars(15)
            self._entry_TopN.connect("changed", topN_changed)
            hbox_TopN.pack_end(self._entry_TopN)
     
            # [Set the horizontal Box with the entry textArea to display the Threshold option and ask the user for the dynamic input of threshold value]
            hbox_Thr = gtk.HBox()
            hbox_Thr.show()
            label = gtk.Label()
	    label.set_markup(" Threshold(%): ")
            label.show()
            hbox_Thr.pack_start(label,0,0) 
            self._entry_Thr = gtk.Entry()
            self._entry_Thr.show()
            self._entry_Thr.set_width_chars(15)    
            self._entry_Thr.connect("changed", threshold_changed)
            hbox_Thr.pack_end(self._entry_Thr)
            
            # [Set the horizontal Box with the hidden level]
            hidden0 = gtk.HBox()
            hidden0.show()    
            label = gtk.Label()
	    label.set_markup("        ")
            label.show()
            hidden0.pack_start(label)             

            # [Add the checkbox for flags]
            
            self.check_box0 = gtk.CheckButton("mem")
	 
	    if self._exstatus:
                 self.check_box0.set_active(True)
            else:
              self.check_box0.set_active(False)
              self.check_box0.set_sensitive(False)  
            
            self.check_box0.connect("toggled",self.mem_checkbox, self.check_box0)
            self.check_box0.show()    
            
            self.check_box1 = gtk.CheckButton("ins")

            if wsAnaFlag:
                if self._exstatus:
                    self.check_box1.set_active(False)
                else:
                    self.check_box1.set_active(False) 
                    self.check_box1.set_sensitive(False)  		
            else:
                if self._exstatus:
                    self.check_box1.set_active(True)
                else:
                    self.check_box1.set_active(False) 
                    self.check_box1.set_sensitive(False)  		
            
            self.check_box1.connect("toggled",self.ins_checkbox, self.check_box1)
            self.check_box1.show()             

            self.check_box2 = gtk.CheckButton("cycle")
            self.check_box2.set_active(False)
            if not(self._exstatus):
               self.check_box2.set_sensitive(False) 
            self.check_box2.connect("toggled",self.cycle_checkbox, self.check_box2)
            self.check_box2.show()    
            
            self.check_box3 = gtk.CheckButton("CPI")
            self.check_box3.set_active(False)
            if not(self._exstatus):
               self.check_box3.set_sensitive(False) 
            self.check_box3.connect("toggled",self.cpi_checkbox, self.check_box3)
            self.check_box3.show()    

            self.check_box4 = gtk.CheckButton("B/ins")
            self.check_box4.set_active(False)
            if not(self._exstatus):
               self.check_box4.set_sensitive(False)  
            self.check_box4.connect("toggled",self.bpi_checkbox, self.check_box4)
            self.check_box4.show()

            self.check_box5 = gtk.CheckButton("B/cycle")
            self.check_box5.set_active(False)
            if not(self._exstatus):
               self.check_box5.set_sensitive(False) 
            self.check_box5.connect("toggled",self.bpc_checkbox, self.check_box5)
            self.check_box5.show()    
            
            self.check_box6 = gtk.CheckButton("iBF")
            self.check_box6.set_active(False)
            if not(self._exstatus):
               self.check_box6.set_sensitive(False) 
            self.check_box6.connect("toggled",self.ibf_checkbox, self.check_box6)
            self.check_box6.show()    

            self.check_box7 = gtk.CheckButton("flop")

            if wsAnaFlag:
                if self._exstatus:
                    self.check_box7.set_active(False)
                else:
                    self.check_box7.set_active(False)
                    self.check_box7.set_sensitive(False) 

            else:
                if self._exstatus:
                    self.check_box7.set_active(True)
                else:
                    self.check_box7.set_active(False)
                    self.check_box7.set_sensitive(False) 

            self.check_box7.connect("toggled",self.flop_checkbox, self.check_box7)
            self.check_box7.show()    
 
            # [Add the checkbox for memRW flag]
            self.check_box9 = gtk.CheckButton("R,W [B]")
            self.check_box9.set_active(False)
            if not(self._exstatus):
               self.check_box9.set_sensitive(False) 
            self.check_box9.connect("toggled",self.memRW_checkbox, self.check_box9)
            self.check_box9.show()  

            # [Add the checkbox for mem dep flag]
            self.check_box8 = gtk.CheckButton("mem dep")

            #print "check", self._filename.replace(self._CWD,"")
            if not(self._exstatus) or (self._filename.replace(self._CWD,"") == "/lcct.dat"):
                self.check_box8.set_active(False)
                self.check_box8.set_sensitive(False) 
            else:
                self.check_box8.set_active(True)

            self.check_box8.connect("toggled",self.dep_checkbox, self.check_box8)
            self.check_box8.show()             



            # for workingSetAna
            self.check_box10 = gtk.CheckButton("wsPage")
            self.check_box10.set_active(False)
            if not(self._exstatus) or wsAnaFlag==0:
               self.check_box10.set_sensitive(False) 
            self.check_box10.connect("toggled",self.wsPage_checkbox, self.check_box10)
            self.check_box10.show()  

            self.check_box11 = gtk.CheckButton("wsRead")
            self.check_box11.set_active(False)
            if not(self._exstatus) or wsAnaFlag==0:
           #if not(self._exstatus):
               self.check_box11.set_sensitive(False) 
            self.check_box11.connect("toggled",self.wsRead_checkbox, self.check_box11)
            self.check_box11.show()  

            self.check_box12 = gtk.CheckButton("wsWrite")
            self.check_box12.set_active(False)
            #if not(self._exstatus):
            if not(self._exstatus) or wsAnaFlag==0:
               self.check_box12.set_sensitive(False) 
            self.check_box12.connect("toggled",self.wsWrite_checkbox, self.check_box12)
            self.check_box12.show()  

            
            # [Set the horizontal Box with the hidden level]
            hidden1 = gtk.HBox()
            hidden1.show()    
            label = gtk.Label()
	    label.set_markup("     ")
            label.show()
            hidden1.pack_start(label) 
            
            # [Set the horizontal Box with the hidden level]
            hidden = gtk.HBox()
            hidden.show()    
            label = gtk.Label()
	    label.set_markup(" ")
            label.show()
            hidden.pack_start(label,0,0)        
           
            # [Declare the function for combobox to get the changes upon the user selection]  
            def combobox_changed(widget):
                self._mode = widget.get_text()
                           
                if self._mode == "Threshold":
                   hbox_Thr.show()
                   hbox_TopN.hide()      

                if self._mode == "TopN":
                   hbox_TopN.show() 
                   hbox_Thr.hide()
                   
                return
            
            # [Declare the combo box for user dynamic input]
            combo = gtk.Combo()
            combo.set_use_arrows_always(1)
            combo.entry.set_editable(0)
            combo.set_popdown_strings(vizPNGLcctm.DisplayModes)
            combo.entry.connect("changed", combobox_changed)
            combo.show()
            
            # [Set the changed method for combo box and add it to the hbox]
            combobox_changed(combo.entry)
            # hbox.pack_start(combo)
            
            # [Fill the main hbox container with the individual hbox and end with the Submit button]
            hbox.pack_start(self.check_box0)
            hbox.add(self.check_box9)  
            hbox.add(self.check_box1)
            hbox.add(self.check_box7)
            hbox.add(self.check_box2)
	    hbox.add(self.check_box3)
            hbox.add(self.check_box4)
	    hbox.add(self.check_box5)
            hbox.add(self.check_box6)

            hbox.add(self.check_box10)
            hbox.add(self.check_box11)
            hbox.add(self.check_box12)

            hbox.add(hidden1)
            hbox.add(self.check_box8) 
            hbox.add(hidden0)
            hbox.add(combo)
            hbox.add(hbox_TopN) 
            hbox.add(hbox_Thr)
            hbox.add(hidden)
            hbox.add(self._hbox_UnScaled)
            hbox.add(self._hbox_Scaled)
            hbox.add(button2)
            hbox.pack_end(button5)


    # [Declare the function for the click event of dynamic inputs]
    def SubmitButton(self,button):
         
         GenerateDot(self)
	
         DisplayImage(self)  
     
         self._hbox_UnScaled.hide()
         self._hbox_Scaled.show() 
        
         return
    
    # [Declare the function for the click event of default inputs]
    def InitialButton(self, button):

        if self._wsAnaFlag==1:
                #print "hoge"
            self._mem = "1"  
            self._ins = "0"
            self._cycle = "0"
            self._cpi = "0"  
            self._bpi = "0"
            self._bpc = "0"
            self._ibf = "0"  
            self._flop = "0"
            self._dependency = "0"
            self._eqTHR = "0"
            self._memRW = "0"  
            self._wsPage = "1"  
            self._wsRead = "1"  
            self._wsWrite = "1"  
        else:
            self._mem = "1"  
            self._ins = "1"
            self._cycle = "0"
            self._cpi = "0"  
            self._bpi = "0"
            self._bpc = "0"
            self._ibf = "0"  
            self._flop = "1"
            self._dependency = "0"
            self._eqTHR = "0"
            self._memRW = "0"  
            self._wsPage = "0"  
            self._wsRead = "0"  
            self._wsWrite = "0"  

        if (self._filename.replace(self._CWD,"") == "/lcct.dat"):
            self._dep = "0"
        else:
            self._dep = "1"

        #print self._dep
      
         # [Setting the default value for topN box]
        self._topN = "8"
        self._entry_TopN.set_text("8") 
        self._entry_Thr.set_text("")
         
        GenerateDot(self)
        
        DisplayImage(self)

         # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show() 

        if self._exstatus:
            self.check_box0.set_active(True)
        else:
            self.check_box0.set_active(False)

        if self._exstatus:
            self.check_box1.set_active(True)
        else:
            self.check_box1.set_active(False)   
         
        self.check_box2.set_active(False)
        self.check_box3.set_active(False)
        self.check_box4.set_active(False)
        self.check_box5.set_active(False)
        self.check_box6.set_active(False)
         
        if self._exstatus:
            self.check_box7.set_active(True)
        else:
            self.check_box7.set_active(False)
         
        self.check_box8.set_active(False) 
        self.check_box9.set_active(False)        
        
         # [Execution for initial button is finished]
        self._initial = "0"
        return      
    
    # [Defined function for Check box options]
    def mem_checkbox(self, widget, checkbox):
        
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        mem_checkbox = checkbox.get_active()

        if mem_checkbox:
           self._mem = "1"
        else:
           self._mem = "0"
         
        GenerateDot(self)
	DisplayImage(self)

	# [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()   
  
        return
      
    # [Defined function for Check box options]
    def ins_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        ins_checkbox = checkbox.get_active()

        if ins_checkbox:
           self._ins = "1"
        else:
           self._ins = "0"
        
        GenerateDot(self)
	DisplayImage(self)

	# [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()   
 
        return 

    # [Defined function for Check box options]
    def cycle_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        cycle_checkbox = checkbox.get_active()

        if cycle_checkbox:
           self._cycle = "1"
        else:
           self._cycle = "0"
        
        GenerateDot(self)
	DisplayImage(self)

	# [Set the configuration to display the UnScaled button and hide the Scaled button] 
 	self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()       
 
        return 
       
    # [Defined function for Check box options]
    def cpi_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        cpi_checkbox = checkbox.get_active()

        if cpi_checkbox:
           self._cpi = "1"
        else:
           self._cpi = "0"
        
        GenerateDot(self)
	DisplayImage(self)

	# [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()

        return 

    # [Defined function for Check box options]
    def bpi_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return
  
        bpi_checkbox = checkbox.get_active()

        if bpi_checkbox:
           self._bpi = "1"
        else:
           self._bpi = "0"
        
        GenerateDot(self)
	DisplayImage(self) 

	# [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()

        return

    # [Defined function for Check box options]
    def bpc_checkbox(self, widget, checkbox):
     
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        bpc_checkbox = checkbox.get_active()

        if bpc_checkbox:
           self._bpc = "1"
        else:
           self._bpc = "0"

        GenerateDot(self)
	DisplayImage(self) 

        # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()
        
        return 

     # [Defined function for Check box options]
    def ibf_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return
  
        ibf_checkbox = checkbox.get_active()

        if ibf_checkbox:
           self._ibf = "1"
        else:
           self._ibf = "0"
        
	GenerateDot(self)
	DisplayImage(self)

        # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()

        return 

    # [Defined function for Check box options]
    def dep_checkbox(self, widget, checkbox):

        # [Return if initial button is pressed]
        if self._initial == "1":
           return
    
        dep_checkbox = checkbox.get_active()

        if dep_checkbox:
           self._dep = "1"
        else:
           self._dep = "0"

        GenerateDot(self)
	DisplayImage(self)

        # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()

        return

    # [Defined function for Check box options]
    def flop_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        flop_checkbox = checkbox.get_active()

        if flop_checkbox:
           self._flop = "1"
        else:
           self._flop = "0"
        
	GenerateDot(self)
	DisplayImage(self)

        # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()

        return 
 
    # [Defined function for Check box options]
    def memRW_checkbox(self, widget, checkbox):
    
        # [Return if initial button is pressed]
        if self._initial == "1":
           return

        memRW_checkbox = checkbox.get_active()

        if memRW_checkbox:
           self._memRW = "1"
        else:
           self._memRW = "0"

        GenerateDot(self)
        DisplayImage(self)

        # [Set the configuration to display the UnScaled button and hide the Scaled button] 
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()  
        return  

    def wsPage_checkbox(self, widget, checkbox):
        if self._initial == "1":
           return
        wsPage_checkbox = checkbox.get_active()
        if wsPage_checkbox:
           self._wsPage = "1"
        else:
           self._wsPage = "0"
        GenerateDot(self)
        DisplayImage(self)
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()  
        return  

    def wsRead_checkbox(self, widget, checkbox):
        if self._initial == "1":
           return
        wsRead_checkbox = checkbox.get_active()
        if wsRead_checkbox:
           self._wsRead = "1"
        else:
           self._wsRead = "0"
        GenerateDot(self)
        DisplayImage(self)
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()  
        return  

    def wsWrite_checkbox(self, widget, checkbox):
        if self._initial == "1":
           return
        wsWrite_checkbox = checkbox.get_active()
        if wsWrite_checkbox:
           self._wsWrite = "1"
        else:
           self._wsWrite = "0"
        GenerateDot(self)
        DisplayImage(self)
        self._hbox_UnScaled.hide()
        self._hbox_Scaled.show()  
        return  
       
       
    # [Declare the function for unscaled display]
    def UnScaled(self, button):
         
         # [Set the configuration to display the Scaled button and hide the Unscaled button] 
         self._hbox_UnScaled.hide()
         self._hbox_Scaled.show()    
         
         # subprocess.call(["ls", "-l", "lcctm.png"])
	
         DisplayImage(self)

         # [Horizontal box container ends with the scrolled_window variable]  
	 #self._hbox.pack_end(self.Scrolled_Window,True, True, 8)    	
 	 return   
   
    # [Declare the function for unscaled display]
    def Scaled(self, button):
         
         # [Set the configuration to display the UnScaled button and hide the Scaled button] 
         self._hbox_UnScaled.show()
         self._hbox_Scaled.hide()    
         
         # subprocess.call(["ls", "-l", "lcctm.png"])
	
         DisplayScaledImage(self)  
        
         return   

    # [Declare the function for returning the widget value to the main function to make the display] 
    def get_widget(self): 
        return self._widget	
    
    # [Declare the function for quit the application]
    def close_application(self, widget):
        gtk.main_quit()
	



