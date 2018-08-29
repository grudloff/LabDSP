// L138_prbs_deemph_intr.c
//

#include "L138_LCDK_aic3106_init.h"
#include "evmomapl138_gpio.h"

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
  uint8_t  DIP_value;

  L138_initialise_intr(FS_8000_HZ,ADC_GAIN_0DB,DAC_ATTEN_0DB,LCDK_LINE_INPUT);
  
  while(1) 
  {
    GPIO_getInput(GPIO_BANK0, GPIO_PIN1, &DIP_value);
    if(DIP_value == 0)
    {
      AIC3106_writeRegister(  0, 0x00 ); 
      AIC3106_writeRegister( 12, 0x00 ); 
    }
    else
    {
      AIC3106_writeRegister(  0, 0x00 ); 
      AIC3106_writeRegister( 12, 0x05 ); 
    }
	  GPIO_getInput(GPIO_BANK0, GPIO_PIN2, &DIP_value);
    if(DIP_value == 0)
    {
      AIC3106_writeRegister(  0, 0x01 ); 
      AIC3106_writeRegister( 21, 0x39 ); 
      AIC3106_writeRegister( 22, 0x55 ); 
      AIC3106_writeRegister( 23, 0xF3 ); 
      AIC3106_writeRegister( 24, 0x2D ); 
      AIC3106_writeRegister( 25, 0x53 ); 
      AIC3106_writeRegister( 26, 0x7E ); 
      AIC3106_writeRegister( 47, 0x39 ); 
      AIC3106_writeRegister( 48, 0x55 ); 
      AIC3106_writeRegister( 49, 0xF3 ); 
      AIC3106_writeRegister( 50, 0x2D ); 
      AIC3106_writeRegister( 51, 0x53 ); 
      AIC3106_writeRegister( 52, 0x7E ); 
    }
    else
    {
      AIC3106_writeRegister(  0, 0x01 ); 
      AIC3106_writeRegister( 21, 0x5F ); 
      AIC3106_writeRegister( 22, 0xFF ); 
      AIC3106_writeRegister( 23, 0xa0 ); 
      AIC3106_writeRegister( 24, 0x00 ); 
      AIC3106_writeRegister( 25, 0x10 ); 
      AIC3106_writeRegister( 26, 0x00 ); 
      AIC3106_writeRegister( 47, 0x5F ); 
      AIC3106_writeRegister( 48, 0xFF ); 
      AIC3106_writeRegister( 49, 0xa0 ); 
      AIC3106_writeRegister( 50, 0x00 ); 
      AIC3106_writeRegister( 51, 0x10 ); 
      AIC3106_writeRegister( 52, 0x00 ); 
    }
  }
}


