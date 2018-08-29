// L138_loop_poll_PGA.c
//
// PGA_L routed to LEFT_LOP/M, PGA_R routed to RIGHT_LOP/M
// using page 0 registers 81 and 91
// ADC and DAC not used

#include "L138_LCDK_aic3106_init.h"

int main(void)
{
  L138_initialise_poll(FS_8000_HZ,ADC_GAIN_3DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  AIC3106_writeRegister( 82, 0x00 );  // disconnect DAC_L1 from LEFT_LOP/M
  AIC3106_writeRegister( 92, 0x00 );  // disconnect DAC_R1 from RIGHT_LOP/M
  AIC3106_writeRegister( 81, 0x80 );  // route PGA_L to LEFT_LOP/M, Gain=0dB
  AIC3106_writeRegister( 91, 0x80 );  // route PGA_R to RIGHT_LOP/M, Gain=0dB

  while(1);
}
