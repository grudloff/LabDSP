// L138_sysid_average_intr.c
//

#include "L138_LCDK_aic3106_init.h"

#define beta 1E-12           // adaptive filter learning rate
#define WLENGTH 256          // no of adaptive filter weights
#define N 5	             // no of fixed filter coeffs
   
float h[N];                  // filter coeffs
float w[WLENGTH+1];          // adaptive filter coeffs
float dly_adapt[WLENGTH+1];  // adaptive filter delay line
float dly_fix[N+1];          // fixed filter delay line
AIC31_data_type codec_data;

interrupt void interrupt4(void)                                
{
  int i;
  float fir_out, E, adaptfir_out;

  dly_fix[0] = prbs();       // input prbs to both filters 
  dly_adapt[0] = dly_fix[0];

  fir_out = 0.0;             // compute fixed filter output  
  for (i = N-1; i>= 0; i--)
  {
    fir_out +=(h[i]*dly_fix[i]); 
    dly_fix[i+1] = dly_fix[i];   
  }
  codec_data.channel[LEFT] = (uint16_t)(fir_out);
  codec_data.channel[RIGHT] = (uint16_t)(fir_out);
  output_sample(codec_data.uint);
  fir_out = (float)(input_left_sample());
   
  adaptfir_out = 0.0;        //compute adaptive filter output
  for (i = 0; i < WLENGTH; i++)  
    adaptfir_out +=(w[i]*dly_adapt[i]);  
 
  E = fir_out - adaptfir_out;      // compute error           

  for (i = WLENGTH-1; i >= 0; i--)         
  {
    w[i] = w[i]+(beta*E*dly_adapt[i]); // adapt weights  
    dly_adapt[i+1] = dly_adapt[i];     // update delay line   
  } 
  return;
}


int main(void)
{
  int i;

  for (i = 0; i <= WLENGTH; i++)
  {
    w[i] = 0.0;              // initialise adaptive weights
    dly_adapt[i] = 0.0;	     // initialise adaptive delay line
  }  
  for (i=0 ; i<N ; i++)      // initialise fixed filter coeffs
    h[i] = 1.0/N;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1) ;
}


