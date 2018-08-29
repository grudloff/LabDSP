//-----------------------------------------------------------------------------
// \file    main.c
// \brief   implementation of main() to test bsl drivers.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_timer.h"
#include "evmam1808_gpio.h"
#include "evmam1808_i2c.h"

#include "test_cvideo.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// comment this define if running with gel initialization.
#define NO_GEL    (1)

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
// \brief   entry point for bsl test code.
//
// \param   none.
//
// \return  none.
//-----------------------------------------------------------------------------
int main(void)
{
   uint32_t results = 0;
   
#if NO_GEL
   EVMAM1808_init();
   EVMAM1808_initRAM();
#endif

   // init the us timer and i2c for all to use.
   USTIMER_init();
   I2C_init(I2C0, I2C_CLK_400K);

   // TEST COMPOSITE VIDEO
   //---------------------
   results += TEST_compVideo();
   printf("\r\n\r\n");
   
   if (results != ERR_NO_ERROR)
      printf("\r\n\r\n********** AM-1808 TEST FAILED **********\r\n\r\n");
   else
      printf("\r\n\r\n********** AM-1808 TEST PASSED **********\r\n\r\n");
}
