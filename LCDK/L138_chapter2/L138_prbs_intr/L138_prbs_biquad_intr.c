// L138_prbs_biquad_intr.c
//

#include "L138_LCDK_aic3106_init.h"

AIC31_data_type codec_data;

interrupt void interrupt4(void) // interrupt service routine
{
  codec_data.channel[0] = prbs();
  codec_data.channel[1] = codec_data.channel[0];
  output_sample(codec_data.uint);
  return;
}

int main(void)
{
  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);

  AIC3106_writeRegister( 0,0x01); // change register page to 1
  // write left biquad audio effects filter coefficients
  AIC3106_writeRegister( 1,0x05); // N0 coefficient MSB
  AIC3106_writeRegister( 2,0xf7); // N0 coefficient LSB
  AIC3106_writeRegister( 3,0x03); // N1 coefficient MSB
  AIC3106_writeRegister( 4,0x72); // N1 coefficient LSB
  AIC3106_writeRegister( 5,0x05); // N2 coefficient MSB
  AIC3106_writeRegister( 6,0xf7); // N2 coefficient LSB
  AIC3106_writeRegister( 7,0x29); // N3 coefficient MSB
  AIC3106_writeRegister( 8,0x37); // N3 coefficient LSB
  AIC3106_writeRegister( 9,0xf9); // N4 coefficient MSB
  AIC3106_writeRegister(10,0x2c); // N4 coefficient LSB
  AIC3106_writeRegister(11,0x29); // N5 coefficient MSB
  AIC3106_writeRegister(12,0x37); // N5 coefficient LSB
  AIC3106_writeRegister(13,0x59); // D1 coefficient MSB
  AIC3106_writeRegister(14,0xae); // D1 coefficient LSB
  AIC3106_writeRegister(15,0xb7); // D2 coefficient MSB
  AIC3106_writeRegister(16,0x85); // D2 coefficient LSB
  AIC3106_writeRegister(17,0x53); // D4 coefficient MSB
  AIC3106_writeRegister(18,0xa2); // D4 coefficient LSB
  AIC3106_writeRegister(19,0x93); // D5 coefficient MSB
  AIC3106_writeRegister(20,0xf6); // D5 coefficient LSB
  // write right biquad audio effects filter coefficients
  AIC3106_writeRegister(27,0x05); // N0 coefficient MSB
  AIC3106_writeRegister(28,0xf7); // N0 coefficient LSB
  AIC3106_writeRegister(29,0x03); // N1 coefficient MSB
  AIC3106_writeRegister(30,0x72); // N1 coefficient LSB
  AIC3106_writeRegister(31,0x05); // N2 coefficient MSB
  AIC3106_writeRegister(32,0xf7); // N2 coefficient LSB
  AIC3106_writeRegister(33,0x29); // N3 coefficient MSB
  AIC3106_writeRegister(34,0x37); // N3 coefficient LSB
  AIC3106_writeRegister(35,0xf9); // N4 coefficient MSB
  AIC3106_writeRegister(36,0x2c); // N4 coefficient LSB
  AIC3106_writeRegister(37,0x29); // N5 coefficient MSB
  AIC3106_writeRegister(38,0x37); // N5 coefficient LSB
  AIC3106_writeRegister(39,0x59); // D1 coefficient MSB
  AIC3106_writeRegister(40,0xae); // D1 coefficient LSB
  AIC3106_writeRegister(41,0xb7); // D2 coefficient MSB
  AIC3106_writeRegister(42,0x85); // D2 coefficient LSB
  AIC3106_writeRegister(43,0x53); // D4 coefficient MSB
  AIC3106_writeRegister(44,0xa2); // D4 coefficient LSB
  AIC3106_writeRegister(45,0x93); // D5 coefficient MSB
  AIC3106_writeRegister(46,0xf6); // D5 coefficient LSB
  AIC3106_writeRegister( 0,0x00); // change register page to 0
  AIC3106_writeRegister(12,0x0A); // enable effects filters on L and R
  while(1); 
}


