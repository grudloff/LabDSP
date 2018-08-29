// L138_dotp4.c
//

#include <stdio.h>
#define count 4

int dotp(short *a, short *b, int ncount);

short x[count] = {1,2,3,4};
short y[count] = {0,2,4,6};

main()
{
  int result = 0;
	
  result = dotp(x,y,count);
  printf("result = %d (decimal) \n",result);
}

int dotp(short *a, short *b, int ncount)
{
  int i;
  int sum = 0;
	
  for (i=0 ; i< count ; i++)
  sum += a[i] *b[i];
	
  return(sum);
}
