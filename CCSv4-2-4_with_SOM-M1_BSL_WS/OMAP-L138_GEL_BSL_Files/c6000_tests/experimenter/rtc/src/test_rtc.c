//-----------------------------------------------------------------------------
// \file    test_rtc.c
// \brief   implementation of OMAP-L138 rtc test.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_rtc.h"
#include "test_rtc.h"

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
// \brief   .
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_rtc(void)
{
   uint32_t rtn = 0;
   rtc_time_t get_time;
   rtc_time_t set_time = { 0x00,
                           0x00,
                           0x15,
                           0x18,
                           0x04,
                           0x80,
                           0x03};
   
   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 RTC Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code will configure the RTC module, set the time, and \r\n");
   printf("verify the time is correct after 10 seconds. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = RTC_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing rtc!\r\n");
		return (rtn);
	}
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("--- Set time: Wednesday April 18, 80 15:00:00 ");
   printf("(%02x/%02x/%02x %02x:%02x:%02x) ---\r\n\r\n", 
            set_time.month,
            set_time.day,
            set_time.year,
            set_time.hour,
            set_time.min,
            set_time.sec);
   rtn = RTC_setTime(&set_time);
   if (rtn != ERR_NO_ERROR)
   {
      return (rtn);
   }

   printf("\twait 10 seconds...\r\n\r\n");
   USTIMER_delay(10 * DELAY_1_SEC);
   
   // get the current time and print.
   rtn = RTC_getTime(&get_time);
   if (rtn != ERR_NO_ERROR)
   {
      return (rtn);
   }
   else
   {
      printf("\tGet time: %02x/%02x/%02x %02x:%02x:%02x\r\n\r\n", 
               get_time.month,
               get_time.day,
               get_time.year,
               get_time.hour,
               get_time.min,
               get_time.sec);
   }

   return (ERR_NO_ERROR);
}
