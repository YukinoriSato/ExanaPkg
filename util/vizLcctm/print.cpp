
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


#define outFileOfProf cout

void printRtnID();

UINT64 timerCnt=0;
double getTime_sec()
{
  timerCnt++;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

//#define DEBUG_MODE

////////////////////////////////////////////////////////////////////////////

void printNode(struct treeNode *node)
{
  if(node!=NULL){
    if(node->type==loop)
      cout<<dec<<node->loopID<<endl;
    else
      cout<<*(node->rtnName)<<endl;
    //cout<<RTN_FindNameByAddress(node->rtnTopAddr)<<endl;

  }
  else
    cout<<"NULL"<<endl;
}
void printNode(struct treeNode *node, ostream &output)
{
  if(node!=NULL){
    if(node->type==loop)
      output<<dec<<node->loopID<<endl;
    else
      output<<*(node->rtnName)<<endl;
    //output<<RTN_FindNameByAddress(node->rtnTopAddr)<<endl;

  }
  else
    output<<"NULL"<<endl;
}

void printNode2(struct treeNode *node)
{
 if(node!=NULL){
   if(node->type==loop)
     cout<<dec<<node->loopID;
   else
     cout<<*(node->rtnName);
   //cout<<RTN_FindNameByAddress(node->rtnTopAddr)<<endl;
 }
 else
   cout<<"NULL";

}
void printNode2(struct treeNode *node, ostream &out)
{
 if(node!=NULL){
   if(node->type==loop)
     out<<dec<<node->loopID;
   else
     out<<*(node->rtnName);
   //cout<<RTN_FindNameByAddress(node->rtnTopAddr)<<endl;
 }
 else
   out<<"NULL";

}

void printNode3(struct treeNode *node)
{
 if(node!=NULL){
   if(node->type==loop)
     outFileOfProf<<dec<<node->loopID<<endl;
   else
     outFileOfProf<<*(node->rtnName)<<endl;
   //outFileOfProf<<RTN_FindNameByAddress(node->rtnTopAddr)<<endl;
 }
 else
   outFileOfProf<<"NULL";

}

void printNode2outFile(struct treeNode *node)
{
 if(node!=NULL){
   if(node->type==loop){
     //cerr<<"print_loopID"<<endl;
     outFileOfProf<<dec<<node->loopID<<" ";
   }
   else{
     //cerr<<"print_rtnName"<<endl;
     //cerr<<*(node->rtnName)<<endl;
     outFileOfProf<<*(node->rtnName)<<" ";
     //outFileOfProf<<RTN_FindNameByAddress(node->rtnTopAddr)<<" ";
   }
 }
 else
   outFileOfProf<<"NONE";

}

void printNode2cerr(struct treeNode *node)
{
 if(node!=NULL){
   if(node->type==loop)
     cerr<<dec<<node->loopID;
   else{
     cerr<<(node->rtnName)<<" ";cerr<<*(node->rtnName);
     //cerr<<RTN_FindNameByAddress(node->rtnTopAddr);
   }
 }
 else
   cerr<<"NULL";

}
void printDepNodeList(struct treeNode *currNode)
{
  struct depNodeListElem *elem=currNode->depNodeList;
  cout<<"depNodeList   ";
  while(elem){    
    printNode2(elem->node);
    cout<<" ("<<dec<<elem->depCnt<<") ";
    elem=elem->next;
  }
  cout<<endl;
}

void printDepNodeListCerr(struct treeNode *currNode)
{
  struct depNodeListElem *elem=currNode->depNodeList;
  cerr<<"depNodeList   ";
  while(elem){
    cerr<<hex<<elem->node<<"@"; 
    printNode2cerr(elem->node);
    cerr<<" ";
    elem=elem->next;
  }
  cerr<<endl;
}

UINT64 memDepCnt=0;

void printDepNodeListOutFile(struct treeNode *currNode, int depth)
{
  //cout<<"printDepNodeListOutFile"<<endl;;
  struct depNodeListElem *elem=currNode->depNodeList;
  if(elem){
    //cerr<<"printDepNodeListOutFile In elem  depth="<<dec<<depth<<endl;
    for (int i=0;i<depth;i++){
      outFileOfProf<<"  ";
    }
    UINT64 cnt=0;
    outFileOfProf<<"    depNode:   ";
    while(elem){ 

      memDepCnt++;

      //cerr<<"elem->node "<<hex<<elem->node<<endl;
      //printNode2outFile(elem->node);
      //if(elem->node) outFileOfProf<<" "<<dec<<elem->node->stat->memReadCnt;
      //cerr<<"hoge"<<endl;

      if(currNode==elem->node){
	outFileOfProf<<"self";
	//outFileOfProf<<dec<<elem->selfInterAppearDepCnt<<" "<<elem->selfInterItrDepCnt<<" ";
	if(elem->selfInterAppearDepCnt>0){
	  outFileOfProf << "[apper Dist="<<setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right << (float)elem->selfInterAppearDepDistSum/elem->selfInterAppearDepCnt <<" ("<<elem->selfInterAppearDepCnt<<")]";
	}
	if(elem->selfInterItrDepCnt>0){
	  outFileOfProf << "[itr Dist="<<setprecision(1)<< setiosflags(ios::fixed) << setw(4) << right <<(float)elem->selfInterItrDepDistSum/elem->selfInterItrDepCnt <<" ("<<elem->selfInterItrDepCnt<<")]";
	}
	outFileOfProf<<" ";
      }
      else{
	printNode2outFile(elem->node);//outFileOfProf<<"@0x"<<hex<<elem->node;
      }
      outFileOfProf<<"("<<dec<<elem->depCnt<<")    ";      
      cnt+=elem->depCnt;
      elem=elem->next;
    }
    outFileOfProf<<"   ; total="<<dec<<cnt<<endl;
  }

  //cout<<"printDepNodeListOutFile  OK"<<endl;;
}


//////////////////////////////////////////////////////////////////////////



const char *stripPath(const char * path)
{
    const char *file = strrchr(path,'/');
    if (file)
        return file+1;
    else
        return path;
}

const char *stripByUnderbar(const char * path)
{
  static char buf[256];
  const char *file = strrchr(path,'_');
  if (file){
    strncpy(buf, path, file-path);
    return buf;
  }
  else
    return path;
}





//struct treeNode *currLoopTree=NULL;
//struct treeNode *rootNodeOfLoopTree=NULL;


//UINT64 totalInst=0;



struct gListOfLoops *isLoopLineInSrc(int loopID)
{
  struct gListOfLoops *curr_gList=head_gListOfLoops;
  while(curr_gList){
    if(curr_gList->loopID==loopID){
      if((*curr_gList->fileName)!="")
	return curr_gList;
      else 
	return NULL;
    }
    curr_gList=curr_gList->next;
  }
  return NULL;

}

#include <cxxabi.h>
char* demangle(const char *demangle) {
    int status;
    char *a=abi::__cxa_demangle(demangle, 0, 0, &status);
    if(status!=0){
      //DPRINT<<"status="<<status<<endl;
      return (char *) demangle;
    }
    //cout<<a<<" len="<<dec<<strlen(a)<<endl;
    for(int i=0;i<(int) strlen(a)-1;i++){
      //cout<<a[i]<<endl;
      if(a[i]=='('){
	a[i]='\0';
	break;
      }
    }    
    //cout<<a<<endl;
    return a;
}

UINT64 instCntInLoop=0;
UINT64 instCntInRtn=0;

void displayNode(struct treeNode *node, int depth, enum cntModeT printMode)
{

  int i;
  for (i=0;i<depth;i++)
  {
    outFileOfProf<<" .";
  }
  //outFileOfProf<<" ["<<dec<<node->nodeID<<"]";

  //string rtnName=RTN_FindNameByAddress(node->rtnTopAddr);
  //string rtnName=*(node->rtnName);
  char *rtnName=demangle((*(node->rtnName)).c_str());
  //cout<<hex<<node->rtnTopAddr<<" "<<rtnName<<endl;

  UINT64 cnt=0,accumCnt=0,totalCnt=0;
  if(printMode==instCnt){
    cnt=node->stat->instCnt;
    accumCnt=node->statAccum->accumInstCnt;;
    totalCnt=totalInst;
  }
  else{
    cnt=node->stat->cycleCnt;
    accumCnt=node->statAccum->accumCycleCnt;
    totalCnt=cycle_application;
  }


  char buf[1024];
  if(node->type==loop){

    instCntInLoop+=cnt;

    struct gListOfLoops *gListPtr=isLoopLineInSrc(node->loopID);
    if(gListPtr){
      //outFileOfProf<<" + "<<dec<<node->loopID<<"  ("<<*gListPtr->fileName<<":"<<gListPtr->srcLine<<")  "<<dec<<node->statAccum->accumInstCnt;<<" "<<node->stat->instCnt;
      outFileOfProf<<" + "<<dec<<node->loopID<<"  (@0x"<<hex<<gListPtr->instAdr<<")  "<<dec<<accumCnt<<" "<<cnt;
    }
    else
      outFileOfProf<<" + "<<dec<<node->loopID<<"  ("<< rtnName <<")  "<<dec<<accumCnt<<" "<<cnt;

    sprintf(buf, "  %.2f %s (%.2f %s)", (float)(accumCnt)*100/totalCnt,"%", (float)(cnt)*100/totalCnt,"%");
    outFileOfProf<<buf;

    if(printMode==instCnt){
      UINT64 flop=node->statAccum->accumFlopCnt;
      if(flop)
	outFileOfProf<<"   iBF="<<setw(5)<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop<<" "<<flop<<"/"<<node->statAccum->accumMemAccessByte;
      else
	outFileOfProf<<"   iBF=None, mem="<<node->statAccum->accumMemAccessByte;

      if(node->stat->FlopCnt)
	outFileOfProf<<" ("<<setprecision(2)<<node->stat->memAccessByte/(float)node->stat->FlopCnt<<")";
      else{
	if(flop)
	  outFileOfProf<<" (None)";
	else ;
      //outFileOfProf<<" "<<node->statAccum->accumFlopCnt<<" "<<node->statAccum->accumMemAccessByte<<" "<<node->stat->FlopCnt<<" "<<node->stat->memAccessByte;
      
      }
    }

    if(node->loopType==irreducible)
      outFileOfProf<<"    |irreducible|";
    struct loopTripInfoElem *elem=node->loopTripInfo;
    sprintf(buf, "  Avg. itr cnt:  %.2f  ", elem->sum_tripCnt/(float) node->stat->n_appearance);
    outFileOfProf<<buf<<"max/min: "<<elem->max_tripCnt<<"/"<<elem->min_tripCnt<<"  #appear: "<<node->stat->n_appearance;
    if((node->stat->memReadByte!=0) && (node->stat->memWrByte!=0)){
      //outFileOfProf<<"  mem R="<<dec<<node->stat->memReadByte<<"  W="<< node->stat->memWrByte;
      outFileOfProf<<"  mem R="<<dec<<node->statAccum->accumMemAccessByteR<<" ("<<node->stat->memReadByte<<")  W="<< node->statAccum->accumMemAccessByteW<<" ("<<node->stat->memWrByte<<")";
    //outFileOfProf<<"  memReadByte "<<dec<<node->memReadByte<<", memWrByte "<<dec<<node->memWrByte;
    }

    if(workingSetAnaFlag){
      struct workingSetInfoElem *a=&(node->workingSetInfo);
      outFileOfProf<<"  WS R avg="<<dec<<a->sumR/(float) node->stat->n_appearance<<" "<<a->maxCntR<<" "<<a->minCntR<<",  W avg="<<dec<<a->sumW/(float) node->stat->n_appearance<<" "<<a->maxCntW<<" "<<a->minCntW<<",  RW avg="<<dec<<a->sumRW/(float) node->stat->n_appearance<<" "<<a->maxCntRW<<" "<<a->minCntRW;
    }

    outFileOfProf<<endl;
    //cout<<" + "<<dec<<node->loopID<<endl;
  }
  else{
    instCntInRtn+=cnt;
    //cerr<<dec<<instCntInRtn<<endl;
    //outFileOfProf<<" + "<<dec<<rtnName<<"@"<<hex<<node<<"  "<<dec<<accumCnt<<" "<<cnt;
    outFileOfProf<<" + "<<dec<<rtnName<<" (@0x"<<hex<<node->rtnTopAddr<<")  "<<dec<<accumCnt<<" "<<cnt;

    //outFileOfProf<<" + "<<dec<<*(node->rtnName)<<"  "<<dec<<node->statAccum->accumInstCnt;<<" "<<node->stat->instCnt;
    sprintf(buf, "  %.2f %s (%.2f %s)", (float)(accumCnt)*100/totalCnt,"%", (float)(cnt)*100/totalCnt,"%");
    //cerr<<buf<<endl;
    outFileOfProf<<buf;  

    if(printMode==instCnt){
      UINT64 flop=node->statAccum->accumFlopCnt;
      if(flop)
	outFileOfProf<<"   iBF="<<setw(5)<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop;
      else
	outFileOfProf<<"   iBF=None";

      if(node->stat->FlopCnt)
	outFileOfProf<<" ("<<setprecision(2)<<node->stat->memAccessByte/(float)node->stat->FlopCnt<<")";
      else{
	if(flop)
	  outFileOfProf<<" (None)";
	else ;
      //outFileOfProf<<" "<<node->statAccum->accumFlopCnt<<" "<<node->statAccum->accumMemAccessByte<<" "<<node->stat->FlopCnt<<" "<<node->stat->memAccessByte;
      
      }
    }

    outFileOfProf<<" #appear: "<<node->stat->n_appearance;
    if((node->stat->memReadByte!=0) && (node->stat->memWrByte!=0)){
      outFileOfProf<<"  mem R="<<dec<<node->statAccum->accumMemAccessByteR<<" ("<<node->stat->memReadByte<<")  W="<< node->statAccum->accumMemAccessByteW<<" ("<<node->stat->memWrByte<<")";
      //outFileOfProf<<"  memReadByte "<<dec<<node->memReadByte<<", memWrByte "<<dec<<node->memWrByte;
    }

    if(workingSetAnaFlag){
      struct workingSetInfoElem *a=&(node->workingSetInfo);
      outFileOfProf<<"  WS R avg="<<dec<<a->sumR/(float) node->stat->n_appearance<<" "<<a->maxCntR<<" "<<a->minCntR<<",  W avg="<<dec<<a->sumW/(float) node->stat->n_appearance<<" "<<a->maxCntW<<" "<<a->minCntW<<",  RW avg="<<dec<<a->sumRW/(float) node->stat->n_appearance<<" "<<a->maxCntRW<<" "<<a->minCntRW;
    }

    outFileOfProf<<endl;
    //cerr<<" + "<<*(node->rtnName)<<endl;
  }

  struct treeNodeListElem *list=node->recNodeList;
  while(list){
    //outFileOfProf << "   *** Recursion ***   "<<RTN_FindNameByAddress(list->node->rtnTopAddr)<<endl;
    outFileOfProf << "   *** Recursion ***   "<<list->node->rtnTopAddr<<endl;
    list=list->next;
  }


  printDepNodeListOutFile(node, depth);
  //printDepInstListOutFile(node, depth);

  //cerr<<"ok"<<endl;
}

#if 0
void displayNode(struct treeNode *node, int depth, enum cntModeT printMode)
{

  //cout<<"1: "; printNode(node);

  int i;
  for (i=0;i<depth;i++)
  {
    outFileOfProf<<" .";
  }
  //outFileOfProf<<" ["<<dec<<node->nodeID<<"]";

  //string rtnName=RTN_FindNameByAddress(node->rtnTopAddr);
  //string rtnName=*(node->rtnName);
  char *rtnName=demangle((*(node->rtnName)).c_str());
  //cout<<hex<<node->rtnTopAddr<<" "<<rtnName<<endl;

  UINT64 cnt=0,accumCnt=0,totalCnt=0;
  if(printMode==instCnt){
    cnt=node->stat->instCnt;
    accumCnt=node->statAccum->accumInstCnt;
    totalCnt=totalInst;
  }
  else{
    cnt=node->stat->cycleCnt;
    accumCnt=node->statAccum->accumCycleCnt;
    totalCnt=cycle_application;
  }


  char buf[1024];
  if(node->type==loop){

    instCntInLoop+=cnt;

    struct gListOfLoops *gListPtr=isLoopLineInSrc(node->loopID);
    if(gListPtr){
      //outFileOfProf<<" + "<<dec<<node->loopID<<"  ("<<*gListPtr->fileName<<":"<<gListPtr->srcLine<<")  "<<dec<<node->statAccum->accumInstCnt;<<" "<<node->stat->instCnt;
      outFileOfProf<<" + "<<dec<<node->loopID<<"  (@0x"<<hex<<gListPtr->instAdr<<")  "<<dec<<accumCnt<<" "<<cnt;
    }
    else
      outFileOfProf<<" + "<<dec<<node->loopID<<"  ("<< rtnName <<")  "<<dec<<accumCnt<<" "<<cnt;

    sprintf(buf, "  %.2f %s (%.2f %s)", (float)(accumCnt)*100/totalCnt,"%", (float)(cnt)*100/totalCnt,"%");
    outFileOfProf<<buf;

    if(printMode==instCnt){
      UINT64 flop=node->statAccum->accumFlopCnt;
      if(flop)
	outFileOfProf<<"   iBF="<<setw(5)<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop;
      else
	outFileOfProf<<"   iBF=None";

      if(node->stat->FlopCnt)
	outFileOfProf<<" ("<<setprecision(2)<<node->stat->memAccessByte/(float)node->stat->FlopCnt<<")";
      else{
	if(flop)
	  outFileOfProf<<" (None)";
	else ;
      //outFileOfProf<<" "<<node->statAccum->accumFlopCnt<<" "<<node->statAccum->accumMemAccessByte<<" "<<node->stat->FlopCnt<<" "<<node->stat->memAccessByte;
      
      }
    }

    if(node->loopType==irreducible)
      outFileOfProf<<"    |irreducible|";
    struct loopTripInfoElem *elem=node->loopTripInfo;
    sprintf(buf, "  Avg. itr cnt:  %.2f  ", elem->sum_tripCnt/(float) node->stat->n_appearance);
    outFileOfProf<<buf<<"max/min: "<<elem->max_tripCnt<<"/"<<elem->min_tripCnt<<"  #appear: "<<node->stat->n_appearance;
    if((node->stat->memReadCnt!=0) && (node->stat->memWrCnt!=0)){
      outFileOfProf<<"  mem R="<<dec<<node->stat->memReadCnt<<"  W="<< node->stat->memWrCnt;
    //outFileOfProf<<"  memReadCnt "<<dec<<node->memReadCnt<<", memWrCnt "<<dec<<node->memWrCnt;
    }
    outFileOfProf<<endl;
    //cout<<" + "<<dec<<node->loopID<<endl;
  }
  else{
    instCntInRtn+=cnt;
    //cerr<<dec<<instCntInRtn<<endl;
    //outFileOfProf<<" + "<<dec<<rtnName<<"@"<<hex<<node<<"  "<<dec<<accumCnt<<" "<<cnt;
    outFileOfProf<<" + "<<dec<<rtnName<<" (@0x"<<hex<<node->rtnTopAddr<<")  "<<dec<<accumCnt<<" "<<cnt;

    //outFileOfProf<<" + "<<dec<<*(node->rtnName)<<"  "<<dec<<node->statAccum->accumInstCnt;<<" "<<node->stat->instCnt;
    sprintf(buf, "  %.2f %s (%.2f %s)", (float)(accumCnt)*100/totalCnt,"%", (float)(cnt)*100/totalCnt,"%");
    //cerr<<buf<<endl;
    outFileOfProf<<buf;  

    if(printMode==instCnt){
      UINT64 flop=node->statAccum->accumFlopCnt;
      if(flop)
	outFileOfProf<<"   iBF="<<setw(5)<<setprecision(2)<<node->statAccum->accumMemAccessByte/(float)flop;
      else
	outFileOfProf<<"   iBF=None";

      if(node->stat->FlopCnt)
	outFileOfProf<<" ("<<setprecision(2)<<node->stat->memAccessByte/(float)node->stat->FlopCnt<<")";
      else{
	if(flop)
	  outFileOfProf<<" (None)";
	else ;
      //outFileOfProf<<" "<<node->statAccum->accumFlopCnt<<" "<<node->statAccum->accumMemAccessByte<<" "<<node->stat->FlopCnt<<" "<<node->stat->memAccessByte;
      
      }
    }

    outFileOfProf<<" #appear: "<<node->stat->n_appearance;
    if((node->stat->memReadCnt!=0) && (node->stat->memWrCnt!=0)){
      outFileOfProf<<"  mem R="<<dec<<node->stat->memReadCnt<<"  W="<< node->stat->memWrCnt;
      //outFileOfProf<<"  memReadCnt "<<dec<<node->memReadCnt<<", memWrCnt "<<dec<<node->memWrCnt;
    }
    outFileOfProf<<endl;
    //cerr<<" + "<<*(node->rtnName)<<endl;
  }

  struct treeNodeListElem *list=node->recNodeList;
  while(list){
    //outFileOfProf << "   *** Recursion ***   "<<RTN_FindNameByAddress(list->node->rtnTopAddr)<<endl;
    outFileOfProf << "   *** Recursion ***   "<<list->node->rtnTopAddr<<endl;
    list=list->next;
  }


  printDepNodeListOutFile(node, depth);

  //cerr<<"ok"<<endl;
}
#endif


void show_tree_dfs(struct treeNode *node, int depth, cntModeT printMode)
{
  int idepth=depth+1;
  if (node == NULL) return;
  
  //printNode(node);

  //if(node->statAccum){
  //cout<<" accumInstCnt "<<hex<<&(node->statAccum->accumInstCnt)<<endl;

  if(node->statAccum->accumInstCnt>=summary_threshold)
    displayNode(node,depth, printMode);


  //cout<<"next "<<hex<<node->child<<" "<<node->sibling<<endl;
  show_tree_dfs(node->child, idepth, printMode);
  show_tree_dfs(node->sibling, depth, printMode);
}

void show_tree_dfs(struct treeNode *node, int depth)
{
  int idepth=depth+1;
  if (node == NULL) return;

  //printNode2(node); cout<<" statAccum "<<hex<<(node->statAccum)<<endl;
  //if(node->statAccum){
    //cout<<" accumInstCnt "<<hex<<&(node->statAccum->accumInstCnt)<<endl;

  if(node->statAccum->accumInstCnt>=summary_threshold){
    if(cntMode==instCnt)
      displayNode(node,depth, instCnt);
    else
      displayNode(node,depth, cycleCnt);
  }

  show_tree_dfs(node->child, idepth);
  show_tree_dfs(node->sibling, depth);
}

#if 0
void printInnerLoop(int rtnID, int bblID)
{
  PredElem *pred=bblArrayList[rtnID]->loopRegion[bblID];
  //outFileOfProf<<"    loop region of "<<dec<<bblID<<" : ";
  int flag=0;
  while(pred){
    //outFileOfProf<<pred->id<<" ";
    if((!bblArrayList[rtnID]->bblArray[pred->id].nodeType==nonheader) && (pred->id!=bblID))
      flag=1;

    if(pred->next==NULL) break;
    else pred=pred->next;
  }
  if(flag){
    PredElem *pred=bblArrayList[rtnID]->loopRegion[bblID];
    outFileOfProf<<" [Inner:";
    while(pred){
      if((!bblArrayList[rtnID]->bblArray[pred->id].nodeType==nonheader) && (pred->id!=bblID))
	outFileOfProf<<" "<<dec<<pred->id;
      if(pred->next==NULL) break;
      else pred=pred->next;
    }
    outFileOfProf<<"]";
  }
  return;

}

static int n_inst, n_fp, n_avx, n_sse, n_sse2, n_sse3, n_sse4, n_int, n_flop;
static int memAccessSize, memAccessCnt;

void calcStaticInstStatInLoop(int rtnID, int bblID)
{
  memAccessCnt= memAccessSize=0;
  n_inst=n_fp= n_avx= n_sse= n_sse2= n_sse3= n_sse4= n_int=n_flop=0;

  PredElem *pred=bblArrayList[rtnID]->loopRegion[bblID];
  //outFileOfProf<<"\n    loop region of "<<dec<<bblID<<" : ";
  while(pred){
    //outFileOfProf<<pred->id<<" ";
    int j=pred->id;
    memAccessSize+=bblArrayList[rtnID]->bblArray[j].memAccessSize;
    memAccessCnt+=bblArrayList[rtnID]->bblArray[j].memAccessCnt;
    n_inst+=bblArrayList[rtnID]->bblArray[j].instCnt;
    n_int+= bblArrayList[rtnID]->bblArray[j].n_int;
    n_fp+=bblArrayList[rtnID]->bblArray[j].n_fp;
    n_sse+=bblArrayList[rtnID]->bblArray[j].n_sse;
    n_sse2+=bblArrayList[rtnID]->bblArray[j].n_sse2;
    n_sse3+=bblArrayList[rtnID]->bblArray[j].n_sse3;
    n_sse4+=bblArrayList[rtnID]->bblArray[j].n_sse4;
    n_avx+=bblArrayList[rtnID]->bblArray[j].n_avx;
    n_flop+=bblArrayList[rtnID]->bblArray[j].n_flop;

    if(pred->next==NULL) break;
    else pred=pred->next;
  }

  return ;

}
#endif



void printStaticLoopInfo(void)
{
  outFileOfProf<<"\n\n ***** Print static loop info *****\n";
  outFileOfProf<<"                                               memAccess              int|fp|sse|sse2|sse3|sse4|avx\n";
  outFileOfProf<<"loopID  bblID  instAdr          rtnName        Cnt  Size   Flop   B/F           InstMix        LoopType\n";
  struct gListOfLoops *curr_gList=head_gListOfLoops;
  while(curr_gList){
    outFileOfProf<<setw(6)<<dec<<curr_gList->loopID<<" "<<setw(6)<<curr_gList->bblID<<" "<<setw(8)<<hex<<curr_gList->instAdr<<"  "<<setw(20);
    //outFileOfProf<<curr_gList->rtnName<<" ";
    outFileOfProf<<demangle(curr_gList->rtnName.c_str())<<" ";
    //int memAccessCnt=calcStaticMemAccessCntInLoop(rtnID, curr_gList->bblID);
    //int memAccessSize=calcStaticMemAccessSizeInLoop(rtnID, curr_gList->bblID);

#if 0    
    int rtnID=getRtnID(&(curr_gList->rtnName));

    calcStaticInstStatInLoop(rtnID, curr_gList->bblID);
    outFileOfProf<<setw(5)<<dec<<memAccessCnt<<" "<<setw(5)<<memAccessSize<<"  "<<setw(5)<<n_flop<<" ";
    if(n_flop>0)
      outFileOfProf<<setw(5)<<setprecision(2)<<memAccessSize/(float)n_flop;
    else
      outFileOfProf<<"     ";

    outFileOfProf<<"  #ins "<<n_inst<<": "<<n_int<<" "<<n_fp<<" "<< n_sse<<" "<< n_sse2<<" "<< n_sse3<<" "<< n_sse4<<" "<< n_avx<<"  ";


    switch(bblArrayList[rtnID]->bblArray[curr_gList->bblID].nodeType){
    case nonheader:
      outFileOfProf<<"?";
      break;
    case reducible:
      outFileOfProf<<"R";
      break;
    case irreducible:
      outFileOfProf<<"I";
      break;
    case self:
      outFileOfProf<<"S";
      break;
    }
    printInnerLoop(rtnID, curr_gList->bblID);
#endif

    outFileOfProf<<endl;

    curr_gList=curr_gList->next;
  }
}


#define N_PATH 32
void checkMemoryUsage(void)
{
  char path[N_PATH];
  pid_t pid=getpid();

  snprintf(path, N_PATH, "/proc/%d/status",pid);
  //cout<<path<<"   PIN_GetPid " <<dec<<PIN_GetPid()<<endl;;

  /*
  int fd=open(path, O_RDONLY);
  if(fd<0){
    printf("ERROR: failed to open %s\n",path);
  }
  */
  FILE *fp=fopen(path,"r");
  if(fp==NULL){
    printf("ERROR: failed to open %s\n",path);
  }
  
  char buf[256];
  char test[6];
  while ((fgets (buf, 256, fp)) != NULL) {
    //fputs (test, stdout);
    memcpy(test,buf,6);
    if(strcmp(test,"VmPeak")==0)
      fputs (buf, stdout);
    if(strcmp(test,"VmHWM:")==0)
      fputs (buf, stdout);
    if(strcmp(test,"VmRSS:")==0)
      fputs (buf, stdout);
  }

  /*
  snprintf(path, N_PATH, "/proc/%d/statm",pid);
  cout<<path<<endl;
  fp=fopen(path,"r");
  if(fp==NULL){
    printf("ERROR: failed to open %s\n",path);
  }
  
  fgets (buf, 256, fp);
  fputs (buf, stdout);
  */

  fclose(fp);

}

void checkMemoryUsage(ostream &out)
{
  char path[N_PATH];
  pid_t pid=getpid();

  snprintf(path, N_PATH, "/proc/%d/status",pid);
  //outFileOfProf<<path<<"   PIN_GetPid " <<dec<<PIN_GetPid()<<endl;;

  FILE *fp=fopen(path,"r");
  if(fp==NULL){
    out<<"ERROR: failed to open "<<path<<endl;
  }
  
  char buf[256];
  char test[6];
  while ((fgets (buf, 256, fp)) != NULL) {
    //fputs (buf, stdout);    
    memcpy(test,buf,6);  
    if(strncmp(test,"VmPeak",6)==0)
      out<<buf;
      //fputs (buf, out);
    if(strcmp(test,"VmHWM:")==0)
      out<<buf;//fputs (buf, out);
    if(strcmp(test,"VmRSS:")==0)
      out<<buf;//fputs (buf, out);
  }


  fclose(fp);

}


UINT64 calc_numNode(struct treeNode *node)
{

  UINT64 accum=0;
  if (node == NULL) return 0;

  accum++;
  struct treeNode *child=node->child;
  while(child){
    accum+=calc_numNode(child);
    child=child->sibling;
  }

  return accum;

}



