//-----------------------------------------------------------------------------
// \file    test_led_pb.c
// \brief   implementation of AM-1808 leds/dip switch/pushbutton test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_timer.h"
#include "evmam1808_led.h"
#include "evmam1808_pb.h"
#include "test_led_pb.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define PB_MASK_LED_1      (0x0000001E)
#define PB_MASK_LED_2      (0x000001E0)

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
// \brief   tests the leds and pushbuttons. lights each led.
//          prints message to stdout for pushbutton changes.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ledPB(void)
{
   uint32_t rtn;
   uint32_t i;
   uint8_t tmp_val;
   uint32_t last_pb_state = 0;

   printf("------------------------------------------------------------\r\n");
   printf("              AM-1808 LED and Pushbutton Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code will begin by flashing the leds, then it will sit in\r\n");
   printf("a forever loop checking the pushbuttons. when a change in the \r\n");
   printf("pushbuttons is detected, a message will be printed to stdout \r\n");
   printf("and an led will be toggled.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");

   rtn = LED_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing leds!\r\n");
      return (rtn);
   }
   
   rtn = PB_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing pushbuttons.\r\n\r\n");
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   printf("flash led patterns\r\n\r\n");
   
   // turn led 0 on and wait 1 second.
   LED_turnOn(LED_1);
   USTIMER_delay(DELAY_1_SEC);
   
   // turn led 1 on and wait 1 second.
   LED_turnOn(LED_2);
   USTIMER_delay(DELAY_1_SEC);
   
   // flash leds for a few seconds.
   for (i = 0; i < 5; i++)
   {
      LED_toggle(LED_1);
      LED_toggle(LED_2);
      USTIMER_delay(DELAY_HALF_SEC);
   }
   
   printf("loop forever printing pushbutton status\r\n\r\n");
   while (1)
   {
      // check the pushbuttons and print status changes.
      for (i = 0; i < MAX_PB_NUM; i++)
      {
         rtn = PB_get(i, &tmp_val);
         if (rtn != ERR_NO_ERROR)
            return (rtn);

         if (tmp_val)
         {
            if (!CHKBIT(last_pb_state, bitval_u32[i]))
            {
               printf("Pushbutton[%u] was pressed\r\n\r\n", i + 1);
            }
            SETBIT(last_pb_state, bitval_u32[i]);
         }
         else
         {
            if (CHKBIT(last_pb_state, bitval_u32[i]))
            {
               printf("Pushbutton[%u] was released\r\n\r\n", i + 1);
            }
            CLRBIT(last_pb_state, bitval_u32[i]);
         }
      }

      // set leds based on latest pb status.
      if (CHKBIT(last_pb_state, PB_MASK_LED_1))
         LED_turnOn(LED_1);
      else
         LED_turnOff(LED_1);

      if (CHKBIT(last_pb_state, PB_MASK_LED_2))
         LED_turnOn(LED_2);
      else
         LED_turnOff(LED_2);
   }

   // currently can't reach this due to the forever loop.
//    return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

