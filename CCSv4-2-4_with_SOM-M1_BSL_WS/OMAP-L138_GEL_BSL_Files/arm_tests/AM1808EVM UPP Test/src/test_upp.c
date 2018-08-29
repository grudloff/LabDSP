//------------------------------------------------------------------------------
// \file    test_upp.c
// \brief   implementation of AM-1808 upp test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "test_upp.h"
#include "evmam1808_uPP.h"
#include "evmam1808_i2c_gpio.h"
#include "evmam1808_gpio.h"
#include "evmam1808_CDCE913.h"
#include "evmam1808_sysconfig.h"
#include "evmam1808_timer.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
	
	
	#pragma DATA_ALIGN(xmit_buffer, 64)	//ensures data is alight on 64 byte boundry
	volatile int16_t xmit_buffer[64] = 	//as required by UPP DMA controller
	{
		512,562,612,661,708,753,796,837,874,908,938,964,985,1002,1014,1022,
		1023,1022,1014,1002,985,964,938,908,874,837,796,753,708,661,612,562,
		512,462,412,363,316,271,228,187,150,116,86,60,39,22,10,2,
		0,2,10,22,39,60,86,116,150,187,228,271,316,363,412,462
	};

	#pragma DATA_ALIGN(recv_buffer,64)
	volatile int16_t recv_buffer[64] = 
	{
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0,
    	0,0,0,0,0,0,0,0        
	};

	

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t executeTest(void);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the adc / dac
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_adcdac(void)
{

	uint32_t retVal = ERR_NO_ERROR;

   printf("--------------------------------------------------------------------\r\n");
   printf("                   UPP / ADC / DAC test\r\n\r\n");
   
   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- Oscilloscope\r\n\r\n");
   printf("- Cable with SMA connector\r\n\r\n");
   printf("- Function Generator (passthrough test only)\r\n\r\n");
   printf("- 2nd cable with SMA connector (passthrough test only)\r\n\r\n");
   
   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("The test will begin producing a 73.2421875KHz sine wave on the analog out line\r\n");
   printf("(left SMA connector). After 20 seconds 64 samples will be collected from the ADC (right\r\n");
   printf("SMA connector). These samples will be output through the DAC for a period of 20 Seconds.\r\n");
   printf("Note that the DAC and ADC are inductively/capacitively coupled with their connectors.\r\n");
   printf("It is strongly recommended that the signal provided to the ADC have a frequency that is\r\n");
   printf("an integer multiple of 73.2421875KHz and below 1.171875MHz.\r\n");

   printf("--------------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
	// initialize the required bsl modules.
	//-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	//setup UPP
  	I2CGPIO_init(I2C_ADDR_GPIO_UI);     //IO expander on UI board
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELA, OUTPUT_LOW);
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELB, OUTPUT_LOW);
	I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELC, OUTPUT_HIGH);      //UPP now controls ui databus 

	//Setup ADC Clock
	CDCE913_setOutput(cdce913_output_2, 6);		//set to 4.5Mhz
	
	//--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   	
   retVal = executeTest();
   if (retVal != ERR_NO_ERROR)
   {
      printf("ADC/DAC Test FAILED\r\n\r\n");
      return (retVal);
   }
   else
   {
      printf("ADC/DAC Test completed successfully\r\n\r\n");
   }

 return retVal; 
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

uint32_t executeTest(void)
{	
    upp_config_t config;
	volatile UPXS2_t	* UPIS2r = (UPXS2_t *)&(UPP->UPIS2);
	volatile UPXS2_t	* UPQS2r = (UPXS2_t *)&(UPP->UPQS2);

  	uint32_t retVal = ERR_NO_ERROR;
  	uint32_t i;

	//UPCTL
	config.UPCTL.value=0;
	config.UPCTL.bits.IWB = 1;			//16 bit interface
	config.UPCTL.bits.DPWB = 2;			//10 bit data

	config.UPCTL.bits.DPFA = 0;
	config.UPCTL.bits.DPWA = 2;
	config.UPCTL.bits.IWA = 1;

	config.UPCTL.bits.CHN = 1;			//dual channel mode
	config.UPCTL.bits.MODE = 2;			//0 all recv, 1 all xmit, 2 a recv b xmit, 3 a xmit b recv
										//Channel A ADC, Channel B DAC

	//UPICR
	config.UPICR.value=0;
	config.UPICR.bits.CLKDIVB = 15;		//Set DAC sampling freqency at 75/16 Mhz (4.6875MHz)

	//UPIVR
	config.UPIVR.value=0;
	config.UPIVR.bits.VALB = 0x1000;
	config.UPIVR.bits.VALA = 0x0000;

	//UPTCR
	config.UPTCR.value=0;				//all values 0 for 64byte DMA bursts read / write
	//UPDLB
	config.UPDLB.value=0;				//no loopback				
	//UPIES							
	config.UPIES.value=0;				//dont enable any interrupts
	//UPPCR
	config.UPPCR.value = 0;				
	config.UPPCR.bits.EN = 1;			//enable uPP
	config.UPPCR.bits.RTEMU = 1;		//allow emulator use
	config.UPPCR.bits.SOFT = 1;			//allow emulation
		
    UPP_init(&config); 	

	printf("---Generating 73.2421875KHz sine wave for 20 seconds---\r\n\r\n");
	for(i = 1; i < 1464843; i++) 
	{
	
			UPP->UPQD0 = (uint32_t)&xmit_buffer;//add next DMA transfer
  			UPP->UPQD1 = 0x00010080;   			//1 lines 128 bytes per line
  			UPP->UPQD2 = 0x00000080;   			//no offset between lines

			while(UPQS2r->bits.PEND == 1){};
	}
	
	printf("---Collecting 64 samples from ADC---\r\n");
	UPP->UPID0 = (uint32_t)&recv_buffer;//add next DMA transfer
	UPP->UPID1 = 0x00010080;   			//1 lines 128 bytes per line
	UPP->UPID2 = 0x00000080;   			//no offset between lines
	while(UPIS2r->bits.PEND == 1){};
	

	printf("---Displaying collected samples---\r\n");
	for(i = 0; i < 1464843; i++)
	{
		UPP->UPQD0 = (uint32_t)&recv_buffer;//add next DMA transfer
  		UPP->UPQD1 = 0x00010080;   			//1 lines 128 bytes per line
  		UPP->UPQD2 = 0x00000080;   			//no offset between lines

		while(UPQS2r->bits.PEND == 1){};	//wait for tx transfer to complete
	}

   return retVal;
}
