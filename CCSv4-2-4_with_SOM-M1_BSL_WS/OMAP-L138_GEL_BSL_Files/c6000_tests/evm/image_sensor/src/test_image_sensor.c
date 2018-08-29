//------------------------------------------------------------------------------
// \file    test_image_sensor.c
// \brief   implementation of OMAP-L138 image sensor test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_vpif.h"
#include "evmomapl138_led.h"
#include "evmomapl138_mt9v032.h"
#include "test_image_sensor.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the camera interface.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_imageSensor(void)
{
   uint32_t rtn = ERR_NO_ERROR;

   printf("------------------------------------------------------------\r\n");
   printf("                OMAP-L138 Image Sensor Test\r\n\r\n");
   
   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- Aptina MT9V032 Head Board\r\n\r\n");
   
   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code will configure an MT9V032 image sensor to \r\n");
   printf("output test data on the VPIF data bus and config the VPIF to \r\n");
   printf("for raw capture receive mode. The code should be paused after each \r\n");
   printf("test pattern to allow the user to verify the data in memory.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
	// initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = VPIF_initReceive(VIDEO_CONN_CAMERA);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing vpif receive!\r\n");
      return (rtn);
   }

   rtn = MT9V032_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing mt9v032!\r\n");
      return (rtn);
   }
   
   rtn = LED_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing leds!\r\n");
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("config image sensor to output 0x000 on data bus\r\n");
   MT9V032_writeRegister(0x7F, 0x2400);
   printf("pause execution...verify memory @ 0xC1000000\r\n\r\n");
   USTIMER_delay(5*DELAY_1_SEC);
   
   printf("config image sensor to output 0x155 on data bus\r\n");
   MT9V032_writeRegister(0x7F, 0x2555);
   printf("pause execution...verify memory @ 0xC1000000\r\n\r\n");
   USTIMER_delay(5*DELAY_1_SEC);
   
   printf("config image sensor to output 0x2AA on data bus\r\n");
   MT9V032_writeRegister(0x7F, 0x26AA);
   printf("pause execution...verify memory @ 0xC1000000\r\n\r\n");
   USTIMER_delay(5*DELAY_1_SEC);
   
   printf("config image sensor to output 0x3FF on data bus\r\n");
   MT9V032_writeRegister(0x7F, 0x27FF);
   printf("pause execution...verify memory @ 0xC1000000\r\n\r\n");
   USTIMER_delay(2*DELAY_1_SEC);
   
   return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

