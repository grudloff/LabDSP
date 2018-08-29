// L138_sysid_dsplib_edma.c
//

#include "L138_LCDK_aic3106_init.h"
#include "dsplib674x.h"

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

#define beta 1E-12          	   //learning rate
#define WLENGTH 256

float x[WLENGTH + (BUFCOUNT/2)];
float y[(BUFCOUNT/2)];
float d[(BUFCOUNT/2)];
float w[WLENGTH];
float E_dsp = 0.0;

interrupt void interrupt4(void) // associated in intvecs.asm with INT4
                                // should be called every time there's
								// an EDMA3 transfer completion
{
  switch(EDMA_3CC_IPR)
  {
    case 1:
      procBuffer = PING;                 // we should process ping
	  EDMA_3CC_ICR = 0x0001; // clear EDMA3 IPR bit 0 (TCC = 0)
	  break;
    case 2:
      procBuffer = PONG;                 // we should process pong
	  EDMA_3CC_ICR = 0x0002; // clear EDMA3 IPR bit 1 (TCC = 1)
	  break;
    default:
	  EDMA_3CC_ICR = 0x0003; // clear EDMA3 IPR bits 0 and 1 (during GEL both flags may have been set)
      break;
  }
  EVTCLR0 = 0x00000100;
  buffer_full = 1;             // global flag indicates switch of buffers
  return;
}


void process_buffer(void) // should be called from infinite loop in main
                          // program when process buffer has been filled
{
  int16_t *inBuf, *outBuf; // temporary pointer to current process buffer
  int16_t left_sample, right_sample;
  int16_t i;

  if (procBuffer == PING) 
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

    d[i] = (float)(left_sample);
    right_sample = prbs();
    x[i+WLENGTH] = (float)(right_sample);

    *outBuf++ = right_sample;
    *outBuf++ = right_sample;
  }
  E_dsp = DSPF_sp_lms(x,w,d,y,beta,E_dsp,WLENGTH,(BUFCOUNT/2));
  for (i = 0; i < WLENGTH ; i++) // shffle delay line contents 
  {
    x[i] = x[i+(BUFCOUNT/2)];
  }
  x[0] = 0.0;
  buffer_full = 0; // indicate that buffer has been processed
  return;
}

int main(void)
{
  int i;

  for (i=0 ; i<WLENGTH ; i++) // initialise adaptive filter weights
  {
	w[i] = 0.0;
  }
  for (i=0 ; i<(WLENGTH+(BUFCOUNT/2)) ; i++) // initialise adaptive filter weights
  {
	x[i] = 0.0;
  }
  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) 
  {
    while (!buffer_full); // wait for DMA transfer complete
    process_buffer();     // process contents of buffer
  }
}




