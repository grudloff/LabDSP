// L138_iirsostr_intr.c
//
// IIR filter implemented using second order sections
// floating point coefficients read from file
//

#include "L138_LCDK_aic3106_init.h"

#include "impinv.cof"

float w[NUM_SECTIONS][2] = {0};



interrupt void interrupt4(void) // associated in intvecs.asm with INT4
{
  int section;   // index for section number
  float input;   // input to each section
  float yn;   // intermediate and output values in each stage

  input = ((float)input_left_sample());
  
  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    yn = b[section][0]*input + w[section][0];
    w[section][0] = b[section][1]*input + w[section][1] - a[section][1]*yn;
    w[section][1] = b[section][2]*input - a[section][2]*yn;
    input = yn;              // output of current section will be input to next
  }

  output_left_sample((int16_t)(yn)); // before writing to codec
  return;
}

int main(void)
{

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}  // end of main()
