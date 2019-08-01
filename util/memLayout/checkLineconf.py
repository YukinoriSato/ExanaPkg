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

	#print "open malloc.out"
	cnt=0
	line = mfs.readline()
	while line:
		#print line
		#if 0<= line.find("Calling"):
		if 0<= line.find("C"):
			v=line.split(' ')
			#print v
			#t1=int(v[3],16)
			t1=v[3]
			#t2=int(v[5],16)
			#t2=v[5]
			#print v,t1
			line = mfs.readline()
			#if -1==line.find("Return"):
			if -1==line.find("R"):
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
				#if size >= 0x1000:
				memLocation.append((sintaddr,eintaddr,rsl))
				cnt+=1
				if cnt==1000:
					print "Warning:  the size of malloc.out is beyond the thresold; break the file analysis loop."
					break
				#	print memLocation[-1]
					#malloc_list.append((sintaddr,eintaddr,size,rsl))
				#
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
	if strname=="" :
		strname="[%x, %x]" %(start,end)

	return strname
	
def merge_lineconf(bin_name):

	#print bin_name, len(new_lineconf_stat)

	#global new_lineconf_stat
	global newMemPatList
	#for i in range(len(new_lineconf_stat)):
	#	print "%x %x %d %s" % new_lineconf_stat[i]


	num_check=(len(new_lineconf_stat))
	inum=0
	for i in range(num_check):
		flag=0
		startAddr=0
		endAddr=0
		name=""
		while inum<len(newMemPatList):
			if new_lineconf_stat[i][0][0]==newMemPatList[inum][0][0]:
				#print "check %x " % new_lineconf_stat[i][0][0]
				flag=1
				break
			inum+=1

		if flag!=1:
			inum=0
			while inum<len(newMemPatList):
				if new_lineconf_stat[i][0][0]==newMemPatList[inum][0][0]:
				#print "check %x " % new_lineconf_stat[i][0][0]
					flag=1
					break
				inum+=1

			if flag!=1:
				print "Error:  cannot find the same instruction in newMemPatList  %x " % new_lineconf_stat[i][0][0]
			#sys.exit()
				continue

		for j in range(len(new_lineconf_stat[i])):

			cmd_line = ["addr2line","-ie", bin_name,""]
			cmd_line[3] = str(hex(new_lineconf_stat[i][j][0]))
		        #print cmd_line


			p_l = subprocess.Popen(cmd_line,
					       stdout = subprocess.PIPE,
					       stderr = subprocess.PIPE,
					       shell = False)
			result = p_l.communicate()[0]  # stdout
			# print result 
			result2=result.split('\n')
			srcLocFullPath=result2[0]
			missLocation=result2[0].split('/')
			#print len(newMemPatList[inum][j])
			startAddr=newMemPatList[inum][0][7]
			endAddr=newMemPatList[inum][0][8]
			name=checkMemLayout(startAddr, endAddr)
			#print "missPC   %x   start %x end %x %s" % (newMemPatList[inum][0][0], startAddr, endAddr, name)
			#print "missPC   %s %s " % (cmd_line[3], missLocation)
			
			origin=new_lineconf_stat[i][j][3]
			sa=[]
			ea=[]
			memObj=[]
			originSrc=[]
			originPC=[]
			originCnt=[]
			loc=""
			attr=[]   # inter-array=0, intra-array=1, unknown=2

			for k in range(len(origin)):
				cmd_line = ["addr2line","-ie", bin_name,""]
				cmd_line[3] = str(hex(origin[k][0]))
				p_l = subprocess.Popen(cmd_line,
						       stdout = subprocess.PIPE,
						       stderr = subprocess.PIPE,
						       shell = False)
				result = p_l.communicate()[0]  # stdout
				result2=result.split('\n')
				srcLocFullPath=result2[0]
				location=result2[0].split('/')
				loc=location[len(location)-1]
				jnum=0
				found2=0
				n=0
				while jnum<len(newMemPatList):
					if origin[k][0]==newMemPatList[jnum][0][0]:
						found2=1
						sa.append(newMemPatList[jnum][0][7])
						ea.append(newMemPatList[jnum][0][8])
						n=len(sa)-1
						memObj.append(checkMemLayout(sa[n], ea[n]))
						originSrc.append(loc)
						originPC.append(origin[k][0])
						originCnt.append(origin[k][1])
						break
					jnum+=1
				if found2!=0:
					#print "find origin inst in newMemPatList %x   start %x end %x %s" % (newMemPatList[jnum][0][0], sa[n], ea[n], memObj[n])
					#print "originPC %s %s " % (cmd_line[3], location )
					if name=="" or memObj[n]=="":
						attr.append(2)
						#print "  -> unknown object"
					elif memObj[n]==name:
						attr.append(1)
						#print "  -> intra-array conflict"						
					elif startAddr<= ea[n] and sa[n] <= endAddr  :
						attr.append(1)
						#print "  -> intra-array conflict"						
					else:
						attr.append(0)
						#print "  -> inter-array conflict  %s " % originSrc[n]

			#print "missPC   %x   L%d  total=%d %s %s %s %s" % (new_lineconf_stat[i][j][0], new_lineconf_stat[i][j][1], new_lineconf_stat[i][j][2], sa, ea, memObj, originSrc)
			elem=(new_lineconf_stat[i][j][0], new_lineconf_stat[i][j][1], new_lineconf_stat[i][j][2], name, missLocation[len(missLocation)-1], originPC, originCnt, memObj, originSrc, attr)
			lineconf_ana.append(elem)
			#print ""







new_lineconf_stat=[]
lineconf_ana=[]
def main():
	global mem_line,no_mem_line,debug_flag,fname
	global mempat_stat
	global newMemPatList

	global json_flag
	global debug_flag

	global new_lineconf_stat

	bin_name=""

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
	#try: 
		#print "bin_name:" + str(bin_name)
	#	str(bin_name)
		#print opt,a

	fname="result"

	#print json_flag, debug_flag, bin_name

	#fpath = fpath_re.match(pattern_fname).group(1)

	#print "bin_reader.read_lineconf_info()"
	bin_reader.read_lineconf_info()
	#print bin_reader.exe_bin_name
	#print "|%s|%s|" %( bin_name, bin_reader.exe_bin_name)

	if bin_name=="":
		bin_name=bin_reader.exe_bin_name

	if bin_name=="":
		print "  % checkLineconf.py  [-j, -g -b exefile]"
		print "        Options:   "
		print "           -b option with the exefile name"
		print "                     open debuginfo using addr2line and obtain source location"
		print "           -j option"
		print "                     output json file for ExanaView"

		print "           -g option for outputting statistics for all of instructions"
		print "                     default is outputting instructions exceeding 1% of totals (only top 1% instructions in the Est.lat descending order)"
		sys.exit()



	mallocdpath = "./malloc.out"
	#print "mallocdpath: " + mallocdpath
	if os.path.exists(bin_name) and os.path.exists(mallocdpath) :
		if json_flag==0:
			print "Target execution binary: "+bin_name
	else:
                if os.path.exists(bin_name):
                        print "Required files are missing:  check  "+ "  and  " + str(mallocdpath)
                else:
                        print "Required files are missing:  check  "+ str(bin_name)

                print "  % checkLineconf.py  [-j, -g -b exefile]"
		print "        Options:   "
		print "           -b option with the exefile name"
		print "                     open debuginfo using addr2line and obtain source location"
		print "           -j option"
		print "                     output json file for ExanaView"

		print "           -g option for outputting statistics for all of instructions"
		print "                     default is outputting instructions exceeding 1% of totals (only top 1% instructions in the Est.lat descending order)"
	
		sys.exit()

	#print "bin_reader.cache_stat()"
	bin_reader.cache_stat()

	#print "read_section_header()"

	read_section_header(bin_name,fname)

	#print "read_symbol_info()"
	read_symbol_info(bin_name,fname)

	#print "read_malloc_info()"
	read_malloc_info(bin_name,mallocdpath)


	##print bin_reader.lineconf_stat
	#for i in range(len(new_lineconf_stat)):
	#	print "%x %x %d %s" % new_lineconf_stat[i]

	#sys.exit()

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
	#cnt=0
	for i in range(len(mempatList)):
		ins=mempatList[i][0]
		#print "orgIns %x" % ins
		#ins=bin_reader.decode_index(x)
		if ins not in list:
			list.append(ins)
			tmp=[]
			tmp.append(mempatList[i])
		else:
			tmp.append(mempatList[i])
			#print "len=%d %s" % (len(tmp), tmp)

		if(i!=len(mempatList)-1):
			if(ins!=mempatList[i+1][0]):
				#print "append to newMemPatList %x  %x" % (ins, tmp[0][0])
				#cnt+=1
				newMemPatList.append(tmp)
				
		else:
			newMemPatList.append(tmp)

	#print "cnt, len = %d %d " %(cnt, len(newMemPatList))
	#sys.exit()
	"""
	for i in range(len(newMemPatList)):
	 	print "%x" % newMemPatList[i][0][0]
		#print "add %x" % ins 
	"""
	#print "Orig num %d" % len(mempatList)
	#print "Uniq num %d" % len(newMemPatList)

	#for i in range(len(new_lineconf_stat)):
	#	print "%x %x %d %s" % new_lineconf_stat[i]

	#lineconf_stat2=sorted(bin_reader.lineconf_stat, key=itemgetter(2),reverse=True)

	#new_lineconf_stat=[]
	list=[]

	for i in range(len(bin_reader.lineconf_stat)):
		ins=bin_reader.lineconf_stat[i][0]
		if ins not in list:
			list.append(ins)
			tmp=[]
			tmp.append(bin_reader.lineconf_stat[i])
		else:
			tmp.append(bin_reader.lineconf_stat[i])
			#print "len=%d %s" % len(tmp), tmp

		if(i!=len(bin_reader.lineconf_stat)-1):
			if(ins!=bin_reader.lineconf_stat[i+1][0]):
				new_lineconf_stat.append(tmp)
		else:
			new_lineconf_stat.append(tmp)



	merge_lineconf(bin_name)
	#print lineconf_ana
	lineconf_sort=sorted(lineconf_ana, key=itemgetter(2),reverse=True)

	sum=0
	psum=[0,0,0]

	for i in range(len(lineconf_sort)):
		print "missPC %x  L%d cnt=%d  memObj=%s loc=%s" %(lineconf_sort[i][0],lineconf_sort[i][1],lineconf_sort[i][2], lineconf_sort[i][3], lineconf_sort[i][4])
		sum+=lineconf_sort[i][2]
		for j in range(len(lineconf_sort[i][5])):
			origin=(lineconf_sort[i][5][j],lineconf_sort[i][6][j],lineconf_sort[i][7][j],lineconf_sort[i][8][j],lineconf_sort[i][9][j])
			#print "    originPC %x cnt=%d  %s %s  attr=%d" %(origin[0], origin[1], origin[2], origin[3], origin[4])
			psum[origin[4]]+=origin[1]
			if origin[4]==0:
				print "  [inter-array] %x cnt=%d  memObj=%s loc=%s"%(origin[0], origin[1], origin[2], origin[3])
			elif origin[4]==1:
				print "  [intra-array] %x cnt=%d  memObj=%s loc=%s"%(origin[0], origin[1], origin[2], origin[3])
			else:
				print "  [unknown-object] %x cnt=%d  memObj=%s loc=%s"%(origin[0], origin[1], origin[2], origin[3])
			#print "%x cnt=%d  %s %s" %(origin[0], origin[1], origin[2], origin[3])

	print ""
	print "-----------------------------------------------------------"
	print "            %12s  %12s   %12s   %12s" % ("sum", "inter-array","intra-array","unknown-obj")
	print "#conflict   %12d: %12d   %12d   %12d"%(sum, psum[0], psum[1], psum[2])
	if sum > 0:
		print "%9s   %12s  %12.2f%1s  %12.2f%1s  %12.2f%1s"%("Ratio    ", "", psum[0]*100.0/sum,"%", psum[1]*100.0/sum,"%", psum[2]*100.0/sum,"%")
	else:
		print "%9s   %12s  %12.2f%1s  %12.2f%1s  %12.2f%1s"%("Ratio    ", "", 0,"%", 0,"%", 0,"%")
	#mergeMempatAndCache(bin_name)

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

if __name__ == "__main__":
    t1 = time.clock()
    main()
    t2 = time.clock()
    exec_time = t2 - t1
    #write_rec_prof(mia_da,fname,exec_time)
    sys.exit()
