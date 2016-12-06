#include <iostream>
#include<stdio.h>

using namespace std;

#define N_PATH 32
__attribute__((destructor))
void checkMemoryUsage(void)
{
  char path[N_PATH];
  pid_t pid=getpid();

  char filename[256];
  sprintf(filename, "memUse.out.%u", getpid());  
  FILE *outfile=fopen(filename,"w");


  snprintf(path, N_PATH, "/proc/%d/status",pid);
  fprintf(outfile,"%s\n",path);

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
  //fputs (buf, outfile);
  while ((fgets (buf, 256, fp)) != NULL) {
    //fputs (buf, stdout);
    memcpy(test,buf,6);
    if(strncmp(test,"Name:",5)==0)

      fputs (buf, outfile);

    if(strcmp(test,"VmPeak")==0)
      fputs (buf, outfile);
    if(strcmp(test,"VmHWM:")==0)
      fputs (buf, outfile);
    if(strcmp(test,"VmRSS:")==0)
      fputs (buf, outfile);
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
