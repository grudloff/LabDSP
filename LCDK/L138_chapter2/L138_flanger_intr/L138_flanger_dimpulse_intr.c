// L138_flanger_dimpulse_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "math.h"
//sampling rate 48kHz
#define TS 0.000020833333
 
#define PERIOD 10
#define PI 3.14159
#define MEAN_DELAY 0.001
#define MODULATION_MAG 0.0008
#define BUFLENGTH 4096
#define ALPHA 0.5
 
uint16_t i = 0;
float buffer[BUFLENGTH];
float t = 0.0;
float xn,yn,delay_in_seconds;
uint16_t j;
uint16_t delay_in_samples;
int impulse_count = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  if (impulse_count++ >= 2048)
  {
    impulse_count = 0;
    xn = 10000.0;
  }
  else
    xn = 0.0;
   
  buffer[i] = xn;
  i = (i+1)%BUFLENGTH;
  t = t + TS;
  delay_in_seconds = MEAN_DELAY + MODULATION_MAG * sin((2*PI/PERIOD)*t); 
  delay_in_samples = (uint16_t)(delay_in_seconds * 48000.0);
  j = (i + BUFLENGTH - delay_in_samples) % BUFLENGTH;
  yn = xn + buffer[j]*ALPHA; // change to "-" for HPF, no bass response
  output_left_sample((uint16_t)(yn));
  return;
}

int main(void)
{
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) ;
}
