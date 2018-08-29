// L138_fir4types_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"
#include "L138_fir4types_coeffs.h" 

uint32_t FIR_number = 0;
float dly[N];          		    // filter delay line
float h[4][N];      			// filter coefficients

interrupt void interrupt4(void) // interrupt service routine
{
  int i;
  float yn = 0.0;
 
  dly[0] = (float)(input_left_sample()); // input from ADC
//  dly[0] = (float)(prbs());            // PRBS input
  for (i = 0; i< N; i++)                 // compute output
    yn +=(h[FIR_number][i]*dly[i]);
  for (i = N-1; i > 0; i--)              // shift delay line
    dly[i] = dly[i-1];
  output_left_sample((uint16_t)(yn));    // output to DAC
  return;
}

int main()
{
  uint8_t DIP_value;
  int i;

  for (i=0; i<N; i++)
  { 
	dly[i] = 0.0;
  	h[0][i] = hlp[i];  	    
   	h[1][i] = hhp[i];
   	h[2][i] = hbp[i];
   	h[3][i] = hbs[i];
  }
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1)
  {
	DIP_value = read_LCDK_user_DIP();
    FIR_number = (DIP_value % 4);  // switch filter coeffs
  }
}

