// L138_sweep_poll.c
//

#include "L138_LCDK_aic3106_init.h"
#include "sine8000_table.h"  //one cycle with 8000 points
#define SAMPLING_FREQ 24000.0
#define N 8000
#define START_FREQ 1000.0
#define STOP_FREQ 2000.0
#define START_INCR START_FREQ*N/SAMPLING_FREQ
#define STOP_INCR STOP_FREQ*N/SAMPLING_FREQ
#define SWEEPTIME 2
#define DELTA_INCR (STOP_INCR - START_INCR)/(N*SWEEPTIME)

int16_t amplitude = 10;
float float_index = 0.0;
float float_incr = START_INCR;
int i;
int counter = 0;

int main(void)
{
  L138_initialise_poll(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  
  while(1)
  {
    float_incr += DELTA_INCR;
	if (float_incr > STOP_INCR) float_incr = START_INCR;
    float_index += float_incr;
    if (float_index > N) float_index -= N;
    i = (int16_t)(float_index);
    output_left_sample(amplitude*sine8000[i]);
    counter++;
  }
}
