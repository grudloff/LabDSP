//-----------------------------------------------------------------------------
// \file    test_ram.c
// \brief   implementation of AM-1808 ram test.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_spiflash.h"
#include "test_read_mac.h"

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
// \brief   reads and prints the mac address that was written into spi flash.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_readMAC(void)
{
   uint32_t rtn = 0;
   uint32_t i;
   uint8_t read_mac[6];

   printf("------------------------------------------------------------\r\n");
   printf("                 AM-1808 Read MAC Address\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code reads and prints the mac address that was \r\n");
   printf("written into spi flash. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = SPIFLASH_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing spi flash!\r\n");
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   // read and print the mac addr.
   printf("--- MAC Address ---\r\n");
   rtn = SPIFLASH_readMACAddr(read_mac);
   for (i = 0; i < 6; i++)
      printf("%02X ", read_mac[i]);

   return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

