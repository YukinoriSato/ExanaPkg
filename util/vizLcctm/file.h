////////////////////////////////////////////////////
//*****    print.h   ********************///
////////////////////////////////////////////////////

#ifndef _file_H_
#define _file_H_

void output_treeNode_dfs(struct treeNode *node, FILE *fp);
void output_gListOfLoop(FILE *fp);
struct treeNode* readLCCTM(FILE *fp, string NODEID);//FAISAL: add the nodeID selection....
struct gListOfLoops *read_gListOfLoop(FILE *fp);


#endif
