// L138_sinegenDTMF_intr.c 
//

#include "L138_LCDK_aic3106_init.h"
#include "math.h"
#include "evmomapl138_dip.h"
#define FREQLO 770
#define FREQHI 1336
#define SAMPLING_FREQ 8000
#define AMPLITUDE 6000
#define BUFSIZE 256
#define PI 3.14159265358979

float ylo[3];
float yhi[3];
float a1lo, a1hi;
float out_buffer[BUFSIZE];
int bufindex = 0;
float output;
volatile short DIP1_ON = 0;

interrupt void interrupt4(void) // interrupt service routine
{
  ylo[0] = -(ylo[1]*a1lo)-ylo[2];
  ylo[2] = ylo[1];                             //update y1(n-2)
  ylo[1] = ylo[0];                             //update y1(n-1)
  yhi[0] = -(yhi[1]*a1hi)-yhi[2];
  yhi[2] = yhi[1];                             //update y1(n-2)
  yhi[1] = yhi[0];                             //update y1(n-1)
  output = (yhi[0]+ylo[0]);
  out_buffer[bufindex++] = output;
  if (bufindex >= BUFSIZE) bufindex = 0;
  if (DIP1_ON) output_left_sample((int16_t)(output));
  else output_left_sample(0);                  //output result
  return;				       //return to main
}

int main(void)
{
  int32_t rtn;
  uint8_t dip_val;

  ylo[1] = 0.0;
  ylo[2] = AMPLITUDE*sin(2.0*PI*FREQLO/SAMPLING_FREQ);
  a1lo = -2.0*cos(2.0*PI*FREQLO/SAMPLING_FREQ);
  yhi[1] = 0.0;
  yhi[2] = AMPLITUDE*sin(2.0*PI*FREQHI/SAMPLING_FREQ);
  a1hi = -2.0*cos(2.0*PI*FREQHI/SAMPLING_FREQ);
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  rtn = DIP_init();
  while(1)
  {
    rtn = DIP_get(0,&dip_val);
    if (dip_val) DIP1_ON = 1; else DIP1_ON = 0;
  }
}  // end of main()
