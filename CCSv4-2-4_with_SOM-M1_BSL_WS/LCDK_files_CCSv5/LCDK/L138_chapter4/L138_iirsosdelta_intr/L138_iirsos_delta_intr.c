// L138_iirsosdelta_intr.c
//
// IIR filter implemented using second order sections
// floating point coefficients read from file
//

#include "L138_LCDK_aic3106_init.h"
#define BUFSIZE 256
#define AMPLITUDE 60000.0
#include "impinv.cof"

float w[NUM_SECTIONS][2] = {0};

float dimpulse[BUFSIZE];
float response[BUFSIZE];
int index = 0;

interrupt void interrupt4(void) // associated in intvecs.asm with INT4
{
  int section;   // index for section number
  float input;   // input to each section
  float wn,yn;   // intermediate and output values in each stage

  input = dimpulse[index];   // input to first section read from impulse sequence

  for (section=0 ; section< NUM_SECTIONS ; section++)
  {
    wn = input - a[section][1]*w[section][0] - a[section][2]*w[section][1];
    yn = b[section][0]*wn + b[section][1]*w[section][0] + b[section][2]*w[section][1];
    w[section][1] = w[section][0];
    w[section][0] = wn;
    input = yn;              // output of current section will be input to next
  }

  response[index++] = yn;
  if (index >= BUFSIZE) index = 0;

  output_left_sample((int16_t)(yn*AMPLITUDE)); // before writing to codec

  return;                       //return from ISR
}

int main(void)
{
  int i;
  
  for (i=0 ; i< BUFSIZE ; i++) dimpulse[i] = 0.0;
  dimpulse[0] = 1.0;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}  // end of main()
