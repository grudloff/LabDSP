// L138_adaptIDFIR_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "bs55.cof"

#define ADAPT_OUT 0
#define FIXED_OUT 1
float beta = 5E-13;
#define WLENGTH 60	
   
float w[WLENGTH+1] ;
int dly_adapt[WLENGTH+1];     // adaptive filter delay line
int dly_fix[N+1];  	      // fixed filter delay line
volatile short out_type = ADAPT_OUT; 
AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{
  int i;
  int fir_out = 0;              // fixed filter output
  int adaptfir_out = 0;         // adaptive filter output
  float E;                      // error

  dly_fix[0] = prbs();  	// input noise to fixed filter
  dly_adapt[0]=dly_fix[0];      // and to adaptive filter
  for (i = N-1; i>= 0; i--)     // compute fixed filter output
  {
    fir_out +=(h[i]*dly_fix[i]);
    dly_fix[i+1] = dly_fix[i];
  }
  for (i = 0; i < WLENGTH; i++) // compute adaptive filter
    adaptfir_out +=(w[i]*dly_adapt[i]);

  E = fir_out - adaptfir_out;   // compute error
  for (i = WLENGTH-1; i >= 0; i--) // update filter weights
  {
    w[i] = w[i]+(beta*E*dly_adapt[i]);
    dly_adapt[i+1] = dly_adapt[i]; 
  }
  if(out_type == ADAPT_OUT)
    codec_data.channel[RIGHT]=((uint16_t)(adaptfir_out));
  else
    codec_data.channel[RIGHT]=((uint16_t)(fir_out));
  codec_data.channel[LEFT]= ((uint16_t)(E));
  output_sample(codec_data.uint);
 return;
}

int main()
{
  int i;
  uint8_t DIP_value;

  for (i=0; i<WLENGTH; i++) // initialise adaptive filter
  {
    w[i] = 0.0;
    dly_adapt[i] = 0.0;
  }
  for (i = 0; i < N; i++)   // initialise fixed filter
    dly_fix[i] = 0.0;
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1)
  {
    GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
    if(DIP_value == 0) out_type = ADAPT_OUT; 
    else out_type = FIXED_OUT;
  }
}



