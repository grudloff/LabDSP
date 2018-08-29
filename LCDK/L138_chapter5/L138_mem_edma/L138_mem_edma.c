// L138_mem_edma.c
//

#include "types.h"
#define EDMA_3CC_ESR *( unsigned int* )( 0x01C01010 )

#define BUFCOUNT 256

uint32_t bufA[BUFCOUNT],bufB[BUFCOUNT],bufC[BUFCOUNT];
uint32_t *EDMA3_PaRAM_ptr;
uint32_t ACNT, BCNT, CCNT;
int i;

void main( void )
{
  for (i=0;i<BUFCOUNT;i++) 
    {bufA[i]=BUFCOUNT-1-i; bufB[i]=i; bufC[i]=i;}
  ACNT = BUFCOUNT*sizeof(int32_t);
  BCNT = 1;
  CCNT = 1;

// initialise PaRAM set #3  
  EDMA3_PaRAM_ptr = (unsigned int *)(0x01C04060);

  *EDMA3_PaRAM_ptr++ = 0x00000000;
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufA;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((BCNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufB;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((ACNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = 0x00000800; 
  *EDMA3_PaRAM_ptr++ = 0x00000000;  
  *EDMA3_PaRAM_ptr++ = 0x00000001; 

//initialise PaRAM set #64
  EDMA3_PaRAM_ptr = (unsigned int *)(0x01C04800);
  
  *EDMA3_PaRAM_ptr++ = 0x00000000;
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufC;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((BCNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufA;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((ACNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = 0x00000820;
  *EDMA3_PaRAM_ptr++ = 0x00000000;  
  *EDMA3_PaRAM_ptr++ = 0x00000001; 

//initialise PaRAM set #65
  EDMA3_PaRAM_ptr = (unsigned int *)(0x01C04820);

  *EDMA3_PaRAM_ptr++ = 0x00000000;
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufB;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((BCNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufC;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((ACNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = 0x00000840; 
  *EDMA3_PaRAM_ptr++ = 0x00000000;  
  *EDMA3_PaRAM_ptr++ = 0x00000001; 

//initialise PaRAM set #66
  EDMA3_PaRAM_ptr = (unsigned int *)(0x01C04840);

  *EDMA3_PaRAM_ptr++ = 0x00000000;
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufA;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((BCNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = (unsigned int)bufB;
  *EDMA3_PaRAM_ptr++ =(int32_t)
         ((((ACNT)<<16) & 0xFFFF0000) | (ACNT & 0x0000FFFF));
  *EDMA3_PaRAM_ptr++ = 0x00000800; 
  *EDMA3_PaRAM_ptr++ = 0x00000000;  
  *EDMA3_PaRAM_ptr++ = 0x00000001; 

  while(1)  
	EDMA_3CC_ESR = 0x00000008; // cause EDMA3 event #3
}
