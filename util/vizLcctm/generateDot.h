////////////////////////////////////////////////////
//*****    generateDot.h   ********************///
////////////////////////////////////////////////////

#ifndef _generateDot_H_
#define _generateDot_H_

void count_dynamicNode(struct treeNode *node);
extern UINT64 numDyInst;
extern UINT64 numDyCycle;

extern UINT64 numDyLoopNode;
extern UINT64 numDyIrrLoopNode;
extern UINT64 numDyRtnNode;
extern UINT64 numDyMemRead;
extern UINT64 numDyMemWrite;
extern UINT64 numDyDepEdge;
extern UINT64 numDyRecursion;
extern UINT64 totalInst;
extern UINT64 cycle_application;

extern bool memFlag,insFlag,cycleFlag,CPIFlag,BPIFlag,BPCFlag,flopFlag,IBFFlag,depFlag,ratioFlag,memRWFlag, wsReadFlag, wsWriteFlag, wsPageFlag;

void makeOrderedListOfTimeCnt(struct treeNode *node, int topN,  cntModeT printMode);
UINT64 getMinimumAccumTimeCntFromN(int n, cntModeT printMode);
void buildDotFileOfNodeTree_mem(struct treeNode *node, enum cntModeT printMode,int nodeDependency,float summary_threshold_ratio, string fileName, string topNodeID); //FAISAL: add node dependency and topNodeID
void makeOrderedListOfInstCnt(struct treeNode *node, int topN);
UINT64 getMinimumAccumInstCntFromN(int n);
void buildDotFileOfNodeTree_mem2(struct treeNode *node,int nodeDependency, float summary_threshold_ratio, string fileName, string topNodeID); //FAISAL: add node dependency and topN and topNodeID

UINT64 calc_accumInstCnt(struct treeNode *node);
UINT64 calc_accumCycleCnt(struct treeNode *node);
void init_accumStat(struct treeNode *node);
UINT64 calc_accumFlopCnt(struct treeNode *node);
UINT64 calc_accumMemAccessByte(struct treeNode *node);

UINT64 calc_accumMemAccessByteR(struct treeNode *node);
UINT64 calc_accumMemAccessByteW(struct treeNode *node);


#endif
