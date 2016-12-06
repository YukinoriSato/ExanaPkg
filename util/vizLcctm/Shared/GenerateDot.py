# [Importing the required modules for this program]
import os
import subprocess
import gtk
import sys
import shutil

# [Showing the image display in the scrolled window]
def GenerateDot(self):
          
        CheckedFlags = ''  
  
        # [Get the status of the Flag veriables]
        if self._mem == "1":
           CheckedFlags = '-mem'  
        if self._memRW == "1":
           if CheckedFlags == '':
              CheckedFlags = '-RW' 
           else:
              CheckedFlags += ' -RW'   
        if self._ins == "1":
           if CheckedFlags == '':
              CheckedFlags = '-ins' 
           else:
              CheckedFlags += ' -ins'    
        if self._cycle == "1":
           if CheckedFlags == '':
              CheckedFlags = '-cycle' 
           else:
              CheckedFlags += ' -cycle'  
        if self._cpi == "1":
           if CheckedFlags == '':
              CheckedFlags = '-cpi' 
           else:
              CheckedFlags += ' -cpi' 
        if self._bpi == "1":
           if CheckedFlags == '':
              CheckedFlags = '-bpi' 
           else:
              CheckedFlags += ' -bpi' 
        if self._bpc == "1":
           if CheckedFlags == '':
              CheckedFlags = '-bpc' 
           else:
              CheckedFlags += ' -bpc'  
        if self._ibf == "1":
           if CheckedFlags == '':
              CheckedFlags = '-ibf' 
           else:
              CheckedFlags += ' -ibf' 
        if self._dep == "1":
           if CheckedFlags == '':
              CheckedFlags = '-dep' 
           else:
              CheckedFlags += ' -dep' 
        if self._flop == "1":
           if CheckedFlags == '':
              CheckedFlags = '-flop' 
           else:
              CheckedFlags += ' -flop'
        if self._eqTHR == "1":
           if CheckedFlags == '':
              CheckedFlags = '-eqTHR' 
           else:
              CheckedFlags += ' -eqTHR'            

	if self._wsPage == "1":
           if CheckedFlags == '':
              CheckedFlags = '-wsPage' 
           else:
              CheckedFlags += ' -wsPage'  
	if self._wsRead == "1":
           if CheckedFlags == '':
              CheckedFlags = '-wsRead' 
           else:
              CheckedFlags += ' -wsRead'  
	if self._wsWrite == "1":
           if CheckedFlags == '':
              CheckedFlags = '-wsWrite' 
           else:
              CheckedFlags += ' -wsWrite'  


        # print  "Checked Flages:" + CheckedFlags
        
        # [Copy the files befor executing generateDot for HTML]
        if self._SCR == "HTML" and self._base_DIR != self._CWD:
           shutil.copy(self._base_DIR+'/lcctm.png',self._CWD)
           shutil.copy(self._base_DIR+'/lcctm.dot',self._CWD)
           shutil.copy(self._base_DIR+'/out.map',self._CWD)

        # [READ THE 1st LINE OF LCCTM.DOT FILE for the topNodeID]
        if self._initial == "1":
           topNodeID = "0"
        else:
           file = open ( self._CWD +'/lcctm.dot','r')
           topNodeID = file.readline()    
           topNodeID = topNodeID.replace('/* topNodeID=','')
           topNodeID = topNodeID.replace(' */','')
        
        # print  "topNodeID:"+ topNodeID   

        # [Getting the executable path for generateDot]
        generateDot= self._base_DIR + '/generateDot'
        
        # [Checkes the uses inputs and creates the lcctm.png file]
        if topNodeID == "0":
           if CheckedFlags == "":
              if self._thr == "" and self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-topN", "8"])
		 else:
                    subprocess.call([generateDot,self._filename,"-topN", "8","-exefile",self._exefile])     
	      elif self._thr == "": 
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-topN", self._topN]) 
		 else:
                    subprocess.call([generateDot,self._filename,"-topN", self._topN,"-exefile",self._exefile]) 
                 
              elif self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-thr", self._thr]) 
		 else:
                    subprocess.call([generateDot,self._filename,"-thr", self._thr,"-exefile",self._exefile]) 
	      else:
                 print ">> invalid State"
           else:  
              if self._thr == "" and self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename, CheckedFlags ,"-topN", "8"])
		 else:
                   subprocess.call([generateDot,self._filename, CheckedFlags ,"-topN", "8","-exefile",self._exefile])
                 
	      elif self._thr == "": 
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename, CheckedFlags ,"-topN", self._topN])
		 else:
                   subprocess.call([generateDot,self._filename, CheckedFlags ,"-topN", self._topN,"-exefile",self._exefile])
                 
              elif self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename, CheckedFlags ,"-thr", self._thr])
		 else:
                   subprocess.call([generateDot,self._filename, CheckedFlags ,"-thr", self._thr,"-exefile",self._exefile])
               
	      else:
                 print ">> invalid State"
        else:
           if CheckedFlags == "":
              if self._thr == "" and self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-topN", "8","-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,"-topN", "8","-nodeID",topNodeID,"-exefile",self._exefile])  
                 
	      elif self._thr == "": 
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-topN",  self._topN,"-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,"-topN",  self._topN,"-nodeID",topNodeID,"-exefile",self._exefile])  
                 
                
              elif self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,"-thr",  self._thr,"-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,"-thr", self._thr,"-nodeID",topNodeID,"-exefile",self._exefile])  
                  
              else:
                 print ">> invalid State"
           else:  
              if self._thr == "" and self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-topN", "8","-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-topN", "8","-nodeID",topNodeID,"-exefile",self._exefile])  
                     
	      elif self._thr == "": 
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-topN", self._topN,"-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-topN", self._topN,"-nodeID",topNodeID,"-exefile",self._exefile])  
                  
              elif self._topN == "":
                 if self._exefile == None:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-thr", self._thr,"-nodeID",topNodeID])
		 else:
                    subprocess.call([generateDot,self._filename,CheckedFlags,"-thr", self._thr,"-nodeID",topNodeID,"-exefile",self._exefile])  
                   
	      else:
                 print ">> invalid State" 
        
        CheckedFlags = '' 
   
        # [Copy the files befor executing generateDot for HTML]
        if self._SCR == "HTML" and self._base_DIR != self._CWD:
           shutil.copy(self._CWD+'/lcctm.png',self._base_DIR)
           shutil.copy(self._CWD+'/lcctm.dot',self._base_DIR)
           shutil.copy(self._CWD+'/out.map',self._base_DIR)
 
        dotFile=self._CWD +  '/lcctm.dot'
	pngfile=self._CWD +'/lcctm.png'
	command="dot -Tpng "+ dotFile + " -o " + pngfile
	#print command
	subprocess.call(command, shell=True)
        
        # subprocess.call(["ls", "-l", "lcctm.png"])

        return 
