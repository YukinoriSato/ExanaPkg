#!/usr/bin/env python
# coding: UTF-8
import sys,getopt,time
#import struct
#import binascii
#import ctypes
from ctypes import *
from operator import itemgetter, attrgetter
class CacheStat(Structure):
    _fields_ = [
        ("instAdr", c_ulonglong),
        ("l1_hits", c_ulonglong),
        ("l2_hits", c_ulonglong),
        ("l3_hits", c_ulonglong),
        ("miss", c_ulonglong),
        ("l1_conflict", c_ulonglong),
        ("l2_conflict", c_ulonglong),
        ("l3_conflict", c_ulonglong),
        ("n_access", c_ulonglong)
        ]

cache_stat_list=[]
num_inst=0
cache_config=[]

def cache_stat():
    try:
        infile = open("cachesim.dat", 'rb')
    except:
        print sys.exit("Error:  cache_stat.dat   cannot be opened")
        
    global num_inst
    global cache_stat_list
    global cache_config

    b=infile.read(13)
    if b!="cachesim.dat\0":
        #print sys.exit("Error:  cannot read header of cachesim.dat file")
        print "\nWarning: Your cachesim.dat file uses an old format.  We recommend to re-simulate using Exana again!!\n"
        infile.close()
        infile = open("cachesim.dat", 'rb')
    else:
        bsize=calcsize('=IIIIIII')
        b=infile.read(bsize)
        cache_config = unpack('=IIIIIII',b)

    #print cache_config

    num_inst=0
    s=CacheStat()
    flag=infile.readinto(s)
    while flag!=0:
        #print "%x %d %d %d %d %d %d %d %d" % (s.instAdr, s.l1_hits, s.l2_hits, s.l3_hits, s.miss, s.l1_conflict, s.l2_conflict, s.l3_conflict, s.n_access )

    #cache_stat_list.append(s)
        ss=(s.instAdr, s.l1_hits, s.l2_hits, s.l3_hits, s.miss, s.l1_conflict, s.l2_conflict, s.l3_conflict, s.n_access )
        cache_stat_list.append(ss)
        num_inst +=1
        flag=infile.readinto(s)
    #print flag
    

    num_elem=9
    #print num_inst
    infile.close()

"""
    newlist=sorted(list, key=itemgetter(8),reverse=True)
    for i in range(num_inst):
        print "%x %d %d %d %d %d %d %d %d" % (newlist[i][0],newlist[i][1],newlist[i][2],newlist[i][3],newlist[i][4],newlist[i][4],newlist[i][6],newlist[i][7],newlist[i][8])
"""
mempat_stat={}
mempat_stat_list=[]
#index(pinfo[2],pinfo[0],pinfo[1]):[pinfo[3],pinfo[4],pinfo[5],pinfo[6],min,max]
#QcIQIIcQQ

from struct import *
import os

pats_stat = None
mem_inst_pats = {}
mem_insts_a_order = []

def confirmL(mem_inst_name):
	
	if "L" == mem_inst_name[-1]:
		mem_inst_name = mem_inst_name[:-1]
	
	return mem_inst_name

def pat_info(pf):
	
	bsize=calcsize('=cIQQIIc')
	b=pf.read(bsize)
	pinfo = unpack('=cIQQIIc',b)

	return pinfo

def decode_index(index):
    ins=unpack('=Qcq',index)
    return ins

def base_info(pf,base_num,pinfo):
	
	i=0
	bsize=calcsize('=cqqQqq')
        currAddr=pinfo[3]
        max=min=currAddr
        pattern=""
	while i < base_num:
		b=pf.read(bsize)
		base = unpack('=cqqQqq',b)
                # base[0] type
                # base[1] offset between lasting element
                # base[2] repeat
                # base[3] counts in Sequential access
                # base[4] offset of stride access
                # base[5] # of iterations of SeqStr
                # in MemPatMakeStr wirte_bin_Process()
                #  //ptype
                #  //data[0]:pattern offset
		#  //data[4]:REP cnt, appear cnt??
                #  //dcount sap->data[1]/p->inf[1]
		#  //data[2]:data offset, 
		#  //data[3]:stride_cnt, 
		pkind = restore_pkind(base[0])
		poff = restore_pat_off(base[1])
		rep = restore_repeat(base[2])
		bp = restore_base(pinfo,base)

                #print "current %x" % ( currAddr)

		#print poff + "\t" + rep + pkind +":" + bp

                # for output
                if i < 20:
                    if(poff == ""):
                        pattern=pattern + poff + "\t" + rep + pkind +":" + bp +"\n"
                    else:
                        pattern=pattern  + rep + "\t" + poff + pkind +":" + bp +"\n"
                elif i == 20:
                    pattern=pattern + "\t ...\n"
                element = str(pinfo[1])+"x"+str(base[3])
	
                lowAddr=currAddr+base[1]
                if base[5]==0 or base[5]==1 or base[2]==1:
                    currAddr= lowAddr+ (pinfo[1]*base[3] + (base[4] + pinfo[1]*base[3])*base[5])
                else:
                    currAddr= currAddr+ (base[1]+pinfo[1]*base[3] + (base[4] + pinfo[1]*base[3])*base[5])*base[2]
                if lowAddr>currAddr:
                    highAddr=lowAddr+pinfo[1]*base[3]
                    lowAddr=currAddr- pinfo[1]*base[3]
                else:
                    highAddr=currAddr

                #print "low high %x %x" % (lowAddr, highAddr)
                    
                if min>lowAddr:
                    min=lowAddr
                if max<highAddr:
                    max=highAddr
                #print "base_info %s %x %x %x %x %x" % base_info
		#mem_inst_pats[mem_inst_name][1].append(base_info)
		i+=1


        index = pack('=Qcq',pinfo[2],pinfo[0],pinfo[1])

        #print "   %s min&max %x %x" % (restore_pkind(pinfo[6]), min, max)

        mempat_stat.update({index:[pinfo[3],pinfo[4],pinfo[5],pinfo[6],min,max]})	
        ss=(pinfo[2],pinfo[0],pinfo[1], pinfo[3],pinfo[4],pinfo[5],pinfo[6],min,max,pattern)
        mempat_stat_list.append(ss)	
        ins=decode_index(index)
        stat=mempat_stat[index]
        #print  "%x %s%d: StartAddr %x totalAccess %d [B]  base_num(set_num)=%d pat=%s  min %x max %x" % (ins[0], str(ins[1]), ins[2], stat[0], stat[1], stat[2], str(restore_pkind(stat[3])), stat[4], stat[5])

        #print  "%x %s%d: StartAddr %x totalAccess %d [B]  pat=%s (%d)  min %x max %x" % (ins[0], str(ins[1]), ins[2], stat[0], stat[1],  str(restore_pkind(stat[3])),stat[2], stat[4], stat[5])

        #print  mempat_stat[index]
        
def readPattern_parts(pf):
	
	pinfo=pat_info(pf)
	rw,acsize,meminstaddr = pinfo[0:3]
	h_minstaddr = (hex(meminstaddr))[2:]
	mem_inst_name = rw+str(acsize)+"@"+h_minstaddr

        #print "pinfo %s %x %x %x %x %x %s" % pinfo

        # print pattern
        #print mem_inst_name + "={ Start_Address:" + str(hex(pinfo[3])) + ", total accesses " + str(pinfo[4]) + " [B]"

	#mem_inst_name = confirmL(mem_inst_name)
	
	#if False == mem_inst_pats.has_key(meminstaddr):
            #mem_inst_name = confirmL(mem_inst_name)
         #   mem_inst_pats.update({meminstaddr:[pinfo,[]]})		
         #   mem_insts_a_order.append(mem_inst_name)

	base_info(pf,pinfo[5],pinfo)

def read_mempat(f):
	
	global pats_stat
	pats_stat_size=calcsize('=QIIIIIIIIIII')
	
        b = f.read(pats_stat_size)
        pats_stat = unpack('=QIIIIIIIIIII',b)

        #print pats_stat

        i = 0
        while i < pats_stat[6]:
            readPattern_parts(f)
            i+=1
        f.close()



def restore_repeat(rep):
	
	if 1 < rep:
		return "REP_" + str(rep)+"::"
	else:
		return ""

def restore_pkind(pkc):	
	
	if pkc == "0":
		return "Fix"
	elif pkc == "1":
		return "Seq"
	elif pkc == "2":
		return "Str"
	elif pkc == "3":
		return "SeqStr"
	else:
		return "Complex"

def restore_pat_off(poff):
	
	if poff == 0:
		return ""
	else:
		return "+"+str(poff)+"+"

# base[3] counts in Sequential access
# base[4] offset of stride access
# base[5] # of iterations of SeqStr

def restore_base(pinfo,base):
	
	element = str(pinfo[1])+"x"+str(base[3])
	
	if 0 < base[5]:
		if base[5] == 1:
			return "[" + element + ",+" + str(base[4]) + "+," + element + "]"
		else:
			return "[" + element + ",(+" + str(base[4]) + "+," + element + ")*" + str(base[5])+ "]"
	else:
		return "[" + element + "]"

def outputBases(fw,pinfo,binfo):
	
	i=0
	while i < pinfo[5]:
		base = binfo[i]
		pkind = restore_pkind(base[0])
		poff = restore_pat_off(base[1])
		rep = restore_repeat(base[2])
		bp = restore_base(pinfo,base)
                #fw.write(poff + "\t" + rep + pkind +":" + bp + "\n")

                if(poff == ""):
                    fw.write(poff + "\t" + rep + pkind +":" + bp + "\n")
                else:
                    fw.write(rep + "\t" + poff + pkind +":" + bp +"\n")

		i+=1

def outputPatterns(fw):
	
	for name in mem_insts_a_order:
		if mem_inst_pats.has_key(name):
			elm = mem_inst_pats[name]
			saddr = confirmL(hex(elm[0][3]))
			fw.write(name + "={ Start_Address:" + saddr + ",Total_Accessed_Data_Size(byte):" + str(elm[0][4]) + "\n")
			outputBases(fw,elm[0],elm[1])
			fw.write("}\n\n")
		else:
			print "Key Error"
			sys.exit("Key error occurred.")

def output_mempat():
	fw = open("./pats.result","w")
	
	fw.write("All line number in trace_file:"+str(pats_stat[0])+"\n")
	fw.write("Line number without loop_info in trace_file:"+str(pats_stat[0])+"\n")
	fw.write("Patterning access_data number:"+str(pats_stat[1])+"\n")
	fw.write("\n")
	fw.write("---Pattern number of each access pattern kind---\n")
	fw.write("Fix:"+str(pats_stat[2])+"\n")
	fw.write("Sequential:"+str(pats_stat[3])+"\n")
	fw.write("Fix_Stride:"+str(pats_stat[4])+"\n")
	fw.write("Sequential_Stride:"+str(pats_stat[5])+"\n")
	fw.write("\n")
	fw.write("---Pattern stride info---\n")
	fw.write("Memory instruction kind number:"+str(pats_stat[6])+"\n")
	fw.write("Breakdown of base pattern to complex pattern\n")
	fw.write("-Fix:"+str(pats_stat[7])+"\n")
	fw.write("-Sequential:"+str(pats_stat[8])+"\n")
	fw.write("-Stride:"+str(pats_stat[9])+"\n")
	fw.write("-Sequential_Stride:"+str(pats_stat[10])+"\n")
	fw.write("Complex pattern number in memory instruction kind:"+str(pats_stat[11])+"\n")
	fw.write("\n")
	fw.write("---Access Pattern---\n")
	
	outputPatterns(fw)	
	fw.close()


def outputPatternKindFile():
	fk = open("./patkind.result","w")
	
	for name in mem_insts_a_order:
		if mem_inst_pats.has_key(name):
			pat_info = mem_inst_pats[name][0]
			i = 0
			while i < pat_info[5]:
				fk.write(name + ": " + restore_pkind(pat_info[6]) + "\n")
				i += 1
		else:
			print "Key Error"
			sys.exit("Key error occurred.")

	fk.close()


def mempat():
    try:
        infile = open("mempat.dat", 'rb')
    except:
        print sys.exit("Error:  mempat.dat   cannot be opened")

    global mempat_stat

    read_mempat(infile)
    #output_mempat()
 




    infile.close()

class MemInstr(Structure):
    _fields_ = [
        ("instAdr", c_ulonglong),
        ("minAdr", c_ulonglong),
        ("maxAdr", c_ulonglong)
        ]

# memmat_stat_list
# pinfo[2],pinfo[0],pinfo[1], pinfo[3],pinfo[4],pinfo[5],pinfo[6],min,max
# QcIQIIcQQ

def read_meminstr(f):
    b=f.read(13)
    if b!="meminstr.dat\0":
        print "\nError: Your meminstr.dat file is a different format.  \n"
        exit(1);

    tmp = pack('=cIQIIc','0',0,0,0,0,'0')

    #print "print tmp "
    #print tmp

    stat_size=calcsize('=QQQ')

    s=MemInstr()
    flag=f.readinto(s)
    while flag!=0:
    #print "%x %d %d %d %d %d %d %d %d" % (s.instAdr, s.l1_hits, s.l2_hits, s.l3_hits, s.miss, s.l1_conflict, s.l2_conflict, s.l3_conflict, s.n_access )

    #cache_stat_list.append(s)
            ss=(s.instAdr, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], s.minAdr, s.maxAdr)
            mempat_stat_list.append(ss)
            #print "%x %x %x" % (s.instAdr, s.minAdr, s.maxAdr)
            flag=f.readinto(s)
    #print flag


    f.close()


def meminstr():
    try:
        infile = open("meminstr.dat", 'rb')
    except:
        print sys.exit("Error:  meminstr.dat   cannot be opened")

    #global mempat_stat

    read_meminstr(infile)
    #output_mempat()
 




    infile.close()


class LineconfInst(Structure):
    _fields_ = [
        ("instAdr", c_ulonglong),
        ("clevel", c_ulonglong),
        ]

class LineconfOrigin(Structure):
    _fields_ = [
        ("originAdr", c_ulonglong),
        ("cnt", c_ulonglong),
        ]

class filenameLength(Structure):
    _fields_ = [
        ("length", c_int),
        ]


exe_bin_name=""

lineconf_stat=[]
def read_lineconf(f):
    b=f.read(13)
    global exe_bin_name
    if b=="lineconf.002\0":   
        bsize=calcsize('=i')
        #print bsize
        b=f.read(bsize)
        len = unpack('=i',b)
        #print len[0]
        name=f.read(len[0])
        exe_bin_name=name        
        #print "%s %s" % (name, exe_bin_name)
        #print "lineconf.002   %d, %s" %(len[0], name)
    elif b!="lineconf.dat\0":
        print "\nError: Your lineconf.dat file is a different format.  \n"
        exit(1);

    s0=LineconfInst()
    s1=LineconfOrigin()
    flag0=f.readinto(s0)
    while flag0!=0:
            #print "%x %d | "% (s0.instAdr, s0.clevel)
            flag1=f.readinto(s1)
            originPC=[]
            total=0;
            while flag1!=0:
            #ss=(s.instAdr, tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], s.minAdr, s.maxAdr)
            #mempat_stat_list.append(ss)
                if s1.originAdr==0 :
                    break
                total+=s1.cnt
                #print "origin=%x cnt=%d "% (s1.originAdr, s1.cnt)
                ss1=(s1.originAdr, s1.cnt)
                originPC.append(ss1)
                flag1=f.readinto(s1)

            ss0=(s0.instAdr, s0.clevel, total, originPC)
            lineconf_stat.append(ss0)

            flag0=f.readinto(s0)
    #print flag


    f.close()


def read_lineconf_info():
    try:
        infile = open("lineconf.dat", 'rb')
    except:
        print sys.exit("Error:  lineconf.dat   cannot be opened")

    #global mempat_stat

    read_lineconf(infile)
    #output_mempat()


"""



class MemPatInfo(Structure):
    _fields_ = [
        ("line_num", c_ulonglong),
        ("pat_num", c_uint),
        ("base_fix", c_uint),
        ("base_seq", c_uint),
        ("base_str", c_uint),
        ("base_seqstr", c_uint),
        ("num_mem_inst", c_uint),
        ("minst_fix", c_uint),
        ("minst_seq", c_uint),
        ("minst_str", c_uint),
        ("minst_seqstr", c_uint),
        ("minst_complex", c_uint)
        ]


class MemPatterns(Structure):
    _fields_ = [
#	("rw", c_ubyte),
	("size", c_uint),
	("mia", c_ulonglong),
	("da", c_ulonglong),
	("total_size", c_uint),
	("set_num", c_uint),
	("kind", c_ubyte)
        ]

class BasePat(Structure):
    _fields_ = [
	("kind", c_char),
	("pat_off", c_longlong),
	("rep", c_longlong),
        ("elm_count", c_ulonglong),
	("data_off", c_longlong),
	("itr", c_longlong)
        ]
import binascii
    s=MemPatInfo()
    flag=infile.readinto(s)
    if flag!=0:
        mempat_stat=(s.line_num, s.pat_num, s.base_fix, s.base_seq, s.base_str, s.base_seqstr, s.num_mem_inst, s.minst_fix, s.minst_seq, s.minst_str, s.minst_seqstr, s.minst_complex)
        #print mempat_stat
        print "%x %x %x %x %x %x %x %x %x %x %x %x" % (mempat_stat[0], mempat_stat[1], mempat_stat[2], mempat_stat[3], mempat_stat[4], mempat_stat[5], mempat_stat[6], mempat_stat[7], mempat_stat[8], mempat_stat[9], mempat_stat[10], mempat_stat[11])
      #print "%x %x " % (ss[1], ss[2])
        #print s.pat_num

    for i in range(s.num_mem_inst):
        c=binascii.hexlify(infile.read(1))
        print "%s" % c
        s1=MemPatterns()
        flag=infile.readinto(s1)
        if flag!=0:
            ss=(c, s1.size, s1.mia, s1.da, s1.total_size, s1.set_num, s1.kind)
            print "%s %x %x %x %x %x" % (ss[0], ss[1], ss[2], ss[3], ss[4], ss[5])

            for j in range(ss[5]):
                s2=MemPatterns()
                flag=infile.readinto(s2)
                if flag!=0:
                    base_list=(s2.kind, s2.pat_off, s2.rep, s2.elm_count, s2.data_off, s2.itr)
                    print base_list
""" 




if __name__ == "__main__":
    t1 = time.clock()
    cache_stat_reader()
    t2 = time.clock()
    exec_time = t2 - t1
    print "%s %f %s" % ("Execution time = ", exec_time, " [s] ")
    sys.exit()
