#!/usr/bin/python -W ignore::GtkWarning 

# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
#import webkit


# [Showing the image display in the scrolled window]
def DisplayScaledImage(self):
    
      # [Read the genenrated png file into the pixbuf] 
      pixbuf = gtk.gdk.pixbuf_new_from_file( self._CWD + '/lcctm.png')
        
      # [Get the picture width and height]   
      pix_width = pixbuf.get_width()
      pix_height = pixbuf.get_height()   
      
      pix_aspect_ratio =  pix_width * 1.0 / pix_height
      print "pix_aspect_ratio", pix_width, pix_height, pix_aspect_ratio
      monitor_aspect_ratio = self._monitor.width * 1.0 / self._monitor.height
      print "monitor_aspect_ratio", self._monitor.width/1.5, self._monitor.height/1.5, monitor_aspect_ratio
 	
      # [Resize the picture width and height to fit into the window display]       	
      if pix_aspect_ratio > monitor_aspect_ratio:
         pix_width = self._monitor.width/1.5
         pix_height = self._monitor.height/1.5 * ((self._monitor.width/1.5) / pix_width) 
      else:
         pix_height = self._monitor.height/1.5
         pix_width = self._monitor.width/1.5 * ((self._monitor.height/1.5) / pix_height)
          
      print "pix_aspect_ratio", pix_width, pix_height, pix_aspect_ratio    
      scaled_buf = pixbuf.scale_simple(pix_width, pix_height,gtk.gdk.INTERP_BILINEAR) 
      self._image.set_from_pixbuf(scaled_buf) 
      ###self._image.set_from_pixbuf(pixbuf) 	
        
      # [Add the image container with the scrolled_window variable to show the image]
      #self.Scrolled_Window.add_with_viewport(self._image)
      self.Scrolled_Window.show()
      self._image.show()
	
      # [Horizontal box container ends with the scrolled_window variable]  
      #self._hbox.pack_end(self.Scrolled_Window,True, True, 8)    	
      #print self._thr, self._topN   

      return  

def DisplayImage(self):

      # [Read the genenrated png file into the pixbuf] 
      pixbuf = gtk.gdk.pixbuf_new_from_file( self._CWD + '/lcctm.png')
        
      self._image.set_from_pixbuf(pixbuf) 
      ###self._image.set_from_pixbuf(pixbuf) 	
        
      # [Add the image container with the scrolled_window variable to show the image]
      self.Scrolled_Window.show()
      self._image.show()
	
      return       

# [Showing the HTML display in the scrolled window]
def DisplayHtml(self):
    
        # [Add the image container with the scrolled_window variable to show the image] 
        #self.Scrolled_Window.add_with_viewport(self._view)
	self.Scrolled_Window.show()
        self._view.show()
        ###self._image.show()
       
        # [Horizontal box container ends with the scrolled_window variable]  
	#self._hbox.pack_end(self.Scrolled_Window,True, True, 8)    	
 	#print self._thr, self._topN
    
        return 

# [Showing the HTML display in the view port]
def HtmlView(self):
#    self._view = webkit.WebView()
#    self._view.open('http://localhost:8000/main.php') 
    return

