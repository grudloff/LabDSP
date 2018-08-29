// L138_sinegenDE_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include <math.h>
#define FREQ 1000
#define SAMPLING_FREQ 8000
#define AMPLITUDE 8000
#define PI 3.14159265358979

float y[3];
float a1;

interrupt void interrupt4(void)        // interrupt service routine
{
  y[0] = -(y[1]*a1)-y[2];              // new y(n)
  y[2] = y[1];                         // update y(n-2)
  y[1] = y[0];                         // update y(n-1)
  output_left_sample((int16_t)(y[0])); // output result
  return;
}

int main(void)
{
  y[1] = 0.0;
  y[2] = AMPLITUDE*sin(2.0*PI*FREQ/SAMPLING_FREQ); // amplitude
  a1 = -2.0*cos(2.0*PI*FREQ/SAMPLING_FREQ);        // frequency

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  while(1);

}
