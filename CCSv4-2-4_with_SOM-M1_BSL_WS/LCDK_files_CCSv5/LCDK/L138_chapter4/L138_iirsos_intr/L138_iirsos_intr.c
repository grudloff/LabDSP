// L138_iirsos_intr.c
//
// IIR filter implemented using second order sections
// floating point coefficients read from file
//

#include "L138_LCDK_aic3106_init.h"
#include "impinv.cof"

float w[NUM_SECTIONS][2] = {0};

interrupt void interrupt4(void) // interrupt service routine
{
  int section;   // index for section number
  float input;   // input to each section
  float wn,yn;   // intermediate and output values in each stage

  input = ((float)input_left_sample());
  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][1]*w[section][0]
         - a[section][2]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0]
         + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }
  output_left_sample((int16_t)(yn)); // before writing to codec
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
