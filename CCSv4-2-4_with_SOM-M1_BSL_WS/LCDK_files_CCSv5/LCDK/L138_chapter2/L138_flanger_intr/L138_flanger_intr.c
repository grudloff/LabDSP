// L138_flanger_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "math.h"

#define TS 0.000020833333      // sampling rate 48 kHz
#define PERIOD 10              // period of delay modulation
#define PI 3.14159
#define MEAN_DELAY 0.001       // mean delay in seconds
#define MODULATION_MAG 0.0008  // delay modulation magnitude
#define BUFLENGTH 4096 
#define ALPHA 0.9

AIC31_data_type codec_data;
uint16_t in_ptr = 0;           // pointer into buffers
uint16_t out_ptr;
float Lbuffer[BUFLENGTH], Rbuffer[BUFLENGTH];
float t = 0.0;
float Rxn, Ryn, Lxn, Lyn, delay_in_seconds;
uint16_t delay_in_samples;

interrupt void interrupt4(void) // interrupt service routine
{

  codec_data.uint = input_sample();
  Lxn = (float)codec_data.channel[LEFT];
  Rxn = (float)codec_data.channel[RIGHT];
  Lbuffer[in_ptr] = Lxn;
  Rbuffer[in_ptr] = Rxn;
  in_ptr = (in_ptr + 1) % BUFLENGTH;
  t = t + TS;
  delay_in_seconds = MEAN_DELAY
                   + MODULATION_MAG * sin((2*PI/PERIOD)*t); 
  delay_in_samples = (uint16_t)(delay_in_seconds * 48000.0);
  out_ptr = (in_ptr + BUFLENGTH - delay_in_samples) % BUFLENGTH;
  Lyn = Lxn + Lbuffer[out_ptr]*ALPHA; 
  Ryn = Rxn + Rbuffer[out_ptr]*ALPHA;
  codec_data.channel[LEFT] = (uint16_t)Lyn;
  codec_data.channel[RIGHT] = (uint16_t)Ryn;;
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
  L138_initialise_intr(FS_48000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) ;
}
