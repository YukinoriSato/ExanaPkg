////////////////////////////////////////////////////
//*****    main.h   ********************///
////////////////////////////////////////////////////

#ifndef _main_H_
#define _main_H_

using namespace std;
typedef unsigned long long int UINT64;
typedef unsigned long long int ADDRINT;
typedef unsigned int UINT32;
typedef unsigned int UINT;
typedef int INT32;
//#define outFileOfProf cout
extern std::ofstream outFileOfProf;

extern std::ofstream outDotFile;
extern string outDotFileName;

extern string exefileName;

enum profModeT{CCT,LCCT,LCCTM};
enum cntModeT{cycleCnt, instCnt};
//enum printModeT{instCntMode, cycleCntMode};
extern enum profModeT profMode;
extern enum cntModeT cntMode;

extern struct gListOfLoops *head_gListOfLoops;
extern UINT64 totalInst;

extern UINT64 cycle_application;

//extern UINT64 summary_threshold;
extern float summary_threshold;
extern float summary_threshold_ratio;

enum node_type {procedure, loop, root};


struct loopTripInfoElem{
  //UINT64 n_appearance;
  //UINT64 cnt;
  UINT64 tripCnt;
  UINT64 max_tripCnt;
  UINT64 min_tripCnt;
  UINT64 sum_tripCnt;
};
struct workingSetInfoElem{
  //UINT64 n_appearance;
  //UINT64 cnt;
  UINT64 depth;
  UINT64 maxCntR;
  UINT64 minCntR;
  UINT64 sumR;
  UINT64 maxCntW;
  UINT64 minCntW;
  UINT64 sumW;
  UINT64 maxCntRW;
  UINT64 minCntRW;
  UINT64 sumRW;
};
struct treeNodeListElem{
  struct treeNode *node;
  struct treeNodeListElem *next;
};

struct treeNodeStat{
  //Statistics
  UINT64 instCnt;
  UINT64 cycleCnt;
  UINT64 FlopCnt;
  UINT64 memAccessByte;
  UINT64 memReadByte;
  UINT64 memWrByte;
  UINT64 n_appearance;
};

struct treeNodeStatAccum{
  UINT64 accumInstCnt;
  UINT64 accumCycleCnt;
  UINT64 accumFlopCnt;
  UINT64 accumMemAccessByte;
  UINT64 accumMemAccessByteR;
  UINT64 accumMemAccessByteW;
};

struct depNodeListElem{
  struct treeNode *node;
  struct depNodeListElem *next;
  UINT64 depCnt;
  //bool selfInterAppearDepFlag;
  //bool selfInterItrDepFlag;
  UINT64 selfInterAppearDepCnt;
  UINT64 selfInterAppearDepDistSum;
  UINT64 selfInterItrDepCnt;
  UINT64 selfInterItrDepDistSum;
};
  
enum nodeTypeE {nonheader, reducible, irreducible, self};

struct treeNode{
  int nodeID;
  enum node_type type;
  string *rtnName;
  int rtnID;
  int loopID;
  ADDRINT rtnTopAddr;
  struct workingSetInfoElem workingSetInfo;
  struct treeNodeStat *stat;
  struct treeNodeStatAccum *statAccum;
  struct treeNode *child;
  struct treeNode *sibling;
  struct treeNode *parent;
  enum nodeTypeE loopType;  // in staticAna.h
  struct loopTripInfoElem *loopTripInfo;
  struct depNodeListElem *depNodeList;
  struct treeNodeListElem *recNodeList;  // for list of recursive nodes
};

struct loopNodeElemStruct{
  struct treeNode loopNode;
  struct loopTripInfoElem loopTripInfo;
  struct treeNodeStat loopNodeStat;
};

struct callNodeElemStruct{
  struct treeNode callNode;
  struct treeNodeStat callNodeStat;
};

struct gListOfLoops{
  int loopID;
  int bblID;
  string rtnName;
  ADDRINT instAdr;
  int srcLine;
  string *fileName;
  struct gListOfLoops *next;
};

extern treeNode *rootNodeOfTree;
extern bool workingSetAnaFlag;
#endif
