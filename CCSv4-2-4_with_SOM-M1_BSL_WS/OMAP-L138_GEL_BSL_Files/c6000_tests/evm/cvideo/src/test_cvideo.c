//------------------------------------------------------------------------------
// \file    test_cvideo.c
// \brief   implementation of OMAP-L138 composite video test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_led.h"
#include "evmomapl138_vpif.h"
#include "evmomapl138_videodec.h"
#include "evmomapl138_videoenc.h"
#include "test_cvideo.h"

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
// \brief   tests the composite video.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_compVideo(void)
{
   uint32_t rtn = ERR_NO_ERROR;

   printf("------------------------------------------------------------\r\n");
   printf("           OMAP-L138 Composite Video Test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- video output device (DVD player)\r\n");
   printf("- video input device (TV)\r\n");
   printf("- 2 composite video cables\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("connect a video output device (DVD player, etc) to\r\n");
   printf("connector J6 and a video input device (TV, etc) to connector J4.\r\n");
   printf("start the video output device and execute the code. The video\r\n");
   printf("from the output device will be transmitted to the input device.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //------------------------------------
   // initialize the required bsl modules
   //------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = CVIDEODEC_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing composite decoder!\r\n");
      return (rtn);
   }
   
   rtn = VPIF_initReceive(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing vpif receive composite video!\r\n");
      return (rtn);
   }
   
   rtn = VPIF_initTransmit(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing vpif transmit composite video!\r\n");
      return (rtn);
   }
   
   rtn = VIDEOENC_init(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing composite encoder!\r\n");
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
   
   printf("toggle leds to show we are alive\r\n");
   while (1)
   {
      LED_toggle(LED_1);
      USTIMER_delay(DELAY_HALF_SEC);

      LED_toggle(LED_2);
      USTIMER_delay(DELAY_HALF_SEC);
   }

   // currently can't reach this due to the forever loop.
//    return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

