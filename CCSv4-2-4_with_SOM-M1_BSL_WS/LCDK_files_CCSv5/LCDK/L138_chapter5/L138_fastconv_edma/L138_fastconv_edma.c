// L138_fastconv_edma.c
//
//
// BUFCOUNT is set in L138_LCDK_aic3106_init.h - usually 256
// this sets the number of samples per edma frame to BUFCOUNT/2
// we will take edma frames of samples and zero pad them to 
// BUFCOUNT and apply BUFCOUNT point FFT
//
// in this program, N is the number of filter coefficients

#include "L138_LCDK_aic3106_init.h"
#include <math.h> 
#include "lp55.cof"
#include "fft.h"

#define PI 3.14159265358979

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

COMPLEX procbuf[BUFCOUNT],coeffs[BUFCOUNT],twiddle[BUFCOUNT];
float overlap[BUFCOUNT/2];
float a,b;

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

  for (i = 0; i < (BUFCOUNT) ; i++)   // initialise procbuf 
  {
    procbuf[i].real = 0.0;
    procbuf[i].imag = 0.0;
  }
  for (i = 0; i < (BUFCOUNT/2) ; i++) // fill half of procbuf 
  {
    procbuf[i].real = (float)(*inBuf++);
    inBuf++;
  }  

  fft(procbuf,BUFCOUNT,twiddle);

  for (i=0 ; i<BUFCOUNT ; i++) // filter in frequency domain
  {                             
    a = procbuf[i].real;
    b = procbuf[i].imag;
    procbuf[i].real = coeffs[i].real*a - coeffs[i].imag*b;
    procbuf[i].imag = -(coeffs[i].real*b + coeffs[i].imag*a);
  }
  fft(procbuf,BUFCOUNT,twiddle);
    
  for (i=0 ; i<BUFCOUNT ; i++)
  {
    procbuf[i].real /= BUFCOUNT;
  }

  for (i = 0; i < (BUFCOUNT/2) ; i++) 
  {
    *outBuf++ = (int16_t)(procbuf[i].real + overlap[i]);
    *outBuf++ = (int16_t)(0);
    overlap[i] = procbuf[i+BUFCOUNT/2].real;
  }
  buffer_full = 0; // flag buffer processed  
  return;
}


int main(void)
{
  int i;

  for (i=0 ; i< BUFCOUNT ; i++) // set up twiddle factors
  {
    twiddle[i].real = cos(PI*i/BUFCOUNT);
    twiddle[i].imag = -sin(PI*i/BUFCOUNT);
  }	
  L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  for(i=0 ; i<(BUFCOUNT) ; i++) 
  { coeffs[i].real = 0.0; coeffs[i].imag = 0.0;}
  for(i=0 ; i<N ; i++) coeffs[i].real = h[i];
  fft(coeffs,BUFCOUNT,twiddle); 

  while(1) 
  {
    while (!buffer_full);
    process_buffer();
  }
}




