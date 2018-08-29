//-----------------------------------------------------------------------------
// \file    test_mmcsd.c
// \brief   implementation of AM-1808 mmc/sd test.
//
//-----------------------------------------------------------------------------

#include <string.h>
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_timer.h"
#include "evmam1808_mmcsd.h"
#include "test_mmcsd.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// size of test buffers.
#define MAX_BUFFER_SIZE_WORD  (128)
#define MAX_BUFFER_SIZE_BYTE  (MAX_BUFFER_SIZE_WORD * 4)

#define TEST_BLOCK            (8)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint32_t tx_buffer[MAX_BUFFER_SIZE_WORD];
static uint32_t rx_buffer[MAX_BUFFER_SIZE_WORD];

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t verifyData(uint32_t verify_block, uint8_t *ram_start_addr, uint8_t print_data);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the mmc/sd controller. 
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_mmcSD(void)
{
   uint32_t rtn;
   uint32_t i;
   uint32_t rca;
   uint8_t num_tries = 0;

   printf("------------------------------------------------------------\r\n");
   printf("                     AM-1808 MMCSD Test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- SD card (MMC card currently not supported)\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code will read/write data to an SD card inserted into the \r\n");
   printf("slot (under side of the board below audio jacks). \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
INIT:
   rtn = MMCSD_init(MMCSD0);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing mmc/sd!\r\n");
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   printf("\r\n--- identify card ---\r\n");
   rtn = MMCSD_identifyCard(MMCSD0, &rca, BUSWIDTH_4BIT);
   if (rtn != ERR_NO_ERROR)
   {
      // try again...
      USTIMER_delay(2 * DELAY_1_SEC);
      rtn = MMCSD_identifyCard(MMCSD0, &rca, BUSWIDTH_4BIT);
      if (rtn != ERR_NO_ERROR)
      {
         printf("\terror identifying card! %u\r\n", rtn);
         return (rtn);
      }
   }

   // if we get here, we succeeded in talking to the card...print rca.
   printf("\tcard rca: %u\r\n", rca);

   printf("\r\n--- read/print chunk of sd memory ---\r\n");
   rtn = MMCSD_readBlock(MMCSD0, TEST_BLOCK, rx_buffer);
   if (rtn != ERR_NO_ERROR)
   {
      if (num_tries++ < 2)
      {
         printf("\terror reading sd memory...try again\r\n");
         goto INIT;
      }
      printf("\terror reading sd memory...too many retries!\r\n");
      return (rtn);
   }
   else
   {
      UTIL_printMem(TEST_BLOCK, (uint8_t *)rx_buffer, MAX_BUFFER_SIZE_BYTE, PRINT_HEADER);
      printf("\tsd memory read passed\r\n");
   }
   
   printf("\r\n--- write block of sd memory and verify ---\r\n");   
   // populate tx buffer.
   for (i = 0; i < MAX_BUFFER_SIZE_WORD; i++)
   {
      tx_buffer[i] = (uint32_t)(i * i);
   }
   
   rtn = MMCSD_writeBlock(MMCSD0, TEST_BLOCK, tx_buffer);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror writing sd memory!\r\n");
      return (rtn);
   }
   else
   {
      rtn = verifyData(TEST_BLOCK, (uint8_t *)tx_buffer, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tsd block write passed\r\n");
      }
      else
      {
         return (rtn);
      }
   }

   printf("\r\n");
   
   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// verifies data matches expected.
//-----------------------------------------------------------------------------
uint32_t verifyData(uint32_t verify_block, uint8_t *ram_start_addr, uint8_t print_data)
{
   uint32_t rtn = ERR_NO_ERROR;

   rtn = MMCSD_readBlock(MMCSD0, verify_block, rx_buffer);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror while reading from sd memory during verify");
      return (rtn);
   }

   // print data if desired.
   if (print_data)
   {
      UTIL_printMem(verify_block, (uint8_t *)rx_buffer, MAX_BUFFER_SIZE_BYTE, PRINT_HEADER);
   }

   if (memcmp((uint8_t *)rx_buffer, ram_start_addr, MAX_BUFFER_SIZE_BYTE) != 0)
   {
      rtn = ERR_FLASH_VERIFY;
      printf("\tdata does not match!");
   }

   return (rtn);
}
