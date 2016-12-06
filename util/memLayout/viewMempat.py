#!/usr/bin/env python
# coding: UTF-8
import sys,getopt,time
import os,subprocess
import re
import math
#import elf_anl_utl,common_variable,idextend4
import bin_reader
from operator import itemgetter, attrgetter


def printMempat():
	global newMemPatList
	pats_stat=bin_reader.pats_stat
	print("%%")
	print("%%     MemPat statistics")
	print("%%\n")
	print("# of traces: "+str(pats_stat[0]))
	print("Patterning access_data number: "+str(pats_stat[1]))
	print("\n---Pattern number of each access pattern kind---")
	print("Fix:        "+str(pats_stat[2]))
	print("Sequential: "+str(pats_stat[3]))
	print("Fix_Stride: "+str(pats_stat[4]))
	print("SeqStride:  "+str(pats_stat[5]))
	print("\n---Pattern stride info---")
	print("# of memory instructions: "+str(pats_stat[6]))
	print("Breakdown of base pattern to complex pattern")
	print("-Fix:        "+str(pats_stat[7]))
	print("-Sequential: "+str(pats_stat[8]))
	print("-Stride:     "+str(pats_stat[9]))
	print("-SeqStride:  "+str(pats_stat[10]))
	print("Complex pattern number in memory instruction kind:"+str(pats_stat[11]))

	print("\n%%")
	print("%%     MemPat access pattern ")
	print("%%\n")


	#print "print top 2% elememts of merged data"

	for i in range(len(newMemPatList)):
		for j in range(len(newMemPatList[i])):
			curr=newMemPatList[i][j]
			ins=curr[0:3]
			stat=curr[3:]
			print  "%s%d@%x    pat=%s (%d)  [%x, %x] " % (str(ins[1]), ins[2], ins[0],  str(bin_reader.restore_pkind(stat[3])),stat[2], stat[4], stat[5])
			print "%s" % stat[6]

def main():
	global mem_line,no_mem_line,debug_flag,fname
	global mempat_stat
	global newMemPatList


	bin_reader.mempat()

	"""
	for k, v in bin_reader.mempat_stat.items(): 
		ins=bin_reader.decode_index(k)
		stat=v
		#print ins, stat
		print  "%x %s%d: SAddr %x total %d [B]  pat=%s (%d)  min %x max %x" % (ins[0], str(ins[1]), ins[2], stat[0], stat[1],  str(bin_reader.restore_pkind(stat[3])),stat[2], stat[4], stat[5])
	"""

	mempatList=sorted(bin_reader.mempat_stat_list, key=itemgetter(0))

	"""
	for i in range(len(newMemPatList)):
		curr=newMemPatList[i]
		ins=curr[0:3]
		stat=curr[3:]
		#print ins, stat
		print  "%x %s%d: SAddr %x total %d [B]  pat=%s (%d)  min %x max %x" % (ins[0], str(ins[1]), ins[2], stat[0], stat[1],  str(bin_reader.restore_pkind(stat[3])),stat[2], stat[4], stat[5])
	"""
	newMemPatList=[]
	list=[]

	for i in range(len(mempatList)):
		ins=mempatList[i][0]
		#ins=bin_reader.decode_index(x)
		if ins not in list:
			list.append(ins)
			tmp=[]
			tmp.append(mempatList[i])
		else:
			tmp.append(mempatList[i])

		if(i!=len(mempatList)-1):
			if(ins!=mempatList[i+1][0]):
				newMemPatList.append(tmp)
		else:
			newMemPatList.append(tmp)

	#print newMemPatList
			#print "add %x" % ins 
	"""
	for i in range(len(mempatList)):
		ins=mempatList[i][0]
		#ins=bin_reader.decode_index(x)
		tmp=[]
		if ins not in list:
			list.append(ins)
			cnt=0
			newMemPatList[cnt].append(mempatList[i])
		else:
			cnt=cnt+1
			newMemPatList.append(mempatList[i])
			#print "add %x" % ins 
	"""		
	#print "Orig num %d" % len(mempatList)
	#print "Uniq num %d" % len(newMemPatList)

	printMempat()

	#result_idext = idextend4.id_extend_call(mia_da,fname,mia_add_info)
	#write_pmp_addtional_info(fname,mem_line,result_idext)

if __name__ == "__main__":
    t1 = time.clock()
    main()
    t2 = time.clock()
    exec_time = t2 - t1
    #write_rec_prof(mia_da,fname,exec_time)
    sys.exit()
