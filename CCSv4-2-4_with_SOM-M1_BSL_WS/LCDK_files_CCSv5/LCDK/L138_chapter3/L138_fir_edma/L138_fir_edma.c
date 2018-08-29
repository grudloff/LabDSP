// L138_fir_edma.c
//

#include "L138_LCDK_aic3106_init.h"
#include "bp1750.cof"

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

float x[N];
float y[(BUFCOUNT/2)];

interrupt void interrupt4(void) // interrupt service routine
{
 switch(EDMA_3CC_IPR)
  {
    case 1:                     // TCC = 0
      procBuffer = PING;        // process ping
	EDMA_3CC_ICR = 0x0001;    // clear EDMA3 IPR bit TCC
      break;
    case 2:                     // TCC = 1
      procBuffer = PONG;        // process pong
	EDMA_3CC_ICR = 0x0002;    // clear EDMA3 IPR bit TCC 
      break;
    default:                    // may have missed an interrupt
	EDMA_3CC_ICR = 0x0003;    // clear EDMA3 IPR bits 0 and 1
      break;
  }
  EVTCLR0 = 0x00000100;
  buffer_full = 1;              // flag EDMA3 transfer
  return;
}

void process_buffer(void)
{
  int16_t *inBuf, *outBuf;      // pointers to process buffers
  int16_t left_sample, right_sample;
  int i,j;

  if (procBuffer == PING)       // use ping or pong buffers
  {
    inBuf = pingIN;
    outBuf = pingOUT;
  }
  if (procBuffer == PONG)
  {
    inBuf = pongIN;
    outBuf = pongOUT;
  }

  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    left_sample = *inBuf++;
    right_sample = *inBuf++;

    y[i] = 0.0;
    x[0] = (float)(left_sample); // input from ADC
    for (j=0 ; j<N ; j++)        // compute filter output
      y[i] += h[j]*x[j];
    for (j=(N-1) ; j>0 ; j--)    // shift delay line
      x[j] = x[j-1]; 
  }
  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    *outBuf++ = (int16_t)(y[i]); // output to L and R DAC
    *outBuf++ = (int16_t)(y[i]);
  }
  buffer_full = 0;               // flag buffer processed
  return;
}

int main(void)
{
  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) 
  {
    while (!buffer_full);
    process_buffer();
  }
}  



