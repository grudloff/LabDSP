// L138_fastconv_demo.c
//

#include <stdio.h>
#include <math.h> 
#include "lp55.cof"
#include "fft.h"

#define PI 3.14159265358979
#define BUFCOUNT 128

float pingIN[BUFCOUNT/2], pingOUT[BUFCOUNT/2];
COMPLEX procbuf[BUFCOUNT],coeffs[BUFCOUNT],twiddle[BUFCOUNT];
float procbuf_mag[BUFCOUNT],coeffs_mag[BUFCOUNT];
float overlap[BUFCOUNT/2];
float a,b;
int wt = 0;

int main(void)
{
  int i;

// set up twiddle factors
  for (i=0 ; i< BUFCOUNT ; i++)
  {
    twiddle[i].real = cos(PI*i/BUFCOUNT);
    twiddle[i].imag = -sin(PI*i/BUFCOUNT);
  }	
// zero overlap array
  for(i=0 ; i<(BUFCOUNT/2) ; i++)
  {
   	overlap[i] = 0.0;
  }
// set up filter coefficients
  for(i=0 ; i<(BUFCOUNT) ; i++)
  { coeffs[i].real = 0.0; coeffs[i].imag = 0.0;}
  for(i=0 ; i<N ; i++) coeffs[i].real = h[i];
  printf("filter coefficients stored in complex array coeffs[]\n");
  printf("\n"); // place breakpoint here
// transform zero-padded filter coeffs into frequency domain  
  fft(coeffs,BUFCOUNT,twiddle);              
// compute frequency domain magnitude of coefficients
  for (i=0 ; i<BUFCOUNT ; i++) 
    coeffs_mag[i] = sqrt(coeffs[i].real*coeffs[i].real 
                       + coeffs[i].imag*coeffs[i].imag);
  printf("filter coefficients transformed into frequency domain\n");
  printf("\n"); // place breakpoint here

  while(1) 
  {
// fill array pingIN with new input samples
    for(i=0 ; i<(BUFCOUNT/2) ; i++)
    {
      pingIN[i] = (float)(sin(2*PI*wt/50) + 0.25*sin(2*PI*wt/3.3));
      wt++;
    }
    printf("input buffer pingIN filled with BUFCOUNT/2 new sample values\n");
    printf("\n"); // place breakpoint here
// zero contents of complex array procbuf
    for(i=0 ; i<BUFCOUNT ; i++)
    {
      procbuf[i].real = 0.0;
      procbuf[i].imag = 0.0;
    }
// copy new input samples into real part of half of procbuf
    for(i=0 ; i<(BUFCOUNT/2) ; i++)
    {
      procbuf[i].real = pingIN[i];
    }
    printf("process buffer filled with BUFCOUNT/2 new sample values\n");
    printf("\n"); // place breakpoint here
// transform zero-padded input samples into frequency domain
    fft(procbuf,BUFCOUNT,twiddle);
    for (i=0 ; i<BUFCOUNT ; i++) 
// compute frequency domain magnitude of procbuf contents
    procbuf_mag[i] = sqrt(procbuf[i].real*procbuf[i].real
                        + procbuf[i].imag*procbuf[i].imag);
    printf("process buffer transformed\n");
    printf("\n"); // place breakpoint here
// multiply frequency domain representations of input and filter coefficients
    for (i=0 ; i<BUFCOUNT ; i++)  
    {
      a = procbuf[i].real;
      b = procbuf[i].imag;
      procbuf[i].real = coeffs[i].real*a - coeffs[i].imag*b;
      procbuf[i].imag = -(coeffs[i].real*b + coeffs[i].imag*a);
    }
// compute frequency domain magnitude of procbuf contents
    for (i=0 ; i<BUFCOUNT ; i++) 
    procbuf_mag[i] = sqrt(procbuf[i].real*procbuf[i].real 
                        + procbuf[i].imag*procbuf[i].imag);
    printf("multiplied\n");
    printf("\n"); // place breakpoint here
// compute inverse FFT
    fft(procbuf,BUFCOUNT,twiddle);
    for (i=0 ; i<BUFCOUNT ; i++)
    {
      procbuf[i].real /= BUFCOUNT;
      procbuf[i].imag /= BUFCOUNT;
    }
    printf("inverse transformed\n");
    printf("\n"); // place breakpoint here
// fill output and overlap buffers    {
    for (i = 0; i < (BUFCOUNT/2) ; i++)   
    {
      pingOUT[i] = procbuf[i].real + overlap[i];
      overlap[i] = procbuf[i+BUFCOUNT/2].real;
    }
    printf("pingOUT and overlap buffers updated\n");
    printf("\n"); // place breakpoint here
  }
}




