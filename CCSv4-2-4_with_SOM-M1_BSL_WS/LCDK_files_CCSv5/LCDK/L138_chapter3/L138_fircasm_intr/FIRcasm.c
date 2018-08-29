// L138_FIRcasm_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "bp41.cof"

int yn = 0;				  // filter output
short dly[N];        		  // filter delay line 

interrupt void interrupt4(void) // interrupt service routine
{
  dly[N-1] = input_left_sample();      // input from ADC
  yn = fircasmfunc(dly,h,N);           // call ASM function
  output_left_sample((short)(yn>>15)); // output to DAC
  return;
}

void main()
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
