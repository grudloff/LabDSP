// L138_adaptc.c - non real-time adaptation demonstration

#include <stdio.h>
#include <math.h>
#define beta 0.01                      //convergence rate
#define N  21                          //order of filter 
#define NS  60                         //number of samples
#define Fs  8000                       //sampling frequency
#define pi  3.1415926                     
#define DESIRED 2*cos(2*pi*t*1000/Fs)  //desired signal
#define NOISE sin(2*pi*t*1000/Fs)      //noise signal

float desired[NS], y_out[NS], error[NS];

void main()
{
  long i, t;
  float d, y, e; 
  float w[N+1] = {0.0};
  float x[N+1] = {0.0};

  for (t = 0; t < NS; t++)          //start adaptive algorithm
  {    
    x[0] = NOISE;                   //new noise sample
    d = DESIRED;                    //desired signal
    y = 0;                          //zero filter output 
    for (i = 0; i <= N; i++)
      y += (w[i] * x[i]);           //calculate filter output
    e = d - y;                      //calculate error signal
    for (i = N; i >= 0; i--)         
    {
      w[i] = w[i] + (beta*e*x[i]);  //update filter coeffs
      if (i != 0) x[i] = x[i-1];    //update delay line 
    }   
    desired[t] = d; 
    y_out[t] = y;
    error[t] = e;
  }   
  printf("done!\n");
  return;
}


