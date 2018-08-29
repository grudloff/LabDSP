// L138_iirsos_edma.c
//
// IIR filter implemented using second order sections
// floating point coefficients read from file
//

#include "L138_LCDK_aic3106_init.h"
#include "elliptic.cof"

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

float w[NUM_SECTIONS][2] = {0};

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
int i;
int section ;   // index for section number
float input;   // input to each section
float wn,yn;   // intermediate and output values in each stage

if (procBuffer == PING)
{
  inBuf = pingIN;
  outBuf = pingOUT;
}
else
//if (procBuffer == PONG)
{
  inBuf = pongIN;
  outBuf = pongOUT;
}

for (i = 0; i < (BUFCOUNT/2) ; i++) // for each sample in frame
{

  left_sample = *inBuf++;
  right_sample = *inBuf++;

//  input =((float)prbs());   // input to first section read from codec
  input =((float)left_sample);   // input to first section read from codec

  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][1]*w[section][0] - a[section][2]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

  left_sample = (int16_t)(yn);
  right_sample = (int16_t)(yn);
  *outBuf++ = left_sample;
  *outBuf++ = right_sample;

}

buffer_full = 0; // indicate that buffer has been processed

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

}  // end of main()
