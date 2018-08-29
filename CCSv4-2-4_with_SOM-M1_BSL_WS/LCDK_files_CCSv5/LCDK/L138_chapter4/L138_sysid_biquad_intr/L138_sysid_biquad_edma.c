// L138_sysid_biquad_edma.c
//

#include "L138_LCDK_aic3106_init.h"

extern int16_t *pingIN, *pingOUT, *pongIN, *pongOUT;
volatile int buffer_full = 0;
int procBuffer;

#define beta 1E-12              // adaptive filter learning rate
#define WLENGTH 256

float w[WLENGTH+1];	        // adaptive filter weights
float x[WLENGTH+1];     	// adaptive filter delay line


interrupt void interrupt4(void) // interrupt service routine
{
 switch(EDMA_3CC_IPR)
  {
    case 1:
      procBuffer = PING;         // we should process ping
	  EDMA_3CC_ICR = 0x0001; // clear EDMA3 IPR bit 0 (TCC = 0)
	  break;
    case 2:
      procBuffer = PONG;         // we should process pong
	  EDMA_3CC_ICR = 0x0002; // clear EDMA3 IPR bit 1 (TCC = 1)
	  break;
    default:
	  EDMA_3CC_ICR = 0x0003; // clear EDMA3 IPR bits 0 and 1 (during GEL both flags may have been set)
      break;
  }

  EVTCLR0 = 0x00000100;
  buffer_full = 1;               // global flag indicates switch of buffers
  return;
}

void process_buffer(void) // should be called from infinite loop in main
                          // program when process buffer has been filled
{
int16_t *inBuf, *outBuf; // temporary pointer to current process buffer
int16_t left_sample, right_sample;
int i,ii;

float d;
float y;
float e;

if (procBuffer == PING)
{
  inBuf = pingIN;
  outBuf = pingOUT;
}
if (procBuffer == PONG)
{
  inBuf = pongIN;
  outBuf = pongOUT;
}

for (ii = 0; ii < (BUFCOUNT/2) ; ii++) 
{
left_sample = *inBuf++;
right_sample = *inBuf++;

    d = (float)(right_sample);
	x[0] = prbs();
    right_sample = (int16_t)(x[0]);

  *outBuf++ = right_sample;
  *outBuf++ = right_sample;

  y = 0.0;
 for (i = 0; i < WLENGTH; i++)
   y +=(w[i]*x[i]); //compute adaptive filter output 
 
 e = d - y;           //error signal           
 
 for (i = WLENGTH-1; i >= 0; i--)         
  {
   w[i] = w[i]+(beta*e*x[i]);  //update weights of adaptive FIR  
   x[i+1] = x[i];      //update samples of adaptive FIR   
  } 

}

buffer_full = 0; // indicate that buffer has been processed

return;
}



int main(void)
{
int i;

 for (i = 0; i <= WLENGTH; i++)
  {
   w[i] = 0.0;       			   //init coeff for adaptive FIR
   x[i] = 0.0;			   //init buffer for adaptive FIR
  }  

L138_initialise_edma(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);


AIC3106_writeRegister( 0,0x01);

// elliptic_coeffs.h
// this file was generated automatically using function aic3106_biquad.m
AIC3106_writeRegister( 1,0x01);
AIC3106_writeRegister( 2,0x49);
AIC3106_writeRegister( 3,0x00);
AIC3106_writeRegister( 4,0x82);
AIC3106_writeRegister( 5,0x01);
AIC3106_writeRegister( 6,0x49);
AIC3106_writeRegister( 7,0x7f);
AIC3106_writeRegister( 8,0xff);
AIC3106_writeRegister( 9,0xce);
AIC3106_writeRegister(10,0x47);
AIC3106_writeRegister(11,0x7f);
AIC3106_writeRegister(12,0xff);
AIC3106_writeRegister(13,0x61);
AIC3106_writeRegister(14,0xd7);
AIC3106_writeRegister(15,0xae);
AIC3106_writeRegister(16,0x76);
AIC3106_writeRegister(17,0x60);
AIC3106_writeRegister(18,0xe1);
AIC3106_writeRegister(19,0x90);
AIC3106_writeRegister(20,0xd0);
AIC3106_writeRegister(27,0x01);
AIC3106_writeRegister(28,0x49);
AIC3106_writeRegister(29,0x00);
AIC3106_writeRegister(30,0x82);
AIC3106_writeRegister(31,0x01);
AIC3106_writeRegister(32,0x49);
AIC3106_writeRegister(33,0x7f);
AIC3106_writeRegister(34,0xff);
AIC3106_writeRegister(35,0xce);
AIC3106_writeRegister(36,0x47);
AIC3106_writeRegister(37,0x7f);
AIC3106_writeRegister(38,0xff);
AIC3106_writeRegister(39,0x61);
AIC3106_writeRegister(40,0xd7);
AIC3106_writeRegister(41,0xae);
AIC3106_writeRegister(42,0x76);
AIC3106_writeRegister(43,0x60);
AIC3106_writeRegister(44,0xe1);
AIC3106_writeRegister(45,0x90);
AIC3106_writeRegister(46,0xd0);


// ellbp.h
/*AIC3106_writeRegister( 1,0x05);
AIC3106_writeRegister( 2,0x79);
AIC3106_writeRegister( 3,0x05);
AIC3106_writeRegister( 4,0x76);
AIC3106_writeRegister( 5,0x05);
AIC3106_writeRegister( 6,0x79);
AIC3106_writeRegister( 7,0x7f);
AIC3106_writeRegister( 8,0xff);
AIC3106_writeRegister( 9,0x80);
AIC3106_writeRegister(10,0x06);
AIC3106_writeRegister(11,0x7f);
AIC3106_writeRegister(12,0xff);
AIC3106_writeRegister(13,0x25);
AIC3106_writeRegister(14,0x39);
AIC3106_writeRegister(15,0x9f);
AIC3106_writeRegister(16,0xf9);
AIC3106_writeRegister(17,0x50);
AIC3106_writeRegister(18,0x18);
AIC3106_writeRegister(19,0x99);
AIC3106_writeRegister(20,0x27);
AIC3106_writeRegister(27,0x05);
AIC3106_writeRegister(28,0x79);
AIC3106_writeRegister(29,0x05);
AIC3106_writeRegister(30,0x76);
AIC3106_writeRegister(31,0x05);
AIC3106_writeRegister(32,0x79);
AIC3106_writeRegister(33,0x7f);
AIC3106_writeRegister(34,0xff);
AIC3106_writeRegister(35,0x80);
AIC3106_writeRegister(36,0x06);
AIC3106_writeRegister(37,0x7f);
AIC3106_writeRegister(38,0xff);
AIC3106_writeRegister(39,0x25);
AIC3106_writeRegister(40,0x39);
AIC3106_writeRegister(41,0x9f);
AIC3106_writeRegister(42,0xf9);
AIC3106_writeRegister(43,0x50);
AIC3106_writeRegister(44,0x18);
AIC3106_writeRegister(45,0x99);
AIC3106_writeRegister(46,0x27);
*/

AIC3106_writeRegister( 0,0x00);
AIC3106_writeRegister(12,0x0A);

  while(1) 
  {
    while (!buffer_full);
     process_buffer();
  }

}  // end of main()


