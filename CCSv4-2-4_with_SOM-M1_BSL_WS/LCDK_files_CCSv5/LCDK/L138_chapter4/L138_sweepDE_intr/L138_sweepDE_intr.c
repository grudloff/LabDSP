// L138_sweepDE_intr.c 
//

#include "L138_LCDK_aic3106_init.h"

#include <math.h>
#define MIN_FREQ 200
#define MAX_FREQ 3800
#define STEP_FREQ 20
#define SWEEP_PERIOD 400
#define SAMPLING_FREQ 8000
#define AMPLITUDE 10000
#define PI 3.14159265358979

float y[3] = {0.0, 0.0, 0.0};
float a1;
float freq = MIN_FREQ;
short sweep_count = 0;

void coeff_gen(float freq)
{
  a1 = -2.0*cos(2.0*PI*freq/SAMPLING_FREQ);
  y[0] = 0.0;
  y[2] = AMPLITUDE*sin(2.0*PI*freq/SAMPLING_FREQ);
  y[1] = 0.0;
  return;
}

interrupt void interrupt4() // interrupt service routine
{
  sweep_count++;
  if (sweep_count >= SWEEP_PERIOD)
  {
    if (freq >= MAX_FREQ) freq = MIN_FREQ;
    else freq += STEP_FREQ;
    coeff_gen(freq);
    sweep_count = 0;
  }
  y[0] = -(y[1]*a1)-y[2];
  y[2] = y[1];                         // update y1(n-2)
  y[1] = y[0];                         // update y1(n-1)
  output_left_sample((int16_t)(y[0])); // output to L DAC
  return;
}


int main(void)
{
  coeff_gen(freq);
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
