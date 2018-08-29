// L138_sinegencasm_intr.c
//

#include "L138_LCDK_aic3106_init.h"


int16_t y[3] = {0, 15137, 11585};		
int16_t a1 = 12540;							

interrupt void interrupt4(void) // interrupt service routine
{
  sinegencasmfunc(&y[0], a1); // call ASM function
  output_left_sample(y[0]);   // output to L DAC
  return;
}

void main()
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}

