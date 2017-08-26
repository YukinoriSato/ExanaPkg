#!/usr/bin/env python

import sys
import re

array=[]

def cut_csv(filename):

	with open(filename, 'r') as f:
                line = f.readline()
		while not 'inFileName' in line:
                    #print "1: "+line
                    #print 'inFileName' in line
                    line = f.readline()                        
                a=line.split('/',1)
                aa=a[1].rstrip()
                #print "%s" % aa

		while not 'profiling time' in line:
			line = f.readline()
                t=line.split()
                #print "%s" % t[2]

		while not 'Mem_ref' in line:
			line = f.readline()

                m=line.split()
                #print "%s" % m[1]

                #cache_miss_count
                line = f.readline()
                nl1m=line.split()

                line = f.readline()
                nl2m= line.split()

                line = f.readline()
                nl3m=line.split()
                #print "%s" % nl1m[1]
                #print "%s" % nl2m[1]
                #print "%s" % nl3m[1]

                line = f.readline()

                #cache_miss_per_ref
                line = f.readline()
                l1m=line.split()

                line = f.readline()
                l2m= line.split()

                line = f.readline()
                l3m=line.split()
                #print "%s" % l1m[1]
                #print "%s" % l2m[1]
                #print "%s" % l3m[1]

                line = f.readline()

                #conflict_miss_per_miss
                line = f.readline()
                l1c= line.split()

                line = f.readline()
                l2c= line.split()
                line = f.readline()
                l3c= line.split()

                #print "%s" % l1c[1]
                #print "%s" % l2c[1]
                #print "%s" % l3c[1]

                #print "%s" % l1c[2]
                #print "%s" % l2c[2]
                #print "%s" % l3c[2]

		array.append((aa, t[2], m[1], nl1m[1], nl2m[1], nl3m[1], l1c[1],l2c[1],l3c[1]))



if __name__ == '__main__':
#main execute

        argc = len(sys.argv) 
        if (argc < 2):   
                print '   Usage:  %s <inFile> [infile2] [infile3] ... ' % sys.argv[0]
                print '     argc= %d  inFile = %s' % (argc, sys.argv)
                exit()    


	for i in range(1,argc):
		#print '     argc= %d  inFile = %s' % (argc, sys.argv[i])
		cut_csv(sys.argv[i])

	fout=open("cacheAna.csv", 'w') 
	print "generating cacheAna.csv "

	for j in range(len(array[0])):
		for i in range(len(array)):
			#print "%d %d " % (i,j)
			if (i!= (len(array)-1)):
				    s="%s, " % array[i][j]
			else:
				    s="%s\n" % array[i][j]
			fout.write(s)
		#fout.write("\n")

