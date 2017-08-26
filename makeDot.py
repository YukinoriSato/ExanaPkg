#!/usr/bin/env python

import sys, subprocess
import glob
import re

array=[]

def makeDot(filename):
	name=filename.split('.dot')
	pngName=name[0]+".png"
	command = "dot -Tpng -o %s %s" % (pngName, filename)
	print command
	sh = subprocess.call(command.split(" "))


if __name__ == '__main__':
#main execute

        argc = len(sys.argv) 

	if (argc==1):
		dotFiles=glob.glob('*.dot')
		print dotFiles
		for a in dotFiles:
			makeDot(a)
	elif (argc>1):
		if (sys.argv[1] =="-h"):   
			print '   Usage:  %s [inFile] [infile2] [infile3] ... ' % sys.argv[0]
			print '     argc= %d  inFile = %s' % (argc, sys.argv)
			exit()    

		for i in range(1,argc):
			makeDot(sys.argv[i])

