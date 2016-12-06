
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

#include <sstream>

#include "main.h"
#include "print.h"
#include "file.h"

void outputNode(struct treeNode *node, FILE *fp)
{

  if (node == NULL) return;

  //FILE *fp=outFileOfLCCTM;

  //fprintf(stderr, "nodeID %d\n",node->nodeID);
  fwrite(&node->nodeID, sizeof(INT32), 1, fp);
  fwrite(&node->type, sizeof(enum node_type), 1, fp);
  int size=(*node->rtnName).size();
  fwrite(&size,sizeof(int),1,fp);
  const char *c=(*node->rtnName).c_str();
  fwrite(c,sizeof(char),size,fp);
  char ct='\0';
  fwrite(&ct,sizeof(char),1,fp);
  fwrite(&node->rtnID, sizeof(int), 1, fp);
  fwrite(&node->loopID, sizeof(int), 1, fp);
  fwrite(&node->rtnTopAddr, sizeof(ADDRINT), 1, fp);
  fwrite(&node->loopType, sizeof(enum nodeTypeE), 1, fp);

  //cerr<<"rtnTopAddr "<<hex<<node->rtnTopAddr<<endl;

  //struct treeNodeStat *stat;
  struct treeNodeStat *s=node->stat;
  //fprintf(stderr, "stat %p\n",s);

  fwrite(&s->instCnt, sizeof(UINT64), 1, fp);
  fwrite(&s->cycleCnt, sizeof(UINT64), 1, fp);
  fwrite(&s->FlopCnt, sizeof(UINT64), 1, fp);
  fwrite(&s->memAccessByte, sizeof(UINT64), 1, fp);
  fwrite(&s->memReadByte, sizeof(UINT64), 1, fp);
  fwrite(&s->memWrByte, sizeof(UINT64), 1, fp);
  fwrite(&s->n_appearance, sizeof(UINT64), 1, fp);

  //cerr<<"stat "<<dec<<s->instCnt<<" "<<s->accumInstCnt<<" "<<s->cycleCnt<<" "<<s->accumCycleCnt<<" "<<s->memReadCnt<<" "<<s->memWrCnt<<" "<<s->n_appearance<<endl;


  int nullNode=-1;
  if(node->child)
    fwrite(&node->child->nodeID, sizeof(INT32), 1, fp);
  else
    fwrite(&nullNode, sizeof(INT32), 1, fp);

  if(node->sibling)
    fwrite(&node->sibling->nodeID, sizeof(INT32), 1, fp);
  else
    fwrite(&nullNode, sizeof(INT32), 1, fp);

  if(node->parent)
    fwrite(&node->parent->nodeID, sizeof(INT32), 1, fp);
  else
    fwrite(&nullNode, sizeof(INT32), 1, fp);

#if 0
  printNode2(node, cerr);
  fprintf(stderr, " output id %d %d %d %d\n",node->nodeID,node->child? node->child->nodeID:-1,node->sibling?node->sibling->nodeID:-1,node->parent?node->parent->nodeID:-1);
#endif

  //struct treeNode *child;
  //struct treeNode *sibling;
  //struct treeNode *parent;

  if(node->type==loop){

    //struct loopTripInfoElem *loopTripInfo;
    struct loopTripInfoElem *t=node->loopTripInfo;
    //fprintf(stderr, "loopTripInfo %p\n",t);
    fwrite(&t->tripCnt, sizeof(UINT64), 1, fp);
    fwrite(&t->max_tripCnt, sizeof(UINT64), 1, fp);
    fwrite(&t->min_tripCnt, sizeof(UINT64), 1, fp);
    fwrite(&t->sum_tripCnt, sizeof(UINT64), 1, fp);
    //cerr<<"loopTripInfo "<<dec<<node->loopTripInfo->sum_tripCnt<<endl;
  }
  //fprintf(stderr, "hogehoge\n");
  

  struct depNodeListElem *depList=node->depNodeList;
  UINT32 depCnt=0;
  while(depList){
    //cerr<<"list: depList->node "<<hex<<depList->node<<endl;
    depCnt++;
    depList=depList->next;
  }
  //fprintf(stderr, "depCnt %d\n",depCnt);
  fwrite(&depCnt, sizeof(UINT32), 1, fp);

  depList=node->depNodeList;
  
  for(UINT i=0;i<depCnt;i++){
    //cerr<<"i="<<dec<<i<<endl;
    //cerr<<"depList->node "<<hex<<depList->node<<endl;
    int depNodeID= (depList->node? depList->node->nodeID:nullNode);
    //printNode2(depList->node);
    //cerr<<"dep nodeID "<<dec<<depNodeID<<endl;
    fwrite(&depNodeID, sizeof(INT32), 1, fp);
    fwrite(&depList->depCnt, sizeof(UINT64), 1, fp);
    fwrite(&depList->selfInterAppearDepCnt, sizeof(UINT64), 1, fp);
    fwrite(&depList->selfInterAppearDepDistSum, sizeof(UINT64), 1, fp);
    fwrite(&depList->selfInterItrDepCnt, sizeof(UINT64), 1, fp);
    fwrite(&depList->selfInterItrDepDistSum, sizeof(UINT64), 1, fp);
    depList=depList->next;
  }

  //cerr<<"ok"<<endl;

  struct treeNodeListElem *list=node->recNodeList;
  UINT32 recCnt=0;
  while(list){
    //outDotFile << "\t N"<<hex<<list->node<<" -> N"<<node<<" [style=bold, dir=back, color=green] ;"<<endl;
    recCnt++;
    list=list->next;
  }
  //fprintf(stderr, "recCnt %d\n",recCnt);
  fwrite(&recCnt, sizeof(UINT32), 1, fp);
  list=node->recNodeList;
  for(UINT i=0;i<recCnt;i++){
    fwrite(&list->node->nodeID, sizeof(INT32), 1, fp);
    list=list->next;
  }

}





void output_treeNode_dfs(struct treeNode *node, FILE *fp)
{
  if (node == NULL) return;
  //if(node->stat->accumInstCnt>=summary_threshold)
  //printNode(node);
  //fprintf(stderr, "Hi node=%p\n",node);
  outputNode(node, fp);
  //cout<<"next "<<hex<<node->child<<" "<<node->sibling<<endl;
  output_treeNode_dfs(node->child, fp);
  output_treeNode_dfs(node->sibling, fp);
}

struct nodeIdListE{
  int nodeID;
  struct nodeIdListE *next;
};

struct treeNodeMapTableE{
  struct treeNode *nodeAdr;
  int nodeID;
  int childID;
  int siblingID;
  int parentID;
  struct nodeIdListE *depNodeIdList;
  struct nodeIdListE *recNodeIdList;
};

struct nodeIdListE *addNodeID_to_List(struct nodeIdListE *idList, int nodeID)
{
  struct nodeIdListE *top=idList;

  if(idList==NULL){
    idList=new nodeIdListE;
    idList->nodeID=nodeID;
    idList->next=NULL;
    //cerr<<"addNodeID(new) "<<hex<<idList<<endl;
    return idList;
  }
  else{
    while(idList){ 
      //cerr<<"check "<<hex<<idList<<" "<<endl;
      if(idList->next)
	idList=idList->next;
      else
	break;
    }

    idList->next=new nodeIdListE;
    idList=idList->next;
    idList->nodeID=nodeID;
    idList->next=NULL;
    //cerr<<"addNodeID "<<hex<<idList<<endl;
  }
  return top;
}


void output_gListOfLoop(FILE *fp)
{
  //FILE *fp=outFileOfLCCTM;


  struct gListOfLoops *node=head_gListOfLoops;
  int num_gListOfLoop=0;

  while(node){
    num_gListOfLoop++;
    node=node->next;
  }

  string *signiture=new string("gListOfLoop");
  const char *c=(*signiture).c_str();
  //int size=(*signiture).size();  
  //cout<<"12 ???  size="<<dec<<size<<endl;
  fwrite(c,sizeof(char),12,fp);

  //cerr<<"num "<<num_gListOfLoop<<endl;
  fwrite(&num_gListOfLoop, sizeof(int), 1, fp);

  node=head_gListOfLoops;
  for(int i=0;i<num_gListOfLoop;i++){
    fwrite(&node->loopID, sizeof(int), 1, fp);
    fwrite(&node->bblID, sizeof(int), 1, fp);
    int s1=node->rtnName.size();
    const char *c1=(node->rtnName).c_str();
    fwrite(&s1,sizeof(int),1,fp);
    fwrite(c1,sizeof(char),s1+1,fp);
  
    fwrite(&node->instAdr, sizeof(ADDRINT), 1, fp);
    fwrite(&node->srcLine, sizeof(int), 1, fp);

    int s2=(*node->fileName).size();
    const char *c2=(*(node->fileName)).c_str();
    fwrite(&s2,sizeof(int),1,fp);
    fwrite(c2,sizeof(char),s2+1,fp);

    //cerr<<"Out: gListOfNode "<<dec<<i<<" "<<node->loopID<<" "<<node->srcLine<<" "<<node->rtnName<<" "<<*(node->fileName)<<endl;

    node=node->next;

  }
  return;

}


void dumpNodeMapTable(struct treeNodeMapTableE *t, UINT64 num)
{
  cerr<<"dumpNodeMapTable"<<endl;
  for(UINT i=0;i<num;i++){
    fprintf(stderr, "  %d %p  id %d %d %d %d %p %p\n",i, t[i].nodeAdr, t[i].nodeID, t[i].childID, t[i].siblingID, t[i].parentID, t[i].depNodeIdList, t[i].recNodeIdList); 
  }
}

int searchNodeID(struct treeNodeMapTableE *t, UINT64 num, int id)
{
  for(UINT i=0;i<num;i++){
    if(t[i].nodeID==id)
      return i;
  }
  cerr<<"Error: cannot find nodeID in NodeMapTable @searchNodeID() request="<<dec<<id<<endl;
  exit(1);
}


void updateTreeNode(struct treeNode *arrayNode, struct treeNodeMapTableE *t, UINT64 num)
{

  //dumpNodeMapTable(t, num);

  for(UINT i=0;i<num;i++){
    struct treeNode *node=&arrayNode[i];
    //fprintf(stderr, "nodeID %d\n",node->nodeID);
    //fprintf(stderr, "treeNodeMapTable %p  id %d %d %d %d %p %p\n", t[i].nodeAdr, t[i].nodeID, t[i].childID, t[i].siblingID, t[i].parentID, t[i].depNodeIdList, t[i].recNodeIdList); 


    node->child=(t[i].childID!=-1?  (t[searchNodeID(t, num, t[i].childID)]).nodeAdr:NULL);
    node->sibling=(t[i].siblingID!=-1?  (t[searchNodeID(t, num, t[i].siblingID)]).nodeAdr:NULL);
    node->parent=(t[i].parentID!=-1?  (t[searchNodeID(t, num, t[i].parentID)]).nodeAdr:NULL);

#if 0
    fprintf(stderr, "node %p  ",node);
    printNode2(node, cerr);
    fprintf(stderr, " output id %d %d %d %d   sibling=%p\n",node->nodeID,node->child? node->child->nodeID:-1,node->sibling?node->sibling->nodeID:-1,node->parent?node->parent->nodeID:-1, node->sibling);
#endif

    //cerr<<"node update "<<hex<<node->child<<" "<<node->sibling<<" "<<node->parent<<" "<<endl;

    struct nodeIdListE *depList=t[i].depNodeIdList;

    struct depNodeListElem *curr1=node->depNodeList;
    while(curr1){
      if(depList==NULL){
	cerr<<"Error: List length inconsistency @updateTreNode()"<<endl;
	exit(1);
      }
      //cerr<<"curr1 "<<hex<<curr1<<"  depNodeID "<<dec<<depList->nodeID<<" "<<endl;
      if(depList->nodeID==-1)curr1->node=NULL;
      else curr1->node=t[searchNodeID(t,num,depList->nodeID)].nodeAdr;
      //cerr<<"depList update "<<hex<<curr1->node<<endl;
      depList=depList->next;
      curr1=curr1->next;
    }

    struct nodeIdListE *recList=t[i].recNodeIdList;
    //cerr<<"currNode "<<hex<<node<<dec<<" "<<node->nodeID<<endl;
    struct treeNodeListElem *curr2=node->recNodeList;
    while(curr2){
      if(recList==NULL){
	cerr<<"Error: List length inconsistency @updateTreNode()"<<endl;
	exit(1);
      }
      //cerr<<"curr2 "<<hex<<curr2<<"  recNodeID "<<dec<<recList->nodeID<<" ";
      curr2->node=t[searchNodeID(t,num,recList->nodeID)].nodeAdr;
      //cerr<<"recList update "<<hex<<curr2->node<<endl;
      recList=recList->next;
      curr2=curr2->next;
    }


  }
}

bool workingSetAnaFlag=0;
struct treeNode* readLCCTM(FILE *fp, string NODEID)//FAISAL: NODEID is included for node selection....
{
  char s0[6],s1[6];
  UINT32 a0;
  fread(&s0, sizeof(char)*6, 1, fp);
  //cout<<s0<<endl;

  if(strncmp(s0, "Exana", 6)){
    cout<<"ERROR of the input file format:  this file is not the Exana format"<<endl;;
    exit(1);
  }
  fread(&a0, sizeof(UINT32), 1, fp);
  fread(&s1, sizeof(char)*6, 1, fp);
  //cout<<s1<<endl;
  if(strncmp(s1, "wsAna", 6)==0){
    workingSetAnaFlag=1;
    //cout<<"wsAna mode"<<endl;
  }
  else if(strncmp(s1, "LCCTM", 6)==0){
    //cout<<"LCCTM mode"<<endl;
  }
  else{
    cout<<"ERROR of the input file format:  this file is not the Exana LCCTM format"<<endl;;
    
    exit(1);
  }

  UINT64 num;
  fread(&num, sizeof(UINT64), 1, fp);
  fread(&cntMode, sizeof(enum cntModeT), 1, fp);
  //cerr<<"num "<<dec<<num<<endl;

  struct treeNodeMapTableE *treeNodeMapTable=new treeNodeMapTableE[num];
  memset(treeNodeMapTable, 0, sizeof(treeNodeMapTableE *)*num);
  struct treeNode *nodeArray=new treeNode[num];
  struct treeNode *topNode=NULL;
  //cout<< "printing of node read:\n"; 
  for(UINT64 i=0;i<num;i++){

    struct treeNode *node=&nodeArray[i];
    fread(&node->nodeID, sizeof(INT32), 1, fp);
    
    treeNodeMapTable[i].nodeID=node->nodeID;
    treeNodeMapTable[i].nodeAdr=node;
    
    //cout<< "node Id:"<<dec<< i<< " "<<treeNodeMapTable[i].nodeID<<"  node Addr:"<<treeNodeMapTable[i].nodeAdr<<"\n";

    if(i==0) rootNodeOfTree=node;

    //FAISAL: NODEID is included for node selection....
    //std::ostringstream os; 
    //string str1;
    //os << treeNodeMapTable[i].nodeAdr;
    //str1 += os.str();
    int tmpID = atoi(NODEID.c_str());
    if( node->nodeID == tmpID)
    {
      topNode=node;
      //cout<<"MATCH FOUND..... nodeID="<<dec<<node->nodeID<<" "<<hex<<node<<endl; 
    }
    
   // cout<< "os.str():"<<str1<<"::"<<NODEID<<endl;
    

    //fprintf(stderr, "node %d\n",node->nodeID);
    //cerr<<"---- New node ---- "<<dec<<node->nodeID<<" "<<hex<<node<<endl;

    fread(&node->type, sizeof(enum node_type), 1, fp);
    //fwrite(&node->rtnName.length,sizeof(int),1,fp);
    int size;
    fread(&size,sizeof(int),1,fp);
    char *c=(char *) malloc(size+1);
    fread(c, sizeof(char),size+1,fp);

    string *ss=new string(c);
    node->rtnName=ss;
    //fread(node->rtnName,sizeof(string),1,fp);

    //cerr<<*(node->rtnName)<<endl;

    fread(&node->rtnID, sizeof(int), 1, fp);
    fread(&node->loopID, sizeof(int), 1, fp);
    fread(&node->rtnTopAddr, sizeof(ADDRINT), 1, fp);
    fread(&node->loopType, sizeof(enum nodeTypeE), 1, fp);

    //cerr<<"rtnTopAddr "<<hex<<node->rtnTopAddr<<endl;

    //struct treeNodeStat *stat;
    struct treeNodeStat *s=node->stat=new treeNodeStat;
    //fprintf(stderr, "stat %p\n",s);

    fread(&s->instCnt, sizeof(UINT64), 1, fp);
    //fread(&s->accumInstCnt, sizeof(UINT64), 1, fp);
    fread(&s->cycleCnt, sizeof(UINT64), 1, fp);
    //fread(&s->accumCycleCnt, sizeof(UINT64), 1, fp);
    fread(&s->FlopCnt, sizeof(UINT64), 1, fp);
    fread(&s->memAccessByte, sizeof(UINT64), 1, fp);

    fread(&s->memReadByte, sizeof(UINT64), 1, fp);
    fread(&s->memWrByte, sizeof(UINT64), 1, fp);
    fread(&s->n_appearance, sizeof(UINT64), 1, fp);


    if(workingSetAnaFlag){
      struct workingSetInfoElem *a=&(node->workingSetInfo);
      fread(&a->maxCntR, sizeof(UINT64), 1, fp);
      fread(&a->minCntR, sizeof(UINT64), 1, fp);
      fread(&a->sumR, sizeof(UINT64), 1, fp);
      fread(&a->maxCntW, sizeof(UINT64), 1, fp);
      fread(&a->minCntW, sizeof(UINT64), 1, fp);
      fread(&a->sumW, sizeof(UINT64), 1, fp);
      fread(&a->maxCntRW, sizeof(UINT64), 1, fp);
      fread(&a->minCntRW, sizeof(UINT64), 1, fp);
      fread(&a->sumRW, sizeof(UINT64), 1, fp);
    }
    //cerr<<"stat "<<dec<<s->instCnt<<" "<<s->accumInstCnt<<" "<<s->cycleCnt<<" "<<s->accumCycleCnt<<" "<<s->memReadCnt<<" "<<s->memWrCnt<<" "<<s->n_appearance<<endl;

    //fprintf(stderr, "node->child id %p\n",node->child);
    //int childID, siblingID, parentID;

    fread(&treeNodeMapTable[i].childID, sizeof(INT32), 1, fp);
    fread(&treeNodeMapTable[i].siblingID, sizeof(INT32), 1, fp);
    fread(&treeNodeMapTable[i].parentID, sizeof(INT32), 1, fp);

    
    //fprintf(stderr, "read node %p  id %d %d %d %d \n", treeNodeMapTable[i].nodeAdr, treeNodeMapTable[i].nodeID, treeNodeMapTable[i].childID, treeNodeMapTable[i].siblingID, treeNodeMapTable[i].parentID); 

    if(node->type==loop){
      //struct loopTripInfoElem *loopTripInfo;
      struct loopTripInfoElem *t=node->loopTripInfo=new loopTripInfoElem;
      //fprintf(stderr, "loopTripInfo %p\n",t);
      fread(&t->tripCnt, sizeof(UINT64), 1, fp);
      fread(&t->max_tripCnt, sizeof(UINT64), 1, fp);
      fread(&t->min_tripCnt, sizeof(UINT64), 1, fp);
      fread(&t->sum_tripCnt, sizeof(UINT64), 1, fp);
      //cerr<<"loopTripInfo "<<dec<<node->loopTripInfo->sum_tripCnt<<endl;

    }
    //fprintf(stderr, "hogehoge\n");


    UINT32 depCnt=0;
    fread(&depCnt, sizeof(UINT32), 1, fp);

    struct depNodeListElem *depList=NULL;

    for(UINT ii=0;ii<depCnt;ii++){

      if(ii==0){
	node->depNodeList=new depNodeListElem;
	depList=node->depNodeList;
      }
      else{
	depList->next=new depNodeListElem;
	depList=depList->next;
      }

      int nodeID;
      fread(&nodeID, sizeof(INT32), 1, fp);
      //cerr<<"depList "<<hex<<depList<<"  nodeID="<<dec<<nodeID<<endl;
      treeNodeMapTable[i].depNodeIdList=addNodeID_to_List(treeNodeMapTable[i].depNodeIdList, nodeID);

      fread(&depList->depCnt, sizeof(UINT64), 1, fp);
      fread(&depList->selfInterAppearDepCnt, sizeof(UINT64), 1, fp);
      fread(&depList->selfInterAppearDepDistSum, sizeof(UINT64), 1, fp);
      fread(&depList->selfInterItrDepCnt, sizeof(UINT64), 1, fp);
      fread(&depList->selfInterItrDepDistSum, sizeof(UINT64), 1, fp);
      depList->next=NULL;
    }

    depList=node->depNodeList;

    /*
    while(depList){
      cerr<<"depList @ "<<hex<<depList<<" "<<depList->next<<endl;
      depList=depList->next;
    }
    */

    struct treeNodeListElem *list=NULL;

    UINT32 recCnt=0;
    fread(&recCnt, sizeof(UINT32), 1, fp);

    //fprintf(stderr, "recCnt %d\n",recCnt);

    for(UINT ii=0;ii<recCnt;ii++){
      if(ii==0){
	node->recNodeList=new treeNodeListElem;
	list=node->recNodeList;
      }
      else{
	list->next=new treeNodeListElem;
	list=list->next;
      }

      int nodeID;
      fread(&nodeID, sizeof(INT32), 1, fp);
      treeNodeMapTable[i].recNodeIdList=addNodeID_to_List(treeNodeMapTable[i].recNodeIdList, nodeID);
      //cerr<<*node->rtnName<<" currID "<<dec<<node->nodeID<<" recNodeList nodeID="<<dec<<nodeID<<endl;
      list->next=NULL;
    }


  }

  updateTreeNode(nodeArray, treeNodeMapTable, num);

  return topNode;
}


struct gListOfLoops *read_gListOfLoop(FILE *fp)
{
  char c[12];
  fread(c,sizeof(char),12,fp);
  if(strncmp(c,"gListOfLoop",12)!=0){
    cout<<"ERROR of the input file @ read_gListOfLoop()";
    exit(1);
  }
  //cerr<<c<<endl;
  int num_gListOfLoop=0;
  fread(&num_gListOfLoop, sizeof(int), 1, fp);
  //cerr<<"num "<<dec<<num_gListOfLoop<<endl;

  struct gListOfLoops *p=new gListOfLoops[num_gListOfLoop];
  for(int i=0;i<num_gListOfLoop;i++){
    struct gListOfLoops *node=&p[i];  
    fread(&node->loopID, sizeof(int), 1, fp);
    fread(&node->bblID, sizeof(int), 1, fp);


    int size;
    fread(&size,sizeof(int),1,fp);
    char *c=(char *) malloc(size);
    fread(c, sizeof(char),size+1,fp);
    string *ss=new string(c);
    node->rtnName=*ss;
    //fread(node->rtnName,sizeof(string),1,fp);

    //cerr<<(node->rtnName)<<endl;

    fread(&node->instAdr, sizeof(ADDRINT), 1, fp);
    fread(&node->srcLine, sizeof(int), 1, fp);
    int size2;
    fread(&size2,sizeof(int),1,fp);
    char *c2=(char *) malloc(size2);
    fread(c2, sizeof(char),size2+1,fp);
    string *ss2=new string(c2);
    node->fileName=ss2;

    //cerr<<*(node->fileName)<<endl;

    //cerr<<"read "<<dec<<i<<" "<<hex<<node<<" "<<&p[i+1]<< endl;

    

    if(i+1 < num_gListOfLoop)
      node->next=&p[i+1];
    else
      node->next=NULL;

    //cerr<<"IN: gListOfNode "<<dec<<i<<" "<<node->loopID<<" "<<node->srcLine<<" "<<node->rtnName<<" "<<*(node->fileName)<<endl;

  }
  return p;


}
