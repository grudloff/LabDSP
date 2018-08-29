// L138_loop_triple_buffer_edma.c
//

#include "L138_LCDK_aic3106_init_triple.h"

extern int16_t *tempptr, *inptr, *outptr, *procptr;
volatile int buffer_full = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  EDMA_3CC_ICR = 0x0001;        // clear EDMA3 IPR bits 0 and 1
  EVTCLR0 = 0x00000100;         // clear TCC event flag

  tempptr = procptr;            // rotate buffer pointers
  procptr = inptr;
  inptr = outptr;
  outptr = tempptr;

  buffer_full = 1;              // indicate TCC event

  return;
}

void process_buffer(void)
{
  int16_t *pBuf;                // temporary pointer
  int16_t left_sample, right_sample;
  int i;

  pBuf = procptr;               // point to process buffer

  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    left_sample = *pBuf;        // extract L + R input samples
    right_sample = *(pBuf+1);

    left_sample *= 2.0;         // process samples
    right_sample *= 2.0;

    *pBuf++ = left_sample;      // place processed values in buffer
    *pBuf++ = right_sample;
  }
  buffer_full = 0;              // indicate that buffer has been processed
  return;
}

int main(void)
{
  L138_initialise_edma(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) 
  {
    while (!buffer_full);
    process_buffer();
  }
}
