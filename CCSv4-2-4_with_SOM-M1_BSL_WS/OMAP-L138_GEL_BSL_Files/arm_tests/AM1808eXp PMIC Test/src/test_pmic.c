//-----------------------------------------------------------------------------
// \file    test_pmic.c
// \brief   implementation of AM-1808 pmic test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_pmic.h"
#include "test_pmic.h"

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
// \brief   tests the pmic.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_pmic(void)
{
   uint32_t rtn;

   printf("--------------------------------------------------------------------\r\n");
   printf("                   PMIC test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- None\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This test demonstrates the operation of the AM1808 I/O at 1.8v. The AM1808\r\n");
   printf("will read the contents of the PMIC control registers and display them.\r\n");
   printf("The AM1808 will also adjust its system clock frequency for additional power savings.\r\n");
   printf("The user should flip the 3.3v/1.8v dip switch prior to executing this test.\r\n");
   printf("--------------------------------------------------------------------\r\n\r\n");

   //set sys clock freqency to 50MHz
   config_pll0(0,4,1,0,1,11,5);
   config_pll1(4,1,0,1,2);

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");

   rtn = PMIC_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing pmic!\r\n");
      return (rtn);
   }

   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   printf("\r\n--- read/print all pmic registers ---\r\n");
   rtn = PMIC_readPrintRegs();
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror reading pmic regs!\r\n");
      return (rtn);
   }
   else
   {
      printf("\tpmic read regs passed\r\n\r\n");
   }

   printf("\r\n");

   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


