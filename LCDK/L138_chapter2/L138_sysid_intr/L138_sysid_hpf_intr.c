// L138_sysid_hpf_intr.c
//

#include "L138_LCDK_aic3106_init.h"

AIC31_data_type codec_data;


#define beta 10E-12          	   //learning rate
#define WLENGTH 256

float w[WLENGTH+1];		           //buffer coeff for adaptive FIR
float dly_adapt[WLENGTH+1];	       //buffer samples of adaptive FIR


interrupt void interrupt4(void) // interrupt service routine
{

int16_t left_sample;
int i;
float fir_out, E, adaptfir_out;

   left_sample = input_right_sample();
    fir_out = (float)(left_sample);
	dly_adapt[0] = prand();
    left_sample = (int16_t)(dly_adapt[0]);


  adaptfir_out = 0.0;


 for (i = 0; i < WLENGTH; i++)
   adaptfir_out +=(w[i]*dly_adapt[i]); //compute adaptive filter output 
 
 E = fir_out - adaptfir_out;           //error signal           
 //	codec_data.channel[RIGHT]=(uint16_t)(E*10); //for right channel;
 	codec_data.channel[RIGHT]=left_sample; //for right channel;
	codec_data.channel[LEFT]=left_sample; //for left channel;

	output_sample(codec_data.uint); //output to both channels 
 
 for (i = WLENGTH-1; i >= 0; i--)         
  {
   w[i] = w[i]+(beta*E*dly_adapt[i]);  //update weights of adaptive FIR  
   dly_adapt[i+1] = dly_adapt[i];      //update samples of adaptive FIR   
  } 


return;
}


int main(void)
{
int i;

 for (i = 0; i <= WLENGTH; i++)
  {
   w[i] = 0.0;       			   //init coeff for adaptive FIR
   dly_adapt[i] = 0.0;			   //init buffer for adaptive FIR
  }  

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  AIC3106_writeRegister( 0, 0x00 ); 
  AIC3106_writeRegister(12, 0xF0 ); // 0.025fs hpf 

  while(1); 
 
  
  
  }  // end of main()


