#!/usr/bin/env python
# coding: UTF-8
import sys,getopt,time
import os,subprocess
import re
import math
#import elf_anl_utl,common_variable,idextend4
import bin_reader
from operator import itemgetter, attrgetter

json_flag=0
debug_flag = 0

L1lat=3
L2lat=9
L3lat=31
DramLat=100

version = "0.1"
debug_flag = 0
text_sec = []
mia_da = {}
mia_add_info = {}
section_info = {}
obj_list = []

Section_info = []
Obj_list = []
malloc_list=[]
mem_line = {}
no_mem_line = []
fname = ""

memLocation=[]  #startAddr, endAddr, name

pat_model = re.compile(r'.+:(.+)')
fs_model = re.compile(r'(.+):\[\d+x(\d+)\]')
stride_model = re.compile(r'(.+):\[\d+x(\d+),\(?\+([-]?\d+)\+,\d+x\d+\)?\*?(\d+)?\]')
pattern_offset = re.compile(r'\+([-]?\d+)\+.+')
prog_name_re = re.compile(r'(.+)\n(.*/)?(.+):(\d+)')


def match_malloc_area():

	for v in mia_add_info.values():
		la=v[12][0]
		ha=v[12][1]
		for ml in malclist:
			mla = ml[3]
			mha = ml[4]
			if mla is not None:
				if mla <= la and ha <=mha:
					v[13][0] = ml[1]
					v[13][1] = ml[2]
					v[13][2] = ml[8]
					break
			else:
				v[13][0] = 0
	return 0


def print_malloc_info(minfo):
	# no,None,size,sintaddr,eintaddr,None,t1,t2,rsl
	# no,mr.group(2),size,sintaddr,eintaddr,mr.group(5),t1,t2,rsl
	#minfo=namedtuple('no mrg2 size sintaddr eintaddr mrg5 t1 t2 rsl')
	print minfo
	print "%s %s %s" % (minfo[0], minfo[1], minfo[2])
	return

n_malloc=0
def read_line_in_source_code(call_addr,prog_name):
	cmd_line = ["addr2line","-fie", prog_name,""]
	cmd_line[3] = call_addr
	#print cmd_line
	p_l = subprocess.Popen(cmd_line,
					stdout = subprocess.PIPE,
					stderr = subprocess.PIPE,
					shell = False)
	result = p_l.communicate()[0]  # stdout
	#print result
	list=result.split('\n')
	cnt=len(list)/2
	#print cnt
	global n_malloc
	n_malloc=n_malloc+1
	for i in range(cnt):
		ffile,line=list[2*i+1].split(':')
		file=ffile.split('/')
		#print i
		#print "%s:%s %s" % (file[len(file)-1], line, list[2*i])
		if(i==0):
			mresult = "malloc(%d)@%s:%s:%s" % (n_malloc, list[2*i],file[len(file)-1], line)
		else:
			mresult += " [inline@%s:%s]" % (list[2*i], line)
	

	#prog_name_re = re.compile(r'(.+)\n(.*/)?(.+):(\d+)')
	#func_name,prog_name,line_num = prog_name_re.match(result).group(1,3,4)
	#mresult = func_name + ": " + line_num + " in " + prog_name + " (Called at "+call_addr+")\n"

	
	#print mresult

	return mresult


def read_malloc_info(prog_name,mallocdpath):
	global malc_list
	#mallocpat1 = re.compile(r"Calling\smalloc@\w+\sat\s(\w+)\s+to\s+(\w+)")#Calling malloc@plt at 4043ce  to 400600
	#Calling malloc@plt at 40269b  to 4005f8
	#mallocpat2 = re.compile(r"calling\smalloc\s\((\d+)\)\s+\[(\w+),\s(\w+)\]\sSize:\s(\d+)\sorgMallocAddr:(\w+)")#calling malloc (1)  [0x7f1f5f616010, 7f1f67757014] Size: 135532548 orgMallocAddr:30ef67a920
	#mallocpat2 = re.compile(r"Retern\sform\smalloc\s\((\d+)\)\s+\[(\w+),\s(\w+)\]\sSize:\s(\d+)\s\+|\s(.+)")

	#mallocpat2 = re.compile(r"Return\sfrom\smalloc\s\((\d+)\)\s+\[(\w+),\s(\w+)\]\sSize:\s(\d+)\s+\|\s(.+)")
	### spell miss form -> from
	#mallocpat2 = re.compile(r"Retern\sfrom\smalloc\s\((\d+)\)\s+\[(\w+),\s(\w+)\]\sSize:\s(\d+)\s+\|\s(.+)")
	try:
		mfs = open(mallocdpath,"r")
	except IOError:
		print "malloc.out can't open."
		sys.exit()

	line = mfs.readline()
	while line:
		#print line
		if 0<= line.find("Calling"):
			v=line.split(' ')
			#print v
			#t1=int(v[3],16)
			t1=v[3]
			#t2=int(v[5],16)
			#t2=v[5]
			#print v,t1
			line = mfs.readline()
			if -1==line.find("Return"):
				no = None
				size = None
				sintaddr = None
				eintaddr = None
				rsl = read_line_in_source_code(t1,prog_name)#fixpoint
				#print rsl
				##minfo = (no,None,size,sintaddr,eintaddr,None,t1,t2,rsl)
				#print " [None]"
					#print_malloc_info(minfo)
				#common_variable.malclist.append(minfo)
			else:
				v2=line.split(' ')
				#print v2
				no = int(v2[3][1:-1])
				size = int(v2[7],16)
				sintaddr = int(v2[4][1:-1],16)#start addr
				eintaddr = int(v2[5][:-1],16)#endaddr
				rsl = read_line_in_source_code(t1,prog_name)#fixpoint
				#print rsl
			##minfo = (no,mr.group(2),size,sintaddr,eintaddr,mr.group(5),t1,t2,rsl)					
				malloc_list.append((sintaddr,eintaddr,size,rsl))
				memLocation.append((sintaddr,eintaddr,rsl))
				#print " [Append]"
					#print_malloc_info(minfo)
				#common_variable.malclist.append(minfo)
				line = mfs.readline()
		else:
			line = mfs.readline()
	mfs.close()
	#print "malclist: " + str(common_variable.malclist)
	"""
	print "print malloc_list"
	for i in range(len(malloc_list)):
		print "  %12x %12x %8x   %s" % (malloc_list[i][0],malloc_list[i][1], malloc_list[i][2], malloc_list[i][3])
	"""

	return 0

def measure_mem_usage():
    memsizelist=[]
    pid = str(os.getpid())
    #print "getpid ==" + pid + "\n"
    fname = "/proc/" + pid + "/status"
    if os.path.exists(fname):
        procfp = open(fname,"r")
        for line in procfp:
            if -1 != line.find("VmPeak"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmPeak:" + byte_unit_conversion(mem))
                memsizelist.append("VmPeak:" + str(mem) + "Kb\n")
                #print mem
            elif -1 != line.find("VmHWM"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmHWM:" + byte_unit_conversion(mem))
                memsizelist.append("VmHWM:" + str(mem) + "Kb\n")
                #print mem
            elif -1 != line.find("VmRSS"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmRSS:" + byte_unit_conversion(mem))
                memsizelist.append("VmRSS:" + str(mem) + "Kb\n")
                #print mem
    else:
        sys.exit("Can not open the specified file:" + fname)
    procfp.close()
    return memsizelist,pid




def read_section_header(prog_name,fname):
    global section_info
    global Section_info
    global text_sec
    #print "read section header"
    #print "output: "+fname+".sec"
    #f_sec = open(fname + ".sec","w")

    args_r = ["readelf","-S",prog_name]
    section_reg1 = re.compile(r'\s+\[\s*(\d+)\]\s(\s*|\S*)\s+\S+\s+(\S+)\s*(\S+)')
    section_reg2 = re.compile(r'\s+(\S*)')
    sh = subprocess.Popen(args_r,
                         stdout = subprocess.PIPE,
                         stderr = subprocess.PIPE,
                         shell = False)
    sh_info = sh.stdout

    i=0
    line = sh_info.readline()
    while i < 7:
        line = sh_info.readline()
        i += 1
    #print line
    while 0 > line.find("Key to Flags:") and 0 > line.find("フラグのキー:"):
        #print line
        #ndx,s_name,addr,off_addr=section_reg1.match(line).group(1,2,3,4)
        ll=section_reg1.match(line)
	#print ll
	ndx=ll.group(1)
	s_name=ll.group(2)
	addr=ll.group(3)
	off_addr=ll.group(4)
	line = sh_info.readline()
	size = (section_reg2.match(line)).group(1)
        #print line
        #f_sec.write("NDX:"+ndx+" Sec_name:"+s_name+" addr:"+addr+" size:"+size+" offset_addr:"+off_addr+".\n")
	section_info.update({ndx:(s_name,addr,size)})
	Section_info.append((s_name, int(addr, 16), int(size, 16)))
	if int(addr, 16)>0:
		memLocation.append((int(addr, 16), int(addr, 16)+int(size, 16), s_name))
		
	#print "%16s %8x %8x" % (s_name, int(addr, 16), int(size, 16))
        line = sh_info.readline()

    sh_info.close()
    #f_sec.close()
    """
    for i in range(len(Section_info)):
	print "section %16s %8x %8x" % (Section_info[i][0], Section_info[i][1], Section_info[i][2]) 
    """

    return 0


def read_symbol_info(prog_name,fname):
    global obj_list
    global Obj_list
    o_list=[]
    #print "read_symbol_info"
    args_r = ["readelf","-s",prog_name]
    sym_name_reg = re.compile(r'Symbol table \'(.+)\' .+')

    var_obj_reg = re.compile(r'\s+\d+: ([0-9a-z]+)\s+(\w+) .+\s+[A-Z]+\s+[A-Z]+\s+(\w+)\s+(.+)')

    p_r = subprocess.Popen(args_r,
                         stdout = subprocess.PIPE,
                         stderr = subprocess.PIPE,
                         shell = False)
    var_info = p_r.stdout

    #f_obj = open("mlayout.out",'w')
    #print "read_symbol_info   output: " + fname + ".symbol"  
    #f_obj.write("variable                        Eff.Addr[start, end]  size[B]  section\n")
    #f_obj.write("--------------------------------------------------------------------\n")
    #f_obj.write("Symbol_name\t SA\t EA\t Size(byte)\n")
    line = var_info.readline()
    while line:
        ndx=""
        table_name=""
        if -1 < sym_name_reg.match(line):#
            table_name= sym_name_reg.match(line).group(1)
        if -1 < var_obj_reg.match(line):
            check = 0
            s_addr,size,ndx,name = var_obj_reg.match(line).group(1,2,3,4)#
            #print "ndx: ",ndx," size:",size

	    # pick up only lines with size > 0 
            if -1 < size.find('0x'):
                if 0 < int(size,16):
                    check = 1
                    s_addr = int(s_addr,16)
                    e_addr = s_addr + int(size,16)
            else:
                if 0 < int(size):
                    check = 1
                    size = hex(int(size))
                    s_addr = int(s_addr,16)
                    e_addr = s_addr + int(size,16)

            if check == 1:
                obj_list.append([s_addr,e_addr,size,name,table_name,[],[],[],ndx])
                if ndx in section_info:
                    s_name = section_info[ndx][0]
                else:
                    s_name = "Not found. Ndx(" + ndx + ")"

		#f_obj.write(name.ljust(30) + "  [" + hex(s_addr) + ", " + hex(e_addr) + "]  " + str(int(size,16)).rjust(6) + "  " + s_name + "\n")

		if (s_name!=".text"):
			o_list.append([s_addr,e_addr, int(size,16) ,name,s_name])			
			if(s_addr>0):
				memLocation.append((s_addr,e_addr,name))
				#memLocation.append((s_addr,e_addr,name+" ["+s_name+"]"))

        line = var_info.readline()

    #f_obj.close()
    """
    for i in range(len(Obj_list)):
	    print "obj  %8x %8x %4x %s %s" % (Obj_list[i][0], Obj_list[i][1], Obj_list[i][2], Obj_list[i][3], Obj_list[i][4])
    """

    Obj_list=sorted(o_list, key=itemgetter(0))


    return 0
def checkMemLayout(start, end):
	list=[]
	ratio=[]
	for i in range(len(memLocation)):
		#print memLocation
		name=memLocation[i][2]
		if(memLocation[i][0]<=start and (start<=memLocation[i][1])):
		#if(memLocation[i][0]<=start and (end<=memLocation[i][1])):
			#name=memLocation[i][2]+" "+name			
			if name not in list:
				list.append(name)
				ratio.append((float)(end-start)/(memLocation[i][1]-memLocation[i][0])*100)
		if(memLocation[i][0]<=end and (end<=memLocation[i][1])):
			if name not in list:
				list.append(name)
				ratio.append((float)(end-start)/(memLocation[i][1]-memLocation[i][0])*100)
		if(start<=memLocation[i][0] and memLocation[i][1]<=end):
			if name not in list:
				list.append(name)
				ratio.append((float)(end-start)/(memLocation[i][1]-memLocation[i][0])*100)
	strname=""
	for i in range(len(list)):
		f='%.1f' %(ratio[i])
		if ratio[i]<100:
			strname=strname+" "+list[i]+";"+f+"%"
		else:
			strname=strname
		"""
		if(list[i].find("malloc(")>=0 and  list[i].find("@")>=0):
			tmp=list[i].split("@")
			strname=strname+" "+tmp[0]+";"+f+"%"
		else:
			strname=strname+" "+list[i]+";"+f+"%"
		"""
	return strname
	

def mergeMempatAndCache(bin_name):
	global newMemPatList
	global L1lat
	global L2lat
	global L3lat
	global DramLat



	#print "num_inst, %d %d" %(len(newMemPatList), bin_reader.num_inst)
	if(len(newMemPatList)!=bin_reader.num_inst):
		print "Error: mempat and cachesim num_inst are different, %d %d" %(len(newMemPatList), bin_reader.num_inst)
		sys.exit()

	c=bin_reader.cache_config
	#print c
	L3flag=1
	if len(c)>0:
		if c[4]==0:
			L3flag=0
			L2lat=DramLat
		if json_flag==0:
			print "Cache Config: %d, %d, %d, %d, %d, %d.  Block=%d\n" % (c[0], c[1], c[2], c[3], c[4], c[5], c[6])

	#print "%s %d " % ("cache_num_inst=", bin_reader.num_inst)
	#newlist=sorted(bin_reader.list, key=itemgetter(8),reverse=True)
	newlist=sorted(bin_reader.cache_stat_list, key=itemgetter(0))
	newlist2=[]
	for i in range(len(newMemPatList)):
		#print "%x %x" %(newlist[i][0], newMemPatList[i][0])
		if(newlist[i][0]!=newMemPatList[i][0][0]):
			print "Error: mempat and cachesim instAddr are different, %x %x" %(newlist[i][0], newMemPatList[i][0][0])
			sys.exit()
		lat=L1lat*newlist[i][1]+L2lat*newlist[i][2]+L3lat*newlist[i][3]+DramLat*newlist[i][4]
		#print lat, newlist[i][1], newlist[i][2], newlist[i][3], newlist[i][4]
		ss=(newlist[i][0],newlist[i][1], newlist[i][2], newlist[i][3],newlist[i][4], newlist[i][5],newlist[i][6],newlist[i][7], newlist[i][8],i,lat )
		newlist2.append(ss)
		#print newlist2
		"""
		for j in range(len(newMemPatList[i])):
			curr=newMemPatList[i][j]
			ins=curr[0:3]
			stat=curr[3:]
			print  "   %s%d: SAddr %x total %d [B]  pat=%s (%d)  min %x max %x" % (str(ins[1]), ins[2], stat[0], stat[1],  str(bin_reader.restore_pkind(stat[3])),stat[2], stat[4], stat[5])
		"""


	
	# for DRAM access 
	# (L3 miss)
	#newlist=sorted(newlist2, key=itemgetter(4),reverse=True)


	# for # memory access (n_access, L1 access)
	#newlist=sorted(newlist2, key=itemgetter(8),reverse=True)

	# for # memory access (L2 access, L1 miss)
	#newlist=sorted(newlist2, key=itemgetter(3),reverse=True)

	# for estimated latency 
	newlist=sorted(newlist2, key=itemgetter(10),reverse=True)

	if debug_flag==0:
		num_check=( bin_reader.num_inst/100)
	else:
		num_check=( bin_reader.num_inst)

	if json_flag==0:
		print "print top 1% elememts of merged data"
	else:
		print "{"
		print "     \"header\": [\"instAddr\", \"Est.lat\", \"Avg.lat\", \"L1 hit\", \"L1 miss\", \"L1 confl\", \"L2 hit\", \"L2 miss\", \"L2 confl\", \"L3 hit\", \"L3 miss\", \"L3 confl\", \"totalAccess\", \"memoryObject\", \"src\"],"
		print "     \"data\":"
		print "     ["

	for i in range(num_check):

	#for i in range( bin_reader.num_inst):

		cmd_line = ["addr2line","-ie", bin_name,""]
		cmd_line[3] = str(hex(newlist[i][0]))
		#print cmd_line


		p_l = subprocess.Popen(cmd_line,
				       stdout = subprocess.PIPE,
				       stderr = subprocess.PIPE,
				       shell = False)
		result = p_l.communicate()[0]  # stdout
		# print result 
		result2=result.split('\n')
		srcLocFullPath=result2[0]
		location=result2[0].split('/')

		data=newlist[i]
		#print data
		L3miss=float(data[4])
		L2miss=float(data[4]+data[3])
		L1miss=float(L2miss+data[2])

		#print"%f %f %f" % (L1miss, L2miss, L3miss)
		#print "  %x %d %d %d %d %d %d %d %d" % (newlist[i][0],newlist[i][1],newlist[i][2],newlist[i][3],newlist[i][4],newlist[i][5],newlist[i][6],newlist[i][7],newlist[i][8])

		if L1miss+data[1]==0 :
			f1=float('nan')
		else:
			f1=L1miss/float(L1miss+data[1])*100
		if L1miss==0:
			f2=float('nan')
		else:
			f2=data[5]/L1miss*100
		if L2miss+data[2]==0:
			f3=float('nan')
		else:
			f3=L2miss/float(L2miss+data[2])*100
		if L2miss==0:
			f4=float('nan')
		else:
			f4=data[6]/L2miss*100
		if L3miss+data[3]==0 or L3flag==0:
			f5=float('nan')
		else:
			f5=L3miss/float(L3miss+data[3])*100
		if L3miss==0 or L3flag==0:
			f6=float('nan')
		else:		
			f6=data[7]/L3miss*100
		if data[8]==0:
			avg=float(0)
		else:
			avg=data[10]/float(data[8])
		#print " %10x | %10d | %6.2f %6.2f | %6.2f %6.2f | %6.2f %6.2f | %24s " % (data[0],  data[4],  L1miss/float(L1miss+data[1])*100 , data[5]/L1miss*100, L2miss/float(L2miss+data[2])*100, data[6]/L2miss*100, L3miss/float(L3miss+data[3])*100,  data[7]/L3miss*100, location[len(location)-1])

		if json_flag==0:
			print "                               |     L1      |     L2      |     L3      |   DRAM "
			print " instAddr |  Est.lat   |Avg.lat| miss% confl%| miss% confl%| miss% confl%| #accesses    "
			print " %8x | %10d | %4.2f|%6.2f %6.2f|%6.2f %6.2f|%6.2f %6.2f| %9d " % (data[0],  data[10], avg, f1, f2, f3, f4, f5, f6, L3miss)
			print "  %s" % location[len(location)-1]
		
		newName=""
		for j in range(len(newMemPatList[data[9]])):
			curr=newMemPatList[data[9]][j]
			ins=curr[0:3]
			stat=curr[3:]

			name=checkMemLayout(stat[4],stat[5])
			#print  "               %s%d: SAddr %x total %d [B]  pat=%s (%d)  %s [%x, %x] " % (str(ins[1]), ins[2], stat[0], stat[1],  str(bin_reader.restore_pkind(stat[3])),stat[2], name, stat[4], stat[5])
			#print  "        %s%d: pat=%s (%d)  %s [%x, %x] " % (str(ins[1]), ins[2],  str(bin_reader.restore_pkind(stat[3])),stat[2], name, stat[4], stat[5])
			if json_flag==0:
				print  "   memObj [%x, %x] %s\n" % (stat[4], stat[5], name)
				print  "        %s%d: pat=%s (%d) " % (str(ins[1]), ins[2],  str(bin_reader.restore_pkind(stat[3])),stat[2])
				print "%s" % stat[6]
			else:
				a="[%x, %x] %s " % (stat[4], stat[5], name)
				newName=newName+a


		if json_flag:			
			if num_check-1!=i:
				print "        [\"%8x\", %d, %.2f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \"%s\", \"%s\"]," % (data[0],  data[10], avg, data[1], L1miss, data[5], data[2], L2miss, data[6], data[3], L3miss, data[7], data[8], newName, srcLocFullPath)	
			else:
				print "        [\"%8x\", %d, %.2f, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, \"%s\", \"%s\"]" % (data[0],  data[10], avg, data[1], L1miss, data[5], data[2], L2miss, data[6], data[3], L3miss, data[7], data[8], newName, srcLocFullPath)	

	if json_flag:
		print "   ]"
		print "}"

def printCacheAnaResult(bin_name):
	global num_inst
	global cache_stat_list
	#print "%s %d %s" % ("num_inst=", bin_reader.num_inst, " sorted by total number of L3 cache misses")
	#newlist=sorted(bin_reader.list, key=itemgetter(8),reverse=True)
	newlist=sorted(bin_reader.cache_stat_list, key=itemgetter(4),reverse=True)

	print "print top 1% elememts of newlist"
	print "                                    |            |      L1       |      L2       |      L3"
	print "       src                 instAddr |  memAccess | miss%  confl% | miss%  confl% | miss%  confl%"
	for i in range( bin_reader.num_inst/50):
		cmd_line = ["addr2line","-ie", bin_name,""]
		cmd_line[3] = str(hex(newlist[i][0]))
		#print cmd_line


		p_l = subprocess.Popen(cmd_line,
				       stdout = subprocess.PIPE,
				       stderr = subprocess.PIPE,
				       shell = False)
		result = p_l.communicate()[0]  # stdout
		# print result 
		result2=result.split('\n')
		location=result2[0].split('/')

		data=newlist[i]
		L3miss=float(data[4])
		L2miss=float(data[4]+data[3])
		L1miss=float(L2miss+data[2])

		#print"%f %f %f" % (L1miss, L2miss, L3miss)
		#print "  %x %d %d %d %d %d %d %d %d" % (newlist[i][0],newlist[i][1],newlist[i][2],newlist[i][3],newlist[i][4],newlist[i][5],newlist[i][6],newlist[i][7],newlist[i][8], location[len(location)-1])
		print "%24s %10x | %10d | %6.2f %6.2f | %6.2f %6.2f | %6.2f %6.2f " % (location[len(location)-1], data[0],  data[4],  L1miss/float(L1miss+data[1])*100 , data[5]/L1miss*100, L2miss/float(L2miss+data[2])*100, data[6]/L2miss*100, L3miss/float(L3miss+data[3])*100,  data[7]/L3miss*100)



def main():
	global mem_line,no_mem_line,debug_flag,fname
	global mempat_stat
	global newMemPatList

	global json_flag
	global debug_flag


	#fname_re = re.compile(r'(.+).result')
	#fpath_re = re.compile(r'(.+)/mapa.result')

	try:
		#opts,args = getopt.getopt(sys.argv[1:],"b:p:vg")
		opts,args = getopt.getopt(sys.argv[1:],"b:p:jg")
	except getopt.GetoptError, err:
		print str(err)
		sys.exit(2)
	#print "args: "+ str(args) + " opts: " + str(opts)

	for opt,a in opts:
		if opt == "-b":
			bin_name = a
		elif opt == "-g":
			debug_flag = 1
		elif opt == "-j":			
			json_flag = 1
	try: 
		#print "bin_name:" + str(bin_name)
		str(bin_name)
	except NameError:
		print opt,a
		print "  % memLayout.py  [-j, -g] -b exefile"
		print "        Options:   "
		print "           -b option with the exefile name"
		print "                     open debuginfo using addr2line and obtain source location"
		print "           -j option"
		print "                     output json file for ExanaView"

		print "           -g option for outputting statistics for all of instructions"
		print "                     default is outputting instructions exceeding 1% of totals (only top 1% instructions in the Est.lat descending order)"
		sys.exit()

	fname="result"

	#print json_flag, debug_flag, bin_name

	#fpath = fpath_re.match(pattern_fname).group(1)

	mallocdpath = "./malloc.out"
	#print "mallocdpath: " + mallocdpath
	if os.path.exists(bin_name) and os.path.exists(mallocdpath) :
		if json_flag==0:
			print "Target execution binary: "+bin_name
	else:
		print "Required files are missing:  check  "+ str(bin_name)+ "  and  " + str(mallocdpath)
		sys.exit()

	bin_reader.cache_stat()

	read_section_header(bin_name,fname)
	read_symbol_info(bin_name,fname)
	read_malloc_info(bin_name,mallocdpath)
	
	#print "fini read_malloc"
	#printCacheAnaResult(bin_name)

	# sort memLocation (section, symbol, malloc)
	newMemLoc=sorted(memLocation, key=itemgetter(0))

	f_obj = open("mlayout.out",'w')

    #print "read_symbol_info   output: " + fname + ".symbol"  
    #f_obj.write("variable                        Eff.Addr[start, end]  size[B]  section\n")
    #f_obj.write("--------------------------------------------------------------------\n")
    #f_obj.write("Symbol_name\t SA\t EA\t Size(byte)\n")
    
	for i in range(len(newMemLoc)):
		#print "%x %x %s" % newMemLoc[i]
		f_obj.write(str(hex(newMemLoc[i][0])) + " "+ str(hex(newMemLoc[i][1])) + " "+ newMemLoc[i][2] + "\n")


	#bin_reader.mempat()
	bin_reader.meminstr()

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

	mergeMempatAndCache(bin_name)

	#result_idext = idextend4.id_extend_call(mia_da,fname,mia_add_info)
	#write_pmp_addtional_info(fname,mem_line,result_idext)

def write_rec_prof(mia_da,fname,exec_time):
    mia_total = len(mia_da)
    mem_res,pid = measure_mem_usage()
    lv_keys = common_variable.lv_counter.keys()
    #print "lv_keys: " +str(lv_keys)
    lv_keys.sort()
    #print "lv_keys: " +str(lv_keys)
    mem_res,pid = measure_mem_usage()
    f_prof = open(fname + '.prof','w')
    f_prof.write("Total count of Memory instruction "+str(mia_total)+"\n")
    f_prof.write("Each of level count\n")
    for lv in lv_keys:
        v = common_variable.lv_counter[lv]
        f_prof.write("Lv "+str(lv)+":"+ str(v)+"\n")
    f_prof.write("Execution time(s): "+str(exec_time)+"\n")
    f_prof.write("PID:" + str(pid)+"\n"+"VmPeak:" + str(mem_res[0]) + ",VmHWM:" + str(mem_res[1]) + ",VmRSS:" + str(mem_res[2])+"\n")
    f_prof.close()
    return 0

"""

def read_result_mpat(prog_name,fname):
    global mia_da
    global mia_add_info
    global mem_line,no_mem_line
    f_rpls = None

    print "read_ResultFile"

    if debug_flag == 1:
		f_rpls = open(fname + '.pmp','w')
		print "output: "+fname+".pmp"

    mia_line_reg = re.compile(r'(.+)={ Start_Address:([a-z0-9]+),Total_Accessed_Data_Size\(byte\):([0-9]+)')
    mia_info_reg = re.compile(r'(W|R)(\d+)@(.+)')
    cmd_line = ["addr2line","-fe", prog_name,""]

    filename=fname+ ".mapa";
    if os.path.exists(prog_name) and os.path.exists(filename):
        f_result = open(filename,"r")
        line = elf_anl_utl.toAccessPattern(f_result)
	print line
        while line:
            if mia_line_reg.match(line):
            	mia_info,sa,total = mia_line_reg.match(line).group(1,2,3)
            	m_addr = mia_info_reg.match(mia_info).group(3)
		#print line
            	if debug_flag == 1:
					f_rpls.write(line)

            	cmd_line[3] = m_addr
            	read_source_line(cmd_line,mem_line,mia_info,no_mem_line)
            	mia_da.update(elf_anl_utl.init_mia_da(mia_info,mia_info_reg,sa,total))


            	mia_add_info.update({mia_info:[[],[],0,[],[],0,[],[],[],[],[],[],[],[]]})
		print mia_add_info
		# f_result is output file stream
		# mia_info is instAdr@RWsize
		# f_rpls is output of debug info

            	#data_address_analyze(line,mia_info,f_result,f_rpls)
            line = f_result.readline()
        f_result.close()
    else:
        print sys.exit("Error:Can not open any specified file:  "+filename)
    if debug_flag == 1:
		f_rpls.close()
    #idextend4.id_extend_call(mia_da,fname)
    return 0

def measure_mem_usage():
    memsizelist=[]
    pid = str(os.getpid())
    #print "getpid ==" + pid + "\n"
    fname = "/proc/" + pid + "/status"
    if os.path.exists(fname):
        procfp = open(fname,"r")
        for line in procfp:
            if -1 != line.find("VmPeak"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmPeak:" + byte_unit_conversion(mem))
                memsizelist.append("VmPeak:" + str(mem) + "Kb\n")
                #print mem
            elif -1 != line.find("VmHWM"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmHWM:" + byte_unit_conversion(mem))
                memsizelist.append("VmHWM:" + str(mem) + "Kb\n")
                #print mem
            elif -1 != line.find("VmRSS"):
                mem = (re.search(r'\d+',line)).group()
                #memsizelist.append("VmRSS:" + byte_unit_conversion(mem))
                memsizelist.append("VmRSS:" + str(mem) + "Kb\n")
                #print mem
    else:
        sys.exit("Can not open the specified file:" + fname)
    procfp.close()
    return memsizelist,pid
"""

if __name__ == "__main__":
    t1 = time.clock()
    main()
    t2 = time.clock()
    exec_time = t2 - t1
    #write_rec_prof(mia_da,fname,exec_time)
    sys.exit()
