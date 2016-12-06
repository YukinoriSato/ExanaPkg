
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>


#include "main.h"
#include "print.h"
#include "file.h"


UINT64 numDyInst=0;
UINT64 numDyLoopNode=0;
UINT64 numDyIrrLoopNode=0;
UINT64 numDyRtnNode=0;

UINT64 numDyMemRead=0;
UINT64 numDyMemWrite=0;
UINT64 numDyDepEdge=0;

UINT64 numDyRecursion=0;
UINT64 numDyCycle=0;

//UINT64 totalInst=0;
//UINT64 cycle_application=0;


bool memFlag=0,insFlag=0,cycleFlag=0,CPIFlag=0,BPIFlag=0,BPCFlag=0,flopFlag=0,IBFFlag=0,depFlag=0,ratioFlag=0,memRWFlag=0,wsReadFlag=0, wsWriteFlag=0, wsPageFlag=0;

//FAISAL: Get information for checked flags...
string checkedFlags()
{
 
 string checkedFlags="";
 
 if ( memFlag == 1 ) checkedFlags = "-mem";
 if ( insFlag == 1 ) 
     if (checkedFlags == "") checkedFlags = "-ins";
     else  checkedFlags += " -ins";
 if ( cycleFlag == 1 ) 
     if (checkedFlags == "") checkedFlags = "-cycle";
     else  checkedFlags += " -cycle";
 if ( CPIFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-cpi";
     else  checkedFlags += " -cpi";
 if ( BPIFlag == 1 ) 
     if (checkedFlags == "") checkedFlags = "-bpi";
     else  checkedFlags += " -bpi";
 if ( BPCFlag == 1 ) 
     if (checkedFlags == "") checkedFlags = "-bpc";
     else  checkedFlags += " -bpc";
 if ( IBFFlag == 1 )
     if (checkedFlags == "") checkedFlags = "-ibf";
     else  checkedFlags += " -ibf"; 
 if ( depFlag == 1 )
     if (checkedFlags == "") checkedFlags = "-dep";
     else  checkedFlags += " -dep"; 
 if ( flopFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-flop";
     else  checkedFlags += " -flop"; 
 if ( ratioFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-eqTHR";
     else  checkedFlags += " -eqTHR"; 
 if ( memRWFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-RW";
     else  checkedFlags += " -RW"; 
 if ( wsPageFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-wsPage";
     else  checkedFlags += " -wsPage"; 
 if ( wsReadFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-wsRd";
     else  checkedFlags += " -wsRd"; 
 if ( wsWriteFlag == 1) 
     if (checkedFlags == "") checkedFlags = "-wsWr";
     else  checkedFlags += " -wsWr"; 

 return checkedFlags;

} 

void count_dynamicNode(struct treeNode *node)
{

  if (node == NULL) return ;
  numDyInst+=node->stat->instCnt;
  numDyCycle+=node->stat->cycleCnt;

  int cnt=0;
  if(cntMode==instCnt){
    cnt=node->stat->instCnt;
  }
  else{
    cnt=node->stat->cycleCnt;
  }

  if(cnt > 0){
    if(node->type==loop){
      numDyLoopNode++;
      if(node->loopType==irreducible)
	numDyIrrLoopNode++;
    }
    else{
      numDyRtnNode++;
      //cout<<hex<<node<<" "<<node->parent<<" ";printNode(node);
    }
  }

  numDyMemRead+=node->stat->memReadByte;
  numDyMemWrite+=node->stat->memWrByte;
  struct depNodeListElem *depElem=node->depNodeList;
  while(depElem){
    numDyDepEdge++;
    depElem=depElem->next;
  }


  struct treeNodeListElem *list=node->recNodeList;
  while(list){
    numDyRecursion++;
    list=list->next;
  }


  count_dynamicNode(node->child);
  count_dynamicNode(node->sibling);

}

struct listOfNodeElem{
  struct treeNode *node;
  struct listOfNodeElem *next;
};

struct listOfNodeElem *orderedListOfTimeCnt=NULL;
struct listOfNodeElem *orderedListOfTimeCntHead=NULL;

void makeOrderedListOfTimeCnt(struct treeNode *node, int topN,  cntModeT printMode)
{

  if (node == NULL) return ;

  struct listOfNodeElem *newElem=new struct listOfNodeElem;
  newElem->node=node;

  //outFileOfProf<<"newElem: ";printNode2(newElem->node, outFileOfProf);outFileOfProf<<" "<<dec<<newElem->node->stat->instCnt<<endl;

  if(orderedListOfTimeCntHead){
    struct listOfNodeElem *curr=orderedListOfTimeCntHead;
    struct listOfNodeElem *prev=NULL;
    int cnt=0;
    while(curr){

      //printNode2(newElem->node, outFileOfProf);outFileOfProf<<" "<<dec<<newElem->node->stat->instCnt<<" ";
      //printNode2(curr->node, outFileOfProf);outFileOfProf<<" "<<dec<<curr->node->stat->instCnt<<endl;

      // for ascending order
      //if(newElem->node->stat->instCnt < curr->node->stat->instCnt ){
      //
      UINT64 newCnt=0, currCnt=0;
      if(printMode==instCnt){
	newCnt=newElem->node->stat->instCnt;
	currCnt=curr->node->stat->instCnt;
      }
      else{
	newCnt=newElem->node->stat->cycleCnt;
	currCnt=curr->node->stat->cycleCnt;
      }



      // for descending order
      if(newCnt > currCnt ){
      //
	if(prev){
	  prev->next=newElem;
	  newElem->next=curr;
	}
	else{
	  newElem->next=orderedListOfTimeCntHead;
	  orderedListOfTimeCntHead=newElem;
	}
	break;
      }
      else if(curr->next==NULL || cnt==topN){
	curr->next=newElem;
	newElem->next=NULL;
	break;
      }
      prev=curr;
      curr=curr->next;
      cnt++;
    }
  }
  else{
    orderedListOfTimeCntHead=newElem;
    newElem->next=NULL;
  }
  //printOrderedListOfInstCnt();

  makeOrderedListOfTimeCnt(node->child, topN, printMode);
  makeOrderedListOfTimeCnt(node->sibling, topN, printMode);

}

UINT64 getMinimumAccumTimeCntFromN(int n, cntModeT printMode)
{
  struct listOfNodeElem *curr=orderedListOfTimeCntHead;

  if(curr==NULL)return 0;

  UINT64 min;
  if(printMode==instCnt)  min=curr->node->statAccum->accumInstCnt;
  else min=curr->node->statAccum->accumCycleCnt;

  //min=curr->node->stat->accumInstCnt;
  for(int i=0;i<n;i++){
    if(curr==NULL)break;
    UINT64 val;
    if(printMode==instCnt)  val=curr->node->statAccum->accumInstCnt;
    else val=curr->node->statAccum->accumCycleCnt;

    if(min > val)
      min= val;
    curr=curr->next;
    if(curr==NULL)break;
  }
  //cout<<"mininum ="<<dec<<min<<endl;
  return min;
}


struct listOfNodeElem *orderedListOfInstCnt=NULL;
struct listOfNodeElem *orderedListOfInstCntHead=NULL;

void printOrderedListOfInstCnt()
{
  int cnt=0;
  struct listOfNodeElem *curr=orderedListOfInstCntHead;
  while(curr){
    cout<<dec<<cnt++<<"  ";
    printNode2(curr->node);
    cout<<"   "<<dec<< curr->node->stat->instCnt<<"  "<<curr->node->statAccum->accumInstCnt <<endl;
    curr=curr->next;
  }
}

void makeOrderedListOfInstCnt(struct treeNode *node, int topN)
{

  if (node == NULL) return ;

  struct listOfNodeElem *newElem=new struct listOfNodeElem;
  newElem->node=node;

  //outFileOfProf<<"newElem: ";printNode2(newElem->node, outFileOfProf);outFileOfProf<<" "<<dec<<newElem->node->stat->instCnt<<endl;

  if(orderedListOfInstCntHead){
    struct listOfNodeElem *curr=orderedListOfInstCntHead;
    struct listOfNodeElem *prev=NULL;
    int cnt=0;
    while(curr){

      //printNode2(newElem->node, outFileOfProf);outFileOfProf<<" "<<dec<<newElem->node->stat->instCnt<<" ";
      //printNode2(curr->node, outFileOfProf);outFileOfProf<<" "<<dec<<curr->node->stat->instCnt<<endl;

      // for ascending order
      //if(newElem->node->stat->instCnt < curr->node->stat->instCnt ){
      //
      // for descending order
      if(newElem->node->stat->instCnt > curr->node->stat->instCnt ){
      //
	if(prev){
	  prev->next=newElem;
	  newElem->next=curr;
	}
	else{
	  newElem->next=orderedListOfInstCntHead;
	  orderedListOfInstCntHead=newElem;
	}
	break;
      }
      else if(curr->next==NULL || cnt==topN){
	curr->next=newElem;
	newElem->next=NULL;
	break;
      }
      prev=curr;
      curr=curr->next;
      cnt++;
    }
  }
  else{
    orderedListOfInstCntHead=newElem;
    newElem->next=NULL;
  }
  //printOrderedListOfInstCnt();

  makeOrderedListOfInstCnt(node->child, topN);
  makeOrderedListOfInstCnt(node->sibling, topN);

}

UINT64 getMinimumAccumInstCntFromN(int n)
{
  struct listOfNodeElem *curr=orderedListOfInstCntHead;
  UINT64 min=curr->node->statAccum->accumInstCnt;
  for(int i=0;i<n;i++){
    if(min > curr->node->statAccum->accumInstCnt)
      min= curr->node->statAccum->accumInstCnt;
    curr=curr->next;
    if(curr==NULL)break;
  }
  //cout<<"mininum ="<<dec<<min<<endl;
  return min;
}


void init_accumStat(struct treeNode *node)
{

  //UINT64 accumInst=0// accumCycle=0, accumFlop=0, accumMem=0;
  if (node == NULL) return;

  node->statAccum=new struct treeNodeStatAccum;

  //cout<<"0: "; printNode(node);
  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;
  
  node->statAccum->accumInstCnt =0;
  node->statAccum->accumCycleCnt =0;
  node->statAccum->accumFlopCnt =0;
  node->statAccum->accumMemAccessByte=0;
  node->statAccum->accumMemAccessByteR=0;
  node->statAccum->accumMemAccessByteW=0;

  //cout<<" statAccum "<<hex<<(node->statAccum)<<endl;

  struct treeNode *child=node->child;

  init_accumStat(node->child);
  init_accumStat(node->sibling);

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;

  return;

}

UINT64 calc_accumInstCnt(struct treeNode *node)
{

  UINT64 accum=0; // accumCycle=0, accumFlop=0, accumMem=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;
  
  accum+=node->stat->instCnt;
  //accum+=calc_accumInstCnt(node->child);
  //accum+=calc_accumInstCnt(node->sibling);
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumInstCnt(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;

  node->statAccum->accumInstCnt=accum;
  //printNode2(node); cout<<" "<<dec<<accum<<endl;
  return accum;

}


UINT64 calc_accumCycleCnt(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;
  
  accum+=node->stat->cycleCnt;

  //accum+=calc_accumCycleCnt(node->child);
  //accum+=calc_accumCycleCnt(node->sibling);

  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumCycleCnt(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;
  node->statAccum->accumCycleCnt=accum;
  return accum;

}

UINT64 calc_accumFlopCnt(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;
  
  accum+=node->stat->FlopCnt;

  //accum+=calc_accumFlopCnt(node->child);
  //accum+=calc_accumFlopCnt(node->sibling);
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumFlopCnt(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;
  node->statAccum->accumFlopCnt=accum;
  return accum;

}


UINT64 calc_accumMemAccessByte(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;
  
  accum+=node->stat->memAccessByte;
  //accum+=calc_accumMemAccessByte(node->child);
  //accum+=calc_accumMemAccessByte(node->sibling);
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumMemAccessByte(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;
  node->statAccum->accumMemAccessByte=accum;
  return accum;

}

UINT64 calc_accumMemAccessByteR(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;

  accum+=node->stat->memReadByte;
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumMemAccessByteR(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;
  node->statAccum->accumMemAccessByteR=accum;
  return accum;

}
UINT64 calc_accumMemAccessByteW(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  //cout<<"now in loop "<<dec<<node->loopID<<" "<<node->stat->instCnt<<endl;

  accum+=node->stat->memWrByte;
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_accumMemAccessByteW(child);
    child=child->sibling;
  }

  //cout<<"accum loopID "<<dec<<node->loopID<<"  "<<accum<<endl;
  node->statAccum->accumMemAccessByteW=accum;
  return accum;

}

extern void printDepNodeList(struct treeNode * );

void createMemDepInfo(struct treeNode *node, enum cntModeT printMode)
{
  //printNode(node);
  //printDepNodeList(node);

  struct depNodeListElem *depElem=node->depNodeList;
  UINT64 cnt=0;
  while(depElem){

    if(depElem->node){

      if(depElem->node->statAccum){
	if(printMode==instCnt) cnt=depElem->node->statAccum->accumInstCnt;
	else cnt=depElem->node->statAccum->accumCycleCnt;
      }
      else{
	depElem=depElem->next;
	continue;
      }
      //cout<<"depNode->node, node "<<hex<<depElem->node<<" "<<node<<endl;
      //cout<<"accumInstCnt, summary_thr "<<dec<<cnt<<" "<<summary_threshold<<endl;

      if((cnt >=summary_threshold) && (depElem->node != node)){
	outDotFile << "\t N"<<hex<<depElem->node<<" -> N"<<node<<" [style = dotted color = red];"<<endl;
      }
      else if((depElem->node == node)){
	// self dep
	if(depElem->selfInterItrDepCnt>0){
	  //outDotFile << "\t N"<<hex<<depElem->node<<" -> N"<<node<<" [style = dashed dir=back color = blue  fontcolor=blue taillabel=\"D<sub>itr</sub>="<<setprecision(1)<< setiosflags(ios::fixed) << depElem->selfInterItrDepDistSum/(float)depElem->selfInterItrDepCnt <<"\" labeldistance = 2 labelangle=30];"<<endl;
	  outDotFile << "\t N"<<hex<<depElem->node<<" -> N"<<node<<" [style = dashed dir=back color = blue  fontcolor=blue taillabel=<D<sub>itr</sub>="<<setprecision(1)<< setiosflags(ios::fixed) << depElem->selfInterItrDepDistSum/(float)depElem->selfInterItrDepCnt <<"> labeldistance = 2 labelangle=30];"<<endl;
	}
	if(depElem->selfInterAppearDepCnt>0){
	  outDotFile << "\t N"<<hex<<depElem->node<<" -> N"<<node<<" [style = dashed dir=back color = darkorange fontcolor=darkorange label=<D<sub>invoke</sub>="<<setprecision(1)<< setiosflags(ios::fixed)  <<depElem->selfInterAppearDepDistSum/(float)depElem->selfInterAppearDepCnt <<"> labeldistance = 2 ];"<<endl;
	}
	if(depElem->selfInterAppearDepCnt==0 && depElem->selfInterItrDepCnt==0){
	  // dependencies among the same body (inside an iteration or an appearance)
	  //  outDotFile << "\t N"<<hex<<depElem->node<<" -> N"<<node<<" [style = dashed dir=back color = deeppink fontcolor=deeppink label=<self> labeldistance = 2 ];"<<endl;
	}
      }
      else{
	//cout<<"else ";printNode(depElem->node);
      }
    }
    else{
      //cout<<"else depNode->node==NULL"<<endl;
    }
    depElem=depElem->next;
  }
}

void printNodeStat(struct treeNode *node){
  outDotFile << "\t N"<<hex<<node<<" -> N"<<hex<<node<<" [ color=transparent, headlabel=\" \\n ";
  
      outDotFile<<" \\n ";
      UINT64 flop=node->statAccum->accumFlopCnt;

      // added by yukinori 2014/04/27
      if(memFlag)
	outDotFile<<scientific << setprecision(2)<< (float) node->statAccum->accumMemAccessByte<<"[B]\\n"<<fixed;
      if(memRWFlag){
	outDotFile<<"  R="<<scientific << setprecision(2)<<(float)node->statAccum->accumMemAccessByteR<<" ("<<(float)node->stat->memReadByte<<") [B] \\n"<<"  W="<< (float)node->statAccum->accumMemAccessByteW<<" ("<<(float)node->stat->memWrByte<<") [B]\\n"<<fixed;
      } 
      //outDotFile<<scientific << setprecision(2)<< (float) flop<<" [FLOP] \\n";
      if(insFlag)
	outDotFile<<scientific << setprecision(2)<< (float) node->statAccum->accumInstCnt<<"[ins]\\n"<<fixed;
      if(flopFlag)
	outDotFile<<scientific << setprecision(2)<< (float) node->statAccum->accumFlopCnt<<"[flop]\\n"<<fixed;
      if(cycleFlag)
	outDotFile<<scientific << setprecision(2)<< (float) node->statAccum->accumCycleCnt<<"[cycle]\\n"<<fixed;
      if(CPIFlag)
      	outDotFile<<"   CPI="<<setprecision(2)<<node->statAccum->accumCycleCnt/(float)node->statAccum->accumInstCnt<<"\\n"<<fixed;
      if(BPIFlag)
	outDotFile<<"   B/ins="<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)node->statAccum->accumInstCnt<<"\\n"<<fixed;
      if(BPCFlag)
	outDotFile<<"   B/cycle="<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)node->statAccum->accumCycleCnt<<"\\n"<<fixed;
      if(IBFFlag){
	outDotFile<<"   iBF="<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop<<"\\n";
	}
      struct workingSetInfoElem *a=&(node->workingSetInfo);
      if(wsPageFlag){
	if(a->minCntRW!=a->maxCntRW)
	  outDotFile<<"   wsPage="<<setprecision(1)<<fixed<<a->sumRW/(float) node->stat->n_appearance<<" ["<<dec<<a->minCntRW<<", "<<a->maxCntRW<<"]\\n";
	else
	  outDotFile<<"   wsPage="<<dec<<a->minCntRW<<"\\n";
      }    
      if(wsReadFlag){
	if(a->minCntR!=a->maxCntR)
	  outDotFile<<"   wsRead="<<setprecision(1)<<fixed<<a->sumR/(float) node->stat->n_appearance<<" ["<<dec<<a->minCntR<<", "<<a->maxCntR<<"]\\n";
	else
	  outDotFile<<"   wsRead="<<dec<<a->minCntR<<"\\n";
      }    
      if(wsWriteFlag){
	if(a->minCntW!=a->maxCntW)
	  outDotFile<<"   wsWrite="<<setprecision(1)<<fixed<<a->sumW/(float) node->stat->n_appearance<<" ["<<dec<<a->minCntW<<", "<<a->maxCntW<<"]\\n";
	else
	  outDotFile<<"   wsWrite="<<dec<<a->minCntW<<"\\n";

      }    

      // end

  outDotFile<<" \" ,labeldistance = 2];"<<endl;

}

/*
UINT64 funcInfoNum=0;
struct funcInfoT *funcInfo;

void printFuncInfo(void)
{
  UINT i;
  for(i=0; i< funcInfoNum;i++)
    cout<<"readelf "<<hex<<funcInfo[i].addr<<" "<< funcInfo[i].size<<endl;
}
*/

char *sys_addr2line_funcName(char *exefileName, ADDRINT addr)
{ 

  char str[512];
  FILE *fp;
  char outfileName[80];

  //cout<<"addr "<<hex<<addr<<endl;
  sprintf(outfileName,"/tmp/addr2line%d.tmp",getpid());
  snprintf(str, 512, "addr2line -e %s -i -f %llx > %s", exefileName, addr, outfileName);

  //printf("commnad   %s\n",str);
  //printf("readelf -s %s |grep FUNC | awk '{print $2 \" \" $3}' > %s\n",(*inFileName).c_str(), filename);

  system(str);

  if((fp=fopen(outfileName,"r"))==NULL){
    fprintf(stderr,"file open error!!! %s\n", outfileName);
    exit(-1);
  }

  int c;
  char s[1024];
  static char funcName[1024];
  if(!fgets(funcName, 1024, fp)){
    fprintf(stderr,"fgets file open error!!! %s\n", outfileName);
    exit(-1);
  }
  int line=1;
  while(fgets(s, 1024, fp)) {
    line++;
  }
#if 0
  while((c = getc(fp)) != EOF) {
    putchar(c);
    if(c == '\n') line++;
  }
#endif
  //cout<<"line "<<dec<<line<<" "<<endl;
  //printf("funcName %s",funcName);
  //funcInfo=new struct funcInfoT[line];

  //fseek(fp, 0, SEEK_SET);



  fclose(fp);
  sprintf(str,"rm -f %s",outfileName);
  system(str);

  //exit(1);

  //printFuncInfo();
  return funcName;
}


void createDotNode_mem(struct treeNode *node, int dep, enum cntModeT printMode, int nodeDependency, float summary_threshold_ratio, string fileName)
{
  if (node == NULL) return;
  //cout<<"exefilename1 "<<exefileName<<endl;

  //cout<< "Printing depth:" <<dep<<" nodedependency:"<<nodeDependency<<endl;
 
  float accumRatio, ratio;
  if(printMode==instCnt){
    accumRatio=(float)node->statAccum->accumInstCnt*100/totalInst;
    ratio=(float)node->stat->instCnt*100/totalInst;
  }
  else{
    accumRatio=(float)node->statAccum->accumCycleCnt*100/cycle_application;
    ratio=(float)node->stat->cycleCnt*100/cycle_application;
  }

  //string rtnName=RTN_FindNameByAddress(node->rtnTopAddr);;

  string rtnName=*(node->rtnName);
  char *rtnNameChar=demangle(rtnName.c_str());
  //char *rtnNameChar=(char *) rtnName.c_str();

  if(rtnNameChar==NULL)
    rtnNameChar=(char *)rtnName.c_str();

  //cout<<rtnName<<" "<<rtnNameChar<<endl;

  //printNode(node, outDotFile);

  if(node->type==loop){

    char buf[1024];
    outDotFile << "\t N"<<hex<<node<<" [";
    if(node->loopType==irreducible)
      outDotFile <<"peripheries = 2 ";

    struct loopTripInfoElem *elem=node->loopTripInfo;
    //sprintf(buf, "#avg.itr: %.2f", elem->sum_tripCnt/(float) node->stat->n_appearance);
    bool shortFlag=0;
    float avgItr=elem->sum_tripCnt/(float) node->stat->n_appearance;
    if(elem->max_tripCnt==elem->min_tripCnt){
      sprintf(buf, "#itr: %llu", elem->max_tripCnt);
      shortFlag=1;
    }
    else if(avgItr<100)
      sprintf(buf, "#itr: %.2f [min=%llu, max=%llu]", avgItr, elem->min_tripCnt, elem->max_tripCnt);
    else
      sprintf(buf, "#itr: %.1f [min=%llu, max=%llu]", avgItr, elem->min_tripCnt, elem->max_tripCnt);

    struct gListOfLoops *gListPtr=isLoopLineInSrc(node->loopID);
    if(gListPtr){

      outDotFile <<"label=<loop "<<dec<<node->loopID<<"<br/>"
		 <<"#invoke: "<<dec<<node->stat->n_appearance;
      if(!shortFlag)outDotFile<<"<br/> ";
      else outDotFile<<"   ";
      outDotFile<<buf<<"<br/>   "
		 <<*gListPtr->fileName<<":"<<gListPtr->srcLine<<" <br/> ";

      //cout<<"exefilename2 "<<exefileName<<endl;
      if(!exefileName.empty()){
	char *funcName=sys_addr2line_funcName((char *) exefileName.c_str(),gListPtr->instAdr);
	if(funcName[strlen(funcName)-1]=='\n')
	  funcName[strlen(funcName)-1]='\0';
	//printf("funcName %s\n",funcName);
	
	if(strncmp(funcName, (*node->rtnName).c_str(), strlen(funcName))){
	  outDotFile <<"<font color=\"red\"> [["<<funcName<<"]] </font><br/>";
	  //cout<<"insert "<<funcName<<endl;
	}
      }
      

      outDotFile << setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right << accumRatio<<"% ("<<ratio<<"%)"
		 <<" >]";


      /*
      //Faisal: Remove the semicolon and endl
      outDotFile <<"label=\"loop "<<dec<<node->loopID<<"\\n "
		 <<buf<<",   "<<"#invoke: "<<dec<<node->stat->n_appearance<<" \\n "
		 <<*gListPtr->fileName<<":"<<gListPtr->srcLine<<" \\n ";

      //cout<<"exefilename2 "<<exefileName<<endl;
      if(!exefileName.empty()){
	char *funcName=sys_addr2line_funcName((char *) exefileName.c_str(),gListPtr->instAdr);
	if(funcName[strlen(funcName)-1]=='\n')
	  funcName[strlen(funcName)-1]='\0';
	//printf("funcName %s\n",funcName);
	
	if(strncmp(funcName, (*node->rtnName).c_str(), strlen(funcName))){
	  outDotFile <<"("<<funcName<<") \\n ";
	  //cout<<"insert "<<funcName<<endl;
	}
      }
      

      outDotFile << setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right << accumRatio<<"% ("<<ratio<<"%)"
		 <<" \"]";
      */



      //outDotFile <<"label=\"loop "<<dec<<node->loopID<<"\\n @0x"<<hex<<gListPtr->instAdr<<" \\n "<< setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right << accumRatio<<"% ("<<ratio<<"%) \"];"<<endl;
    }
    else{
      outDotFile <<"label=\"loop "<<dec<<node->loopID<<" \\n "
		 <<"#invoke: "<<dec<<node->stat->n_appearance;
      if(!shortFlag)outDotFile<<"\\n  ";
      else outDotFile<<"   ";
      outDotFile<<buf<<" \\n "
		 << setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right << accumRatio<<"% ("<<ratio<<"%)"
		 <<" \"]";//Faisal: Remove the semicolon and endl
    }

    //Faisal: Add the URL option 
    float eqRatio;
    if (checkedFlags().find("-eqTHR") != std::string::npos)
    {   
       eqRatio =  (accumRatio)*(summary_threshold_ratio);
       if (eqRatio > 50)
          eqRatio = summary_threshold_ratio; 
    }
    else 
       eqRatio = (summary_threshold_ratio);// /(1.49) ;

    //Code to find the working directory folder...
    string fname = fileName.substr(fileName.find_last_of("/\\") + 1);
    //std::cout<<"Fname: "<<fname<<"\n";
    
    outDotFile<<" [URL=\"index.php?nodeID="<<dec<<node->nodeID<<"&filename="<< fname<<"&checkedflags="<<checkedFlags()<<"&summary_threshold_ratio="<<eqRatio<<"&exefile="<<exefileName<<"\"];"<<endl;


    //fprintf(outDotFile, "%.1f %s, %.1f %s];\n", accumRatio,"%",ratio,"%");

    /*
    outDotFile<<"#appear: "<<dec<<node->stat->n_appearance;
    outDotFile<<" \\n ";

    outDotFile<<buf;
    */

    if(printMode==instCnt){
      printNodeStat(node);
    }


  }
  else{
    //outFileOfProf<<"In createDot: "<<endl;printRtnID();

    //outDotFile << "\t N"<<hex<<node<<" [shape=box, label=\""<<rtnName<<" \\n ";
    outDotFile << "\t N"<<hex<<node<<" [shape=box, label=\""<<rtnNameChar<<" \\n ";
#if 0
    int rtnID=-1;
    //rtnID=getRtnID(node->rtnName);
      if(rtnID!=-1){
	outDotFile <<*(bblArrayList[rtnID]->filename)<<":"<<dec<<bblArrayList[rtnID]->line<<" \\n ";
      }
#endif

      outDotFile<<"#invoke: "<<dec<<node->stat->n_appearance<<" \\n ";
       
      //FAISAL: add below line for the URL option 
      float eqRatio;
      if (checkedFlags().find("-eqTHR") != std::string::npos)
      {   
       eqRatio =  (accumRatio)*(summary_threshold_ratio);
       if (eqRatio > 50)
          eqRatio = summary_threshold_ratio; 
      }
      else 
         eqRatio = (summary_threshold_ratio);// /(1.49); 
 
      //Code to find the working directory folder...
      string fname = fileName.substr(fileName.find_last_of("/\\") + 1);
      //std::cout<<"Fname: "<<fname<<"\n";
     
      outDotFile <<setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right <<accumRatio<<"% ("<<ratio<<"%) \"]";
      outDotFile<<" [URL=\"index.php?nodeID="<<dec<<node->nodeID<<"&filename="<<fname<<"&checkedflags="<<checkedFlags()<<"&summary_threshold_ratio="<<eqRatio <<"&exefile="<<exefileName<<"\"];"<<endl; 
    //fprintf(outDotFile, "%.1f %s, %.1f %s];\n", accumRatio,"%",ratio,"%");

    //outDotFile << "\t N"<<hex<<node<<" -> N"<<hex<<node<<" [ color=transparent, headlabel=\" \\n \\n ";
    //outDotFile<<"#appear: "<<dec<<node->stat->n_appearance;

    if(printMode==instCnt){
      printNodeStat(node);
      /*
      outDotFile<<" \\n ";
      UINT64 flop=node->statAccum->accumFlopCnt;

      // added by yukinori 2014/04/27
      outDotFile<<scientific << setprecision(2)<< (float) node->statAccum->accumMemAccessByte<<"[B]\\n"<<fixed;
      //outDotFile<<scientific << setprecision(2)<< (float) flop<<" [FLOP] \\n";
      if(flop){
	outDotFile<<"   iBF="<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop;
      }
      // end
      */
    }

    //outDotFile<<" \" ,labeldistance = 2];"<<endl;

  }
  if(node->parent)
    outDotFile << "\t N"<<hex<<node->parent<<" -> N"<<node<<" ;"<<endl;

  struct treeNodeListElem *list=node->recNodeList;
  while(list){
    outDotFile << "\t N"<<hex<<list->node<<" -> N"<<node<<" [style=bold, dir=back, color=green] ;"<<endl;
    //cerr<<"nodeID="<<dec<<list->node->nodeID<<" "<<node->nodeID<<endl;
    list=list->next;
  }
  
  //FAISAL: Show or Extract the node dependency
  //if (nodeDependency == 0)
  if (depFlag)
     createMemDepInfo(node, printMode);

}



#define MAX_NODE_DEPTH 1000
struct nodeDepTableElem{
  struct treeNode *node;
  struct nodeDepTableElem *next;
};
struct nodeDepTableElem **nodeDepTable;
int maxNodeDep=0;
void addDepTableNode(struct treeNode *node, int dep)
{
  if(dep<=MAX_NODE_DEPTH){
    struct nodeDepTableElem *newElem=new struct nodeDepTableElem;    
    newElem->node=node;
    if(nodeDepTable[dep-1]){
      newElem->next= nodeDepTable[dep-1]->next;
      nodeDepTable[dep-1]->next=newElem;
    }
    else{
      newElem->next= nodeDepTable[dep-1];
      nodeDepTable[dep-1]=newElem;
    }
  }
  else{
    cout<<"ERROR: exceed MAX_NODE_DEPTH @main.pcc:addDepTableNode()"<<endl;
    //exit(1);
  }
}
void printDepTable(void)
{
  for(int i=0;i<maxNodeDep;i++){
    cout<<"level-"<<dec<<i<<":   ";
    for(struct nodeDepTableElem *elem=nodeDepTable[i];elem;elem=elem->next){
      cout<<"N"<<hex<<elem->node<<" ";
    }
    cout<<endl;
  }
}

void printLevel(void)
{
  for(int i=0;i<maxNodeDep;i++){
    outDotFile<<"level_"<<dec<<i<<" [pos=\"0,"<<i*100<<"\" shape=none];";
  }

  outDotFile<<"level_0";
  for(int i=1;i<maxNodeDep;i++){
    outDotFile<<" -> level_"<<dec<<i;
  }
  outDotFile<<"[color=transparent];"<<endl;

}
	
void printRankAndDepth(void)
{
  for(int i=0;i<maxNodeDep;i++){
    outDotFile<<"{rank = same; level_"<<dec<<i<<"; ";
    //cout<<"\n";
    for(struct nodeDepTableElem *elem=nodeDepTable[i];elem;elem=elem->next){
      outDotFile<<"N"<<hex<<elem->node<<"; ";
      //cout<<"N"<<hex<<elem->node<<"; ";
    }
    outDotFile<<"} "<<endl;
  }

  //FAISAL: MAKE THE URL FOR EACH NODE
  /*for(int i=0;i<maxNodeDep;i++){
    for(struct nodeDepTableElem *elem=nodeDepTable[i];elem;elem=elem->next){
      outDotFile<<"{ \""<<hex<<elem->node<<"\""; 
      outDotFile<<" [URL=\"test.php?nodeID="<<hex<<elem->node<<"\"];";
      outDotFile<<"} "<<endl;  
    }    
  }*/
  

}	

void dfs_dotNode2(struct treeNode *node, int dep, enum cntModeT printMode, int nodeDependency, float summary_threshold_ratio, string fileName)
{
  if (node == NULL) return;
  UINT64 cnt=0;
  if(printMode==instCnt)  cnt=node->statAccum->accumInstCnt;
  else   cnt=node->statAccum->accumCycleCnt;

  if(cnt>=summary_threshold){
  //if(1){
    if(dep>maxNodeDep)maxNodeDep=dep;
    //addDepTableNode(node, dep);
    //cout<< "printing maxNodeDep:"<<maxNodeDep<< dec<<node->nodeID<<endl;
    createDotNode_mem(node, dep, printMode,nodeDependency,summary_threshold_ratio, fileName); //FAISAL: add node dependency & topN
  }
  dfs_dotNode2(node->child, dep+1, printMode,nodeDependency, summary_threshold_ratio, fileName);
  dfs_dotNode2(node->sibling, dep, printMode,nodeDependency, summary_threshold_ratio, fileName);

}

void makeDepNodeTable(struct treeNode *node, int dep, enum cntModeT printMode)
{
  if (node == NULL) return;

  UINT64 cnt=0;

  if(printMode==instCnt) cnt=node->statAccum->accumInstCnt;
  else cnt=node->statAccum->accumCycleCnt;

  if(cnt>=summary_threshold){
  //if(1){
    if(dep>maxNodeDep)maxNodeDep=dep;
    addDepTableNode(node, dep);
  }
  makeDepNodeTable(node->child, dep+1, printMode);
  makeDepNodeTable(node->sibling, dep, printMode);
}

void buildDotFileOfNodeTree_mem(struct treeNode *node, enum cntModeT printMode, int nodeDependency, float summary_threshold_ratio, string fileName,string topNodeID)
{

  int dep=1;
  nodeDepTable=new struct nodeDepTableElem *[MAX_NODE_DEPTH];
  if(nodeDepTable==NULL){
    cout<<"Error: we cannot allocate nodeDepTableElem in buildDotFileOfNodeTree_mem"<<endl;
    return;
  }

  memset(nodeDepTable, 0, sizeof(struct nodeDepTableElem* )*MAX_NODE_DEPTH);

  outDotFile << "/* topNodeID=" << topNodeID <<" */"<< endl;
  outDotFile << "digraph G { "<< endl;
  outDotFile << " graph [outputMode=nodesfirst packMode=\"graph\" splines=true size=\"1000,1000\"]; " <<endl;
  outDotFile << "label=\""<<outDotFileName <<" (Thr="<<setprecision(2)<<setiosflags(ios::fixed) << setw(5)<<summary_threshold_ratio<< "%, ";
  if(cntMode==instCnt)
    outDotFile << "instCnt";
  else
    outDotFile << "cycleCnt";
  outDotFile<<")\""<<endl;

  makeDepNodeTable(node, dep, printMode);
  //printRankAndDepth(); //FAISAL: remove the line
  printLevel(); 
  dfs_dotNode2(node, dep, printMode,nodeDependency,summary_threshold_ratio, fileName); //FAISAL: add node Dependency and topN
  printRankAndDepth(); //FAISAL: add the line for Rank print

#if 0
  outDotFile << "\t N"<<hex<<node<<" [shape=\"box\" label=\"main\"];"<<endl;

  if(node){
    dfs_dotNode2(node->child, dep+1, printMode);
    dfs_dotNode2(node->sibling, dep, printMode);
  }
#endif

  outDotFile << "} "<< endl;

  outDotFile.close();

}


void buildDotFileOfNodeTree_mem2(struct treeNode *node, int nodeDependency, float summary_threshold_ratio, string fileName, string topNodeID)
{

  if(cntMode==instCnt)
    buildDotFileOfNodeTree_mem(node, instCnt, nodeDependency,summary_threshold_ratio, fileName,topNodeID);//FAISAL: add node dependency and topN and topNodeID
  else
    buildDotFileOfNodeTree_mem(node, cycleCnt, nodeDependency,summary_threshold_ratio, fileName,topNodeID);//FAISAL: add node dependency and topN and topNodeID
  
}

