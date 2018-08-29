// L138_echo_intr_s.c
//

#include "L138_LCDK_aic3106_init.h"
#define GAIN 0.2
#define BUF_SIZE 2000

int16_t Linput,Rinput,Loutput,Routput,Ldelayed,Rdelayed;
int16_t Lbuffer[BUF_SIZE], Rbuffer[BUF_SIZE];
int i = 0;
AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{
  codec_data.uint = input_sample();
  Linput = codec_data.channel[LEFT];
  Rinput = codec_data.channel[RIGHT];
  Ldelayed = Lbuffer[i];
  Rdelayed = Rbuffer[i];
  codec_data.channel[LEFT] = Ldelayed + Linput;
  codec_data.channel[RIGHT] = Rdelayed + Rinput;
  Lbuffer[i] = Linput + Ldelayed*GAIN;
  Rbuffer[i] = Rinput + Rdelayed*GAIN;
  i = (i+1)%BUF_SIZE;
  output_sample(codec_data.uint);
  
  return;
}

int main(void)
{
  for (i=0 ; i<BUF_SIZE ; i++)
  {
    Lbuffer[i] = 0;
    Rbuffer[i] = 0;
  }

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);

}  // end of main()
