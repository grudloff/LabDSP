// L138_adaptIDFIR_init_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "L138_adaptIDFIR_init_coeffs.h"

#define ADAPT_OUT 0
#define FIXED_OUT 1
float beta = 1E-13;          	       //rate of convergence
int dly_adapt[WLENGTH+1];	           //buffer samples of adaptive FIR
int dly_fix[N+1];                      //buffer samples of fixed FIR
short out_type = ADAPT_OUT; 		   //output for adaptive/fixed FIR
AIC31_data_type codec_data;

interrupt void interrupt4(void)        //ISR
{
 int i;
 int fir_out = 0;                      //init output of fixed FIR
 int adaptfir_out = 0;                 //init output of adapt FIR
 float E;                              //error=diff of fixed/adapt out

 dly_fix[0] = prbs();  	               //input noise to fixed FIR
 dly_adapt[0]=dly_fix[0];          	   //as well as to adaptive FIR
 for (i = N-1; i>= 0; i--)
  {
   fir_out +=(h[i]*dly_fix[i]);        //fixed FIR filter output
   dly_fix[i+1] = dly_fix[i];          //update samples of fixed FIR
  }
 for (i = 0; i < WLENGTH; i++)
   adaptfir_out +=(w[i]*dly_adapt[i]); //adaptive FIR filter output

 E = fir_out - adaptfir_out;           //error signal

 for (i = WLENGTH-1; i >= 0; i--)
  {
   w[i] = w[i]+(beta*E*dly_adapt[i]);  //update weights of adaptive FIR
   dly_adapt[i+1] = dly_adapt[i];      //update samples of adaptive FIR
  }
if(out_type == ADAPT_OUT)
  codec_data.channel[RIGHT]= ((uint16_t)(adaptfir_out));
else
  codec_data.channel[RIGHT]= ((uint16_t)(fir_out));

  codec_data.channel[LEFT]= ((uint16_t)(E));
  output_sample(codec_data.uint);
 return;
}

int main()
{
  int i=0;
  uint8_t DIP_value;

 for (i = 0; i < WLENGTH; i++)
  {
   dly_adapt[i] = 0.0;			   //init buffer for adaptive FIR
  }
 for (i = 0; i < N; i++)
  dly_fix[i] = 0.0;			       //init buffer for fixed FIR

 L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
 out_type = ADAPT_OUT;
 while(1)
 {
  GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
  if(DIP_value == 0) out_type = FIXED_OUT; else out_type = ADAPT_OUT;
 }
}



