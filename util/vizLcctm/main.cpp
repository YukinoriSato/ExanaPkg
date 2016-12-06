
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <gvc.h> //FAISAL: ADD the graphviz library 

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
//#include <cstring> //FAISAL: remove the cstring library as it is conflicting with the graphviz
//#include <string>  //FAISAL: remove the string library as it is conflicting with the graphviz
#include <sstream> // FAISAL: Added for substring checking
#include <vector> // FAISAL: Added for substring checking

#include "main.h"
#include "print.h"
#include "file.h"
#include "generateDot.h"

UINT64 totalInst=0;
UINT64 cycle_application=0;


enum cntModeT cntMode;
enum profModeT profMode;


//enum printModeT{instCntMode, cycleCntMode};

// when threshold is 0 then all of nodes are created in the dot file
//#define SUMMARY_THRESHOLD 1.0
//UINT64 summary_threshold=0;

float summary_threshold=0;
float summary_threshold_ratio;

std::ofstream outDotFile;
string outDotFileName;


string exefileName;

//std::ofstream outFileOfProf;

struct gListOfLoops *head_gListOfLoops=NULL;


treeNode *rootNodeOfTree;
int topN=8;

void printHowToUse(int argc, char **argv)
{
  //cout << "argc is = " << argc << argv[1]<< argv[2]<< argv[3]<< argv[4]<< argv[5]<<endl; 
      cout << "Usage: generateDot filename [Flags][Options]\n\n";
      cout << "FLAGS for displaying output:\n";
      cout << "\t -mem \t memory footprint for each node"<<endl;
      cout << "\t -RW  memory read-write byte information"<<endl;
      cout << "\t -ins \t accumulated instructions executed in the node"<<endl;
      cout << "\t -flop \t accumulated floating point instructions executed in the node"<<endl;
      cout << "\t -cycle \t total clock cycles for the node"<<endl;
      cout << "\t -cpi \t Clock cycles Per Instruction for each node"<<endl;
      cout << "\t -bpi \t Bytes Per Instruction for each node"<<endl;
      cout << "\t -bpc \t Bytes Per Clock cycle for each node"<<endl;
      cout << "\t -ibf \t Bytes Per flop for each node"<<endl;
      cout << "\t -dep \t enable data dependence information"<<endl;
      cout << "OPTIONS:\n";
      cout << "\t-debug    Output show_tree_dfs.out which contains all of data in the input file"<<endl;
      cout << "\t-thr t    Threshold  set the threshold value"<<endl;
      cout << "\t     t is percentage within a range from 0 to 100"<<endl;
      cout << "\n\t-topN n   Top Node   set the top number of nodes to display"<<endl;
      cout<<  "\t     n is integer and n>0, default value for topN: "<<topN<<endl;
      cout << "\n\t-exefile filename"<<endl;
      cout << "\t     Specify the filename of the profiled binary executable"<<endl;
      cout<<  "\t     The filename will help to find inlined functions"<<endl;
      cout << "\n\t-nodeID i\n";
      cout<<  "\t     i is ID number of nodes, which can be found in dot file."<<endl;
      cout<<  "\t     If you specify NodeID, the node is displayed at the top as a root node\n"<<endl;
      cout << "\nFOR EXAMPLE: \n";
      cout << "\t     The execution of the generateDot file can be done as follows\n";
      cout << "\t     ./generateDot lcctm.out -mem -cpi -topN 5"; 	
      cout << "\n\n";
      

}

void printError(int argc, char **argv)
{
  printHowToUse(argc, argv);
  exit(1);	
}

bool thrFlag=0,topNFlag=0;

int main(int argc, char **argv)
{
  //cntMode=cycleCnt;
  
  //FAISAL: Checkes for the filename
  if( argc == 1) {
    printHowToUse(argc, argv);
    return 0;	
  }
  
  //FAISAL: Reads the filename 
  string filename = argv[1];
   
  //Code to find the working directory folder...
  string working_dir = filename.substr(0,filename.find_last_of("/\\") + 1);
  std::cout<<"working directory: "<<working_dir<<"\n";
 
  const char *lcctm_file_name=filename.c_str();
  FILE *fp=fopen(lcctm_file_name, "rb");

  string NODEID = "0";

  for(int i=2;i<argc;i++){
    string str = argv[i];
    //cout<<"\'"<<str<<"\'"<<endl;
    if (!str.compare("-thr")){
      if(argc>i+1){
	i++;
	thrFlag=1;
	summary_threshold_ratio=atof(argv[i]);
      }
      else printError(argc, argv);
    }
    else if (!str.compare("-topN")){
      if(argc>i+1){
	i++;
	topNFlag=1;
	topN = atoi(argv[i]);     
      }
      else printError(argc, argv);
    }
    else if (!str.compare("-nodeID")){
      if(argc>i+1){
	i++;
	NODEID=argv[i];
      }
      else printError(argc, argv);
    }
    else if (!str.compare("-exefile")){
      if(argc>i+1){
	i++;
	exefileName=argv[i];
	cout<<"exefilename option: "<<exefileName<<endl;
      }
      else printError(argc, argv);
    }
    else if (!str.compare("-debug")){
      
    }
    else{
 
        std::string delimiter = " ";

        std::vector<std::string> outputArr;
        size_t pos = 0;
        std::string token;
        while ((pos = str.find(delimiter)) != std::string::npos) {
            token = str.substr(0, pos);
            str.erase(0, pos + delimiter.length());
            outputArr.push_back(token);
        }
        outputArr.push_back(str);

        // Printing Array to see the results
        std::cout<<"============================[SELECTED FLAGS]===========================\n";
        for ( int j=0;j<outputArr.size();j++){
	  std::cout<<outputArr[j]<<" ";
                 
	  if (outputArr[j].find("-mem") != std::string::npos ) memFlag=1;
	  else if (outputArr[j].find("-ins") != std::string::npos ) insFlag=1;
	  else if (outputArr[j].find("-cycle") != std::string::npos ) cycleFlag=1;
	  else if (outputArr[j].find("-cpi") != std::string::npos ) CPIFlag=1;
	  else if (outputArr[j].find("-bpi") != std::string::npos ) BPIFlag=1;
	  else if (outputArr[j].find("-bpc") != std::string::npos ) BPCFlag=1;
	  else if (outputArr[j].find("-ibf") != std::string::npos ) IBFFlag=1;
	  else if (outputArr[j].find("-dep") != std::string::npos ) depFlag=1;
	  else if (outputArr[j].find("-flop") != std::string::npos ) flopFlag=1;
	  else if (outputArr[j].find("-RW") != std::string::npos ) memRWFlag=1; 
	  else if (outputArr[j].find("-eqTHR") != std::string::npos ) ratioFlag=1;
	  else if (outputArr[j].find("-wsRead") != std::string::npos ) wsReadFlag=1;
	  else if (outputArr[j].find("-wsWrite") != std::string::npos ) wsWriteFlag=1;
	  else if (outputArr[j].find("-wsPage") != std::string::npos ) wsPageFlag=1;
	  else  {
	    cout<<"Error:   Undefiled option "<<str<<endl;
	    printError(argc, argv);
	  }
	  
        }
        std::cout<<"\n=======================================================================\n";
      
#if 0
    else if (!str.compare("-mem")) memFlag=1;
    else if (!str.compare("-ins")) insFlag=1;
    else if (!str.compare("-cycle")) cycleFlag=1;
    else if (!str.compare("-cpi")) CPIFlag=1;
    else if (!str.compare("-bpi")) BPIFlag=1;
    else if (!str.compare("-bpc")) BPCFlag=1;
    else if (!str.compare("-ibf")) IBFFlag=1;
    else if (!str.compare("-dep")) depFlag=1;
    else if (!str.compare("-flop")) flopFlag=1;
    else{
      cout<<"Error:   Undefiled option "<<str<<endl;
      printError(argc, argv);
    }
#endif
    }
  }

  if(topNFlag&&thrFlag){
    cout<<"Error:   we cannot specifiy both of -thr & topN option at the same time"<<endl;
    exit(1);
  }

 //cout << "nodeid" << NODEID;    
  treeNode *topNode=readLCCTM(fp, NODEID);//FAISAL: NODEID is included for node selection....
  if(topNode==NULL){
    topNode=rootNodeOfTree;
  }
  printNode(topNode);
  //cout<<"topNodeID="<<NODEID<<endl;

  // Delete the sibling node of topNode to present only its successors.
  topNode->sibling=NULL;

  if(!feof(fp)){
    struct gListOfLoops *new_gListOfloop=read_gListOfLoop(fp);
    //cerr<<"old ptr of gListOfloop "<<hex<<head_gListOfLoops<<endl;
    head_gListOfLoops=new_gListOfloop;
    //cerr<<"new ptr of gListOfloop "<<hex<<head_gListOfLoops<<endl;
    //printStaticLoopInfo();
  }

  fclose(fp);

  if(cntMode==cycleCnt)
    cout<<"Execution Mode: cycleCnt mode"<<endl;
  else
    cout<<"Execution Mode: instCnt mode"<<endl;

  
  outDotFileName= working_dir+std::string("lcctm.dot");
  //outFileOfProf<<"outDotFileName  "<<outDotFileName<<endl;
  outDotFile.open(outDotFileName.c_str());



  count_dynamicNode(topNode);
  init_accumStat(topNode);

  calc_accumInstCnt(topNode);

  calc_accumFlopCnt(topNode);
  calc_accumMemAccessByte(topNode);
  calc_accumMemAccessByteR(topNode);
  calc_accumMemAccessByteW(topNode);
  calc_accumCycleCnt(topNode);


  totalInst=numDyInst;
  cycle_application=numDyCycle;

  // cout<<"totalInst "<<dec<<totalInst<<endl;
  // cout<<"cycle_application "<<dec<<numDyCycle<<endl;

  //outFileOfProf.open("show_tree_dfs.out");
  //cout<<"topNode "<<hex<<topNode<<endl;
  //show_tree_dfs(topNode, 0);


  if(cntMode==instCnt){
    if(thrFlag){
      summary_threshold = summary_threshold_ratio* totalInst/100;
      //cout<<"summary_threshold is  "<<dec<<summary_threshold<<"  ("<<summary_threshold_ratio<<" %)"<<endl;
    }
    else{
      makeOrderedListOfTimeCnt(topNode, topN, cntMode);
      summary_threshold=getMinimumAccumTimeCntFromN(topN, cntMode);
      summary_threshold_ratio=(float)summary_threshold*100/totalInst;
      //cout<<"topN summary_threshold is  "<<dec<<summary_threshold<<"  ("<<summary_threshold_ratio<<" %)"<<endl;
    }
  }
  else{
    if(thrFlag){
      summary_threshold = summary_threshold_ratio* cycle_application/100;
      //cout<<"summary_threshold is  "<<dec<<summary_threshold<<"  ("<<summary_threshold_ratio<<" %)"<<endl;
    }
    else{
      makeOrderedListOfTimeCnt(topNode, topN, cntMode);
      summary_threshold=getMinimumAccumTimeCntFromN(topN, cntMode);
      summary_threshold_ratio=(float)summary_threshold*100/cycle_application;
      //cout<<"topN summary_threshold is  "<<dec<<summary_threshold<<"  ("<<summary_threshold_ratio<<" %)"<<endl;
    }
  }


  
  //cout<<"accumInstCnt "<<dec<<topNode->statAccum->accumInstCnt<<endl;
  //cout<<"exefilename "<<exefileName<<endl;
  topNode->parent=NULL;
  topNode->sibling=NULL;
  buildDotFileOfNodeTree_mem2(topNode,depFlag,summary_threshold_ratio,filename,NODEID);//Faisal: add the dependency extract option and add the topNodeID
    

  ///

  //outFileOfProf<<"DOT FILE GENERATION DONE"<<endl;
  
  //FAISAL: Generation of png file from the DOT file using the graphviz library
  GVC_t *gvc;
  Agraph_t *g;
  FILE *fp1;
  gvc = gvContext();
  
  fp1 = fopen(outDotFileName.c_str(), "r");
  //outFileOfProf<<fp1<<endl;
  g = agread(fp1, 0);
  gvLayout(gvc, g, "dot");
  //sleep(2);
  string outPngFileName = working_dir + std::string("lcctm.png");
  string outMapFileName = working_dir + std::string("out.map");
  
  gvRenderFilename(gvc, g, "png", outPngFileName.c_str());
  gvRenderFilename(gvc, g, "cmap", outMapFileName.c_str());
  gvFreeLayout(gvc, g);
  agclose(g);
  //return (gvFreeContext(gvc));
  fclose(fp1);
  //....
  

  //////
  //outFileOfProf<<"Fini OK"<<endl;
 
  ////ADDED by FAISAL 24/10/2014
  return cntMode; //'0' for cycleCnt mode & '1' for instCnt mode

}
