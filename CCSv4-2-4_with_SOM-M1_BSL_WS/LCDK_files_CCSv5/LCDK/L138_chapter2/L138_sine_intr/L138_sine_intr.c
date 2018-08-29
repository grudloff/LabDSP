// L138_sine_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "math.h"

#define SAMPLING_FREQ 8000
#define PI 3.14159265358979

float frequency = 1000.0;
float amplitude = 20000.0;
float theta_increment;
float theta = 0.0;

interrupt void interrupt4(void) // interrupt service routine
{
  theta_increment = 2*PI*frequency/SAMPLING_FREQ;
  theta += theta_increment;
  if (theta > 2*PI) theta -= 2*PI;
  output_left_sample((int16_t)(amplitude*sin(theta)));
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
