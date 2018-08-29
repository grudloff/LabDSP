//-----------------------------------------------------------------------------
// \file    test_rmii_loopback.c
// \brief   implementation of AM-1808 ethernet test.
//
//-----------------------------------------------------------------------------

#include <string.h>
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_emac.h"
#include "evmam1808_timer.h"
#include "test_rmii_loopback.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint8_t test_data[1024];
static uint8_t rx_data[1024];

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the emac, mdio peripherals and ethernet phy connection. 
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_rmii_loopback(void)
{
   uint32_t rtn;
   uint32_t i;
   uint32_t rx_len;

   memset(rx_data, 0, 1024);
   
   printf("--------------------------------------------------------------------\r\n");
   printf("                   RMII Loopback Test\r\n\r\n");
   
   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- None\r\n\r\n");
   
   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("The test will begin by initializing the ethernet PHY on the UI Board.\r\n");
   printf("After initializing, the PHY Activity / Link lights should be\r\n");
   printf("illuminated. Then the PHY will be placed in loopback mode and a\r\n");
   printf("packet will be transmitted. The packet will be checked against\r\n");
   printf("the original data after it is received. Note that a loopback cable\r\n");
   printf("is not required.\r\n");
   printf("--------------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   rtn = EMAC_init(EMAC_INTERFACE_RMII);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror initializing emac!\r\n");
      return (rtn);
   }

   //--------------
   // execute test.
   //--------------
   printf("\r\n--- put phy into loopback mode ---\r\n");
   rtn = EMAC_phyEnterLoopback();
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror entering loopback!\r\n");
      return (rtn);
   }
   else
   {
      printf("\tentered phy loopback!\r\n");
   }
   
   printf("\r\n--- transmit some data ---\r\n");
   rtn = EMAC_txPacket(test_data, 1024);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror transmitting enet packet!\r\n");
      return (rtn);
   }
   else
   {
      printf("\tenet packet transmitted successfully!\r\n");
   }
   
   USTIMER_delay(2000000);

   printf("\r\n--- receive data ---\r\n");
   while (1)
   {
      rtn = EMAC_rxPacket(rx_data, &rx_len);
      if (rtn != ERR_NO_ERROR)
      {
         printf("\terror receiving enet packet: %u\r\n", rtn);
         //return (rtn);
      }
      else
      {
         printf("\tenet packet received successfully!\r\n");
   	  break;
      }
   }

   printf("\r\n--- verify receive data ---\r\n");
   if (rx_len != 1024)
   {
      printf("\tincorrect receive data length!\r\n");
      return (ERR_FAIL);
   }
   for (i = 0; i < 1024; i++)
   {
      if (test_data[i] != rx_data[i])
      {
         printf("\trx data does not match test data!\r\n");
         return (ERR_FAIL);
      }
   }
   printf("\treceive data matches test data!\r\n");
   
   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------
