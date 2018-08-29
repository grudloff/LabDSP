// L138_adaptnoise_intr.c 
//

#include "L138_LCDK_aic3106_init.h"
#include "math.h"

#define SAMPLING_FREQ 8000
#define PI 3.14159265358979
#define NOISE_FREQ 1200.0
#define SIGNAL_FREQ 2500.0
#define NOISE_AMPLITUDE 8000.0
#define SIGNAL_AMPLITUDE 8000.0
#define beta 1E-13              // adaptive learning rate      
#define N 30                    // number of weights

float w[N];                     // adaptive filter weights
float x[N];                     // adaptive filter delay line
short buffercount=0;		 

float theta_increment_noise;
float theta_noise = 0.0;

float theta_increment_signal;
float theta_signal = 0.0;

AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{                         
  short i;
  float yn, error, signal, signoise, refnoise;
 
  theta_increment_noise = 2*PI*NOISE_FREQ/SAMPLING_FREQ;
  theta_noise += theta_increment_noise;
  if (theta_noise > 2*PI) theta_noise -= 2*PI;

  theta_increment_signal = 2*PI*SIGNAL_FREQ/SAMPLING_FREQ;
  theta_signal += theta_increment_signal;
  if (theta_signal > 2*PI) theta_signal -= 2*PI;

  refnoise = (NOISE_AMPLITUDE*cos(theta_noise));  
  signoise = (NOISE_AMPLITUDE*sin(theta_noise));
  signal = (SIGNAL_AMPLITUDE*sin(theta_signal));
  
  x[0] = refnoise;        // reference input to adaptive filter
  yn = 0;                 // compute adaptive filter output
  for (i = 0; i < N; i++)
     yn += (w[i] * x[i]);
  
  error = signal + signoise - yn; // compute error   
          			
  for (i = N-1; i >= 0; i--) // update weights and delay line
  {            
     w[i] = w[i] + beta*error*x[i];
     x[i] = x[i-1];
  } 
      
  codec_data.channel[LEFT]= ((uint16_t)(error));
  codec_data.channel[RIGHT]= ((uint16_t)(signal + signoise));
  output_sample(codec_data.uint);
  return;
}

int main()
{
  int16_t i;                          

  for (i=0 ; i<N ; i++) // initialise weights and delay line 
  {
    w[i] = 0.0;
    x[i] = 0.0;
  } 
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  while(1);
}
