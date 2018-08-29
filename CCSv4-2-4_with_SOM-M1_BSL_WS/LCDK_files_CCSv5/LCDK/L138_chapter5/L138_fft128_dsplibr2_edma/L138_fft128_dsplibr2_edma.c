// L138_fft128_DSPLIB_edma.c
//

#include "L138_LCDK_aic3106_init.h"
#include <math.h> 
#include "dsplib674x.h"

#define TRIGGER 32000

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

void gen_w_r2(float* w, int n);
void bit_rev(float* x, int n);

#pragma DATA_ALIGN(x, 8);
#pragma DATA_ALIGN(w, 8);

float x[BUFCOUNT];
float w[BUFCOUNT];

int16_t outbuffer[BUFCOUNT/2];

interrupt void interrupt4(void) // interrupt service routine
{
 switch(EDMA_3CC_IPR)
  {
    case 1:                     // TCC = 0
      procBuffer = PING;        // process ping
      EDMA_3CC_ICR = 0x0001;    // clear EDMA3 IPR bit TCC
      break;
    case 2:                     // TCC = 1
      procBuffer = PONG;        // process pong
      EDMA_3CC_ICR = 0x0002;    // clear EDMA3 IPR bit TCC 
      break;
    default:                    // may have missed an interrupt
      EDMA_3CC_ICR = 0x0003;    // clear EDMA3 IPR bits 0 and 1
      break;
  }
  EVTCLR0 = 0x00000100;
  buffer_full = 1;              // flag EDMA3 transfer
  return;
}

void process_buffer(void) // should be called from infinite loop in main
                          // program when process buffer has been filled
{
int16_t *inBuf, *outBuf;  // pointers to process buffers
int16_t left_sample, right_sample;
int i;

if (procBuffer == PING)   // use ping or pong buffers
{
  inBuf = pingIN;
  outBuf = pingOUT;
}
if (procBuffer == PONG)
{
  inBuf = pongIN;
  outBuf = pongOUT;
}

for (i = 0; i < (BUFCOUNT/2) ; i++) 
{
  left_sample = *inBuf++;
  right_sample = *inBuf++;  // get left channel input samples into real part of x[]
  x[i*2] = (float)(left_sample);
  x[i*2 + 1] = 0.0;
} 
DSPF_sp_cfftr2_dit(x, w, BUFCOUNT/2);
bit_rev(x, BUFCOUNT/2);
for (i = 0; i < (BUFCOUNT/2) ; i++) 
{
  left_sample = (int16_t)(sqrt(x[i*2]*x[i*2]+x[i*2 + 1]*x[i*2 + 1])/16.0);
  outbuffer[i]=left_sample;
  if (i==0) left_sample = TRIGGER;
  *outBuf++ = left_sample;
  *outBuf++ = left_sample;
}
buffer_full = 0; // indicate that buffer has been processed
return;
}

int main(void)
{
  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  gen_w_r2(w, BUFCOUNT/2);
  bit_rev(w, BUFCOUNT/4);
  while(1) 
  {
    while (!buffer_full);
      process_buffer();
  }
}

/* generate real and imaginary twiddle table of size n/2 complex numbers */
void gen_w_r2(float* w, int n)
{
    int i;
    float pi = 4.0 * atan(1.0);
    float e = pi * 2.0 / n;

    for(i = 0; i < (n >> 1); i++)
    {
        w[2 * i] = cos(i * e);
        w[2 * i + 1] = sin(i * e);
    }
}

/* bit reverse coefficients */
void bit_rev(float* x, int n)
{
    int i, j, k;
    float rtemp, itemp;
    j = 0;

    for(i=1; i < (n-1); i++)
    {
        k = n >> 1;

        while(k <= j)
        {
            j -= k;
            k >>= 1;
        }

        j += k;

        if(i < j)
        {
            rtemp = x[j*2];
            x[j*2] = x[i*2];
            x[i*2] = rtemp;
            itemp = x[j*2+1];
            x[j*2+1] = x[i*2+1];
            x[i*2+1] = itemp;
        }
    }
}

