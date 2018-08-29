// L138_fft128_edma.c
//

#include "L138_LCDK_aic3106_init.h"
#include <math.h> 
#include "fft.h"

#define N (BUFCOUNT/2)
#define PI 3.14159265358979

#define TRIGGER 32000

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;
COMPLEX twiddle[N];
COMPLEX cbuf[N];
int16_t outbuffer[N];

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

void process_buffer(void) // should be called from infinite 
                          // loop in main() when process
                          // buffer has been filled
{
  int16_t *inBuf, *outBuf;      // pointers to process buffers
  int16_t left_sample, right_sample[256];
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
    right_sample[i] = *inBuf++;

    cbuf[i].real = (float)left_sample;
    cbuf[i].imag = 0.0;
  } 

  fft(cbuf,N,twiddle);

  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    left_sample = (int16_t)(sqrt(cbuf[i].real*cbuf[i].real
                       + cbuf[i].imag*cbuf[i].imag)/16.0);
    outbuffer[i]=left_sample;
    if (i==0) left_sample = TRIGGER;
   
    *outBuf++ = left_sample;
    *outBuf++ = right_sample[i];
  }
  buffer_full = 0; // indicate that buffer has been processed
  return;
}


int main(void)
{
  int n;

  for (n=0 ; n< N ; n++)
  {
    twiddle[n].real = cos(PI*n/N);
    twiddle[n].imag = -sin(PI*n/N);
  }	
  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) 
  {
    while (!buffer_full);
    process_buffer();
  }
}


