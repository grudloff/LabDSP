// L138_graphicEQ_DSPLIB_edma.c
//

#include "L138_LCDK_aic3106_init.h"
#include <math.h> 
#include "dsplib674x.h"
#include "GraphicEQcoeff.h"
#include "evmomapl138_gpio.h"


extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

short NUMCOEFFS = sizeof(lpcoeff)/sizeof(float);

void gen_w_r2(float* w, int n);
void bit_rev(float* x, int n);


float xL[2*BUFCOUNT];
float xR[2*BUFCOUNT];
float w[2*BUFCOUNT];
float coeffs[2*BUFCOUNT];
float bass[2*BUFCOUNT], mid[2*BUFCOUNT], treble[2*BUFCOUNT];
float overlapL[BUFCOUNT/2];
float overlapR[BUFCOUNT/2];
float a,b;
float bass_gain = 0.0;
float mid_gain = 0.0;
float treble_gain = 1.0;

#pragma DATA_ALIGN(xL, 8);
#pragma DATA_ALIGN(xR, 8);
#pragma DATA_ALIGN(w, 8);
#pragma DATA_ALIGN(coeffs, 8);

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


void process_buffer(void) // should be called from infinite 
                          // loop in main() when process
                          // buffer has been filled
{
  int16_t *inBuf, *outBuf;      // pointers to process buffers
  int i;

  if (procBuffer == PING)       // use ping or pong buffers
  {
    inBuf = pingIN;
    outBuf = pingOUT;
  }
  if (procBuffer == PONG)
  {
    inBuf = pongIN;
    outBuf = pongOUT;
  }
  for (i=0; i<(BUFCOUNT*2) ; i++) 
  {
    xL[i] = 0.0;
    xR[i] = 0.0;
  }
  for (i=0; i<(BUFCOUNT/2) ; i++) 
  {
    xL[i*2] = (float)(*inBuf++);
    xR[i*2] = (float)(*inBuf++);
  }

  DSPF_sp_cfftr2_dit (xL, w, BUFCOUNT);
  bit_rev(xL, BUFCOUNT);
  DSPF_sp_cfftr2_dit (xR, w, BUFCOUNT);
  bit_rev(xR, BUFCOUNT);

  for (i=0 ; i<BUFCOUNT ; i++)
    coeffs[i] = bass[i]*bass_gain
              + mid[i]*mid_gain 
              + treble[i]*treble_gain;
  for (i=0 ; i<BUFCOUNT ; i++) // filter L in frequency domain
  {
    a = xL[i*2]; // real part of result
    b = xL[i*2+1]; // imag part of result
    xL[i*2] = coeffs[i*2]*a - coeffs[i*2+1]*b;
    xL[i*2 + 1] = -(coeffs[i*2]*b + coeffs[i*2+1]*a);
  }
  for (i=0 ; i<BUFCOUNT ; i++) // filter R in frequency domain
  {
    a = xR[i*2]; // real part of result
    b = xR[i*2+1]; // imag part of result
    xR[i*2] = coeffs[i*2]*a - coeffs[i*2+1]*b;
    xR[i*2 + 1] = -(coeffs[i*2]*b + coeffs[i*2+1]*a);
  }
  DSPF_sp_cfftr2_dit (xL, w, BUFCOUNT);
  bit_rev(xL, BUFCOUNT);
  for (i=0 ; i<BUFCOUNT ; i++) xL[i*2] /= BUFCOUNT;
  DSPF_sp_cfftr2_dit (xR, w, BUFCOUNT);
  bit_rev(xR, BUFCOUNT);
  for (i=0 ; i<BUFCOUNT ; i++) xR[i*2] /= BUFCOUNT;

  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    *outBuf++ = (int16_t)(xL[i*2] + overlapL[i]);
    *outBuf++ = (int16_t)(xR[i*2] + overlapR[i]);
    overlapL[i] = xL[(i*2)+BUFCOUNT];
    overlapR[i] = xR[(i*2)+BUFCOUNT];
  }
  buffer_full = 0; // indicate that buffer has been processed
  return;
}

int main(void)
{
  int i;
  uint8_t DIP_value;

  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  gen_w_r2(w,BUFCOUNT);
  bit_rev(w,BUFCOUNT>>1);
     
  for(i=0 ; i<(BUFCOUNT*2) ; i++) 
  { 
    coeffs[i] = 0.0; 
    bass[i] = 0.0;
    mid[i] = 0.0 ;
    treble[i] = 0.0;
  }
  for(i=0 ; i<(BUFCOUNT/2) ; i++) 
  { 
    overlapL[i] = 0.0;
    overlapR[i] = 0.0;
  }
  for(i=0 ; i<NUMCOEFFS ; i++)
  {
    bass[i*2] = lpcoeff[i]; 
    mid[i*2] = bpcoeff[i]; 
    treble[i*2] = hpcoeff[i];
  }
  DSPF_sp_cfftr2_dit(bass, w, BUFCOUNT);
  bit_rev(bass, BUFCOUNT);
  DSPF_sp_cfftr2_dit(mid, w, BUFCOUNT);
  bit_rev(mid, BUFCOUNT);
  DSPF_sp_cfftr2_dit(treble, w, BUFCOUNT);
  bit_rev(treble, BUFCOUNT);

  while(1) 
  {
    while (!buffer_full);
    process_buffer();
    GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
    if(DIP_value==0) bass_gain=1.0; else bass_gain=0.0;
    GPIO_getInput(GPIO_BANK0, GPIO_PIN2, &DIP_value);
    if(DIP_value==0) mid_gain=1.0; else mid_gain=0.0;
    GPIO_getInput(GPIO_BANK0, GPIO_PIN3, &DIP_value);
    if(DIP_value==0) treble_gain=1.0; else treble_gain=0.0;
  }
}

// generate real and imaginary twiddle table of size n/2
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

// bit reverse coefficients
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



