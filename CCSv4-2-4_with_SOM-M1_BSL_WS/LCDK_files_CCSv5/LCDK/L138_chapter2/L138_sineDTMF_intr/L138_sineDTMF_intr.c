// L138_sineDTMF_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include <math.h>
#define PI 3.14159265358979

#define TABLESIZE 512        // size of look up table
#define SAMPLING_FREQ 16000
#define STEP_770 (float)(770 * TABLESIZE)/SAMPLING_FREQ
#define STEP_1336 (float)(1336 * TABLESIZE)/SAMPLING_FREQ
#define STEP_697 (float)(697 * TABLESIZE)/SAMPLING_FREQ
#define STEP_852 (float)(852 * TABLESIZE)/SAMPLING_FREQ
#define STEP_941 (float)(941 * TABLESIZE)/SAMPLING_FREQ
#define STEP_1209 (float)(1209 * TABLESIZE)/SAMPLING_FREQ
#define STEP_1477 (float)(1477 * TABLESIZE)/SAMPLING_FREQ
#define STEP_1633 (float)(1633 * TABLESIZE)/SAMPLING_FREQ

int16_t sine_table[TABLESIZE];
float loopindexlow = 0.0;
float loopindexhigh = 0.0;
int16_t i;

interrupt void interrupt4(void) // interrupt service routine
{
  output_left_sample(sine_table[(int16_t)loopindexlow]
                    + sine_table[(int16_t)loopindexhigh]);
  loopindexlow += STEP_770;
  if (loopindexlow > (float)TABLESIZE)
    loopindexlow -= (float)TABLESIZE;
  loopindexhigh += STEP_1477;
  if (loopindexhigh > (float)TABLESIZE)
    loopindexhigh -= (float)TABLESIZE;

  return;
}

int main(void)
{
  L138_initialise_intr(FS_16000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  for (i=0 ; i< TABLESIZE ; i++)
    sine_table[i] = (short)(10000.0*sin(2*PI*i/TABLESIZE));
  while(1);
}
