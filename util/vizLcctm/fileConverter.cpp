
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

using namespace std;
typedef unsigned long long int UINT64;
typedef unsigned long long int ADDRINT;
typedef unsigned int UINT32;
typedef unsigned int UINT;
typedef int INT32;

void printHowToUse(int argc, char **argv)
{
  cout << "Usage: fileConverter filename\n";
  cout << "       A old LCCTM file format is converted to new one, whose name is filename.cvt\n";

}

int main(int argc, char **argv)
{
  //cntMode=cycleCnt;
  
  //FAISAL: Checkes for the filename
  if( argc == 1) {
    printHowToUse(argc, argv);
    return 0;	
  }

  string filename = argv[1];
  FILE *fp;
  if((fp=fopen(argv[1], "rb"))==NULL){
    printf("file open error!! %s\n",argv[1]);
    exit(1);
  }

  filename=filename+".cvt";
  const char *lcctm_file_name=filename.c_str();
  FILE *outFile=fopen(lcctm_file_name, "wb");


  char s0[6]="Exana";
  UINT32 a0=0;
  char s1[6]="LCCTM";
  fwrite(&s0, sizeof(char), 6, outFile);
  fwrite(&a0, sizeof(UINT32), 1, outFile);
  fwrite(&s1, sizeof(char), 6, outFile);
  char c;
  while(fread(&c, sizeof(char),1,fp)) {
    //cout<<c;
    fwrite(&c, sizeof(char), 1, outFile);
  }

}
