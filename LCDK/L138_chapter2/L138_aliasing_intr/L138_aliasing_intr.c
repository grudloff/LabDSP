// L138_aliasing_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "lp6545.cof"	          // filter coefficients
#include "evmomapl138_gpio.h"

#define DISCARD 0
#define SAMPLE 1

int16_t flag = DISCARD;           // toggles for 2x down-sampling
int16_t indly[N],outdly[N];       // antialias and reconst delay lines
float yn; int i;                  //filter output, index
int16_t antialiasing = 0;           //init for no antialiasing filter

interrupt void interrupt4(void) // interrupt service routine
{
 indly[0]=(float)(input_left_sample());//new sample to antialias filter
 yn = 0.0;                        //initialize downsampled value
 if (flag == DISCARD) flag = SAMPLE; //don't discard at next sampling
 else
  {
   if (antialiasing == 1)         //if antialiasing filter desired
    {                             //compute downsampled value
     for (i = 0 ; i < N ; i++)    //using LP filter coeffs
        yn += (h[i]*indly[i]);    //filter is implemented using float
    }
   else                           //if filter is bypassed
        yn = indly[0];            //downsampled value is input value
   flag = DISCARD;                //next input value will be discarded
  }
 for (i = N-1; i > 0; i--)
 		indly[i] = indly[i-1];	  //update input buffer

 outdly[0] = (yn); 		     	  //input to reconst filter
 yn = 0.0;                        //8 kHz sample values and zeros
 for (i = 0 ; i < N ; i++)        //are filtered at 16 kHz rate
      yn += (h[i]*outdly[i]);     //by reconstruction lowpass filter

 for (i = N-1; i > 0; i--)
      outdly[i] = outdly[i-1];    //update delays

 output_left_sample((int16_t)yn);   //16 kHz rate sample
 return;                          //return from interrupt
}

int main(void)
{
uint8_t DIP_value;

  L138_initialise_intr(FS_16000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1)
{
  GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
  if(DIP_value == 0) antialiasing = 1; else antialiasing = 0;
}

}  // end of main()
