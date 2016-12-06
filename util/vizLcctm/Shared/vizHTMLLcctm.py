#!/usr/bin/python -W ignore::GtkWarning 

# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
import webkit
from Display import DisplayHtml
from Display import HtmlView
from GenerateDot import GenerateDot
from ChangePermission import ChangePermission
from printHowToUse import printHowToUse

# [Declare the class vizLcctm]
class vizHtmlLcctm:
    
    # [Define the DisplayModes for combobox]
    DisplayModes = ['TopN','Threshold']
  
    # [Declare the constructor for vizLcctm class]
    def __init__(self,cwd,base_dir,monitor,filename,exstatus,exefile=None):
            
            # [Define the constructor variables]
	    self._widget = None	
	    self._status = None
	    self._image = None
	    self._combo = None	
	    self._thr = ""
            self._view = None 
            self._hbox = None 	
            self._topN = "" 
            self._mode = "TopN"
  	    self._filename = filename
	    self._mem = "1"  
            self._ins = "1"
            self._cycle = "0"
            self._cpi = "0"  
            self._bpi = "0"
            self._bpc = "0"
            self._ibf = "0"  
            self._dep = "0"
            self._flop = "1"  
            self._dependency = "0"
            self._eqTHR = "0"
            self._memRW = "0"
	    self._initial = "0"
            self._base_DIR = base_dir
            self._CWD = cwd
            self._SCR = "HTML"
            self._monitor = monitor
            self._exefile = exefile
	    self._exstatus = exstatus #'0' for cycleCnt mode & '1' for instCnt mode
  
	    #print "execfile name",self._exefile
	    #print 'Base directory: ', self._base_DIR 
            #print 'Base directory: ', self._CWD  
            
            HtmlView(self) 

            ChangePermission(self)  
            
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
          
            # [Add the image container with the scrolled_window variable to show the image]	   
	    self.Scrolled_Window.add_with_viewport(self._view)
            self._view.show() 
            ###self._image.show()	
            
            # [Vertical box container ends with the scrolled_window variable] 
            vbox.pack_end(self.Scrolled_Window,True, True, 8) 

            # [Create frame for create a separate part for dynamic inputs and add the frame with the vbox]               
	    frame = gtk.Frame("Change of Threshold & TopN")
            frame.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("red"))
            frame.show()
            colorbox = gtk.EventBox()
            colorbox.add(frame) 
            colorbox.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("GOLD"))
            colorbox.show()
            vbox.pack_start(colorbox,0,0)

            # [Create a new horizontal box for dynamic input options and add it into the frame container]
            hbox = gtk.HBox()
            colorbox = gtk.EventBox()
            colorbox.add(hbox)
            hbox.show()
            colorbox.modify_bg(gtk.STATE_NORMAL, gtk.gdk.color_parse("GOLD"))
            frame.add(colorbox)
            colorbox.show()	
            
            # [Create button for Submit display to show the changes in display for the dynamic inputs]  
            button2 = gtk.Button("Submit")
            # [Make a gdk.color for BLUE]
            map = button2.get_colormap()
            color = map.alloc_color("WHITE")
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
	    label.set_markup("                       ")
            label.show()
            hidden0.pack_start(label) 

            # [Add the checkbox for flags]
            self.check_box = gtk.CheckButton("EqTHR")
            self.check_box.set_active(False) 
            if not(self._exstatus):
               self.check_box.set_sensitive(False)    
            self.check_box.connect("toggled",self.eqTHR_checkbox, self.check_box)
            self.check_box.show() 
           
            self.check_box0 = gtk.CheckButton("mem")

            if self._exstatus:
               self.check_box0.set_active(True)
            else:
               self.check_box0.set_active(False) 
               self.check_box0.set_sensitive(False)

            self.check_box0.connect("toggled",self.mem_checkbox, self.check_box0)
            self.check_box0.show()    
            
            self.check_box1 = gtk.CheckButton("ins")

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

            if self._exstatus:
               self.check_box7.set_active(True)
            else:
               self.check_box7.set_active(False)        
               self.check_box7.set_sensitive(False)  
    
            self.check_box7.connect("toggled",self.flop_checkbox, self.check_box7)
            self.check_box7.show()      
            
            # [Set the horizontal Box with the hidden level]
            hidden1 = gtk.HBox()
            hidden1.show()    
            label = gtk.Label()
	    label.set_markup("   ")
            label.show()
            hidden1.pack_start(label) 
            
            # [Add the checkbox for mem dep flag]
            self.check_box8 = gtk.CheckButton("memDep")
            self.check_box8.set_active(False)
            if not(self._exstatus) or (self._filename.replace(self._CWD,"") == "/lcct.dat"):
               self.check_box8.set_sensitive(False) 
            self.check_box8.connect("toggled",self.dep_checkbox, self.check_box8)
            self.check_box8.show()    

            # [Add the checkbox for memRW flag]
            self.check_box9 = gtk.CheckButton("R,W [B]")
            self.check_box9.set_active(False)
            if not(self._exstatus):
               self.check_box9.set_sensitive(False) 
            self.check_box9.connect("toggled",self.memRW_checkbox, self.check_box9)
            self.check_box9.show()         

            # [Set the horizontal Box with the hidden level]
            hidden = gtk.HBox()
            hidden.show()    
            label = gtk.Label()
	    label.set_markup("    ")
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
            combo.set_popdown_strings(vizHtmlLcctm.DisplayModes)
            combo.entry.connect("changed", combobox_changed)
            combo.show()
            
            # [Set the changed method for combo box and add it to the hbox]
            combobox_changed(combo.entry)
            #hbox.pack_start(combo)
            
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
            hbox.add(hidden1)
            hbox.add(self.check_box8)
            hbox.add(hidden) 
            hbox.add(self.check_box)
            hbox.add(hidden0)
            hbox.add(combo)
            hbox.add(hbox_TopN) 
            hbox.add(hbox_Thr)
            hbox.add(button2) 
            hbox.pack_end(button5)

    # [Declare the function for the click event of dynamic inputs]
    def SubmitButton(self, button):
         
         GenerateDot(self)

	 ChangePermission(self)

         HtmlView(self)
 
         DisplayHtml(self)
         
         return   
    
    # [Declare the function for the click event of default inputs]
    def InitialButton(self, button):
         
         self._mem = "1"  
         self._ins = "1"
         self._cycle = "0"
         self._cpi = "0"  
         self._bpi = "0"
         self._bpc = "0"
         self._ibf = "0"  
         self._dep = "0"
         self._flop = "1"  
         self._dependency = "0"
         self._eqTHR = "0"
         self._initial = "1"
         self._memRW = "0"
 
         # [Setting the default value for topN box]
	 self._topN = "8"
       	 self._entry_TopN.set_text("8") 
         self._entry_Thr.set_text("")
         
         GenerateDot(self)
	
         ChangePermission(self)
         
         HtmlView(self)
                
         DisplayHtml(self)
         
         self.check_box.set_active(False)
         
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
    def eqTHR_checkbox(self, widget, checkbox):
      
        # [Return if initial button is pressed] 
        if self._initial == "1":
           return
    
        eqTHR_checkbox = checkbox.get_active()

        if eqTHR_checkbox:
           self._eqTHR = "1"
        else:
           self._eqTHR = "0"
         
        GenerateDot(self)
        ChangePermission(self)  
  
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
        ChangePermission(self)    
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
        ChangePermission(self)  
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
        ChangePermission(self) 
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
           self._MemRW = "0"

        GenerateDot(self)
        ChangePermission(self) 
        return  
    
    # [Declare the function for returning the widget value to the main function to make the display] 
    def get_widget(self): 
	return self._widget	
    
    # [Declare the function for quit the application]
    def close_application(self, widget):
        gtk.main_quit()
	

