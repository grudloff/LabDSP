// L138_iir_intr.c
//
// IIR filter implemented using second order sections
// integer coefficients read from file
//

#include "L138_LCDK_aic3106_init.h"

#include "bs1800int.cof"

int w[NUM_SECTIONS][2] = {0};


interrupt void interrupt4()	 //interrupt service routine
{
  int section;   // index for section number
  int input;     // input to each section
  int wn,yn;       // intermediate and output values in each stage

  input = input_left_sample();
//  input = (int)prbs();
  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - ((a[section][1]*w[section][0])>>15) - ((a[section][2]*w[section][1])>>15);
    yn = ((b[section][0]*wn)>>15) + ((b[section][1]*w[section][0])>>15) + ((b[section][2]*w[section][1])>>15);
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

  output_left_sample((int16_t)(yn)); // before writing to codec

  return;                       //return from ISR
}

int main(void)
{

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}  // end of main()
