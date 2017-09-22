#include<stdio.h>

#define N 1000
float a[N][N];

int initA()
{
  int i, j;
  printf("  Init a[N][N]\n");
  for(i = 0; i < N; i++ ) {
    for(j = 0; j < N; j++ ) {
      a[i][j]=0.1*i*j;
    }
  }
}

int func1()
{
  int i,j;
  printf("  func1()\n");

  initA();

  printf("  a doubly nested loop in func1\n");
  for(i = 0; i < N; i++ ) {
    for(j = 0; j < N; j++ ) {
      a[i][j]+=a[i][j]*j/(i+1);
    }
  }

  return 0;

}

int func2()
{
  int i;
  //printf("    A loop in func2\n");
  for(i = 0; i < N; i++ ) {
    a[i][i]+=a[i][i]*i/(i+1);
  }
    return 0;
}

int main()
{
  int i, j;
  printf("\n main(), calling func1\n");
  func1();

  printf(" main(), calling func2, 500 times\n\n");
  for(i = 0; i < 500; i++ ){
    func2();  	
  }

  return 0;
}
       
