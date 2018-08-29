//-----------------------------------------------------------------------------
// \file    test_nor.c
// \brief   implementation of OMAP-L138 nor test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "string.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_nor.h"
#include "test_nor.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// address that should be beyond anything used by u-boot.
#define NOR_SAFE_BLOCK              (8)
#define NOR_SAFE_ADDR               (NOR_SAFE_BLOCK * 128 * 1024 + NOR_FLASH_BASE)
// size of test buffers.
#define MAX_BUFFER_SIZE             (256)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint16_t tx_buffer[MAX_BUFFER_SIZE];
static uint16_t rx_buffer[MAX_BUFFER_SIZE];

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t verifyFlashData(uint32_t flash_start_addr, uint16_t *ram_start_addr, uint32_t length, uint8_t print_data);
//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_nor(void)
{
   uint32_t rtn;
   uint32_t i;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 NOR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with NOR \r\n");
   printf("flash on the UI board. Once configured, the code will read  \r\n");
   printf("and write a portion of NOR. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = NOR_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing nor: %u\r\n", rtn);
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("\r\n--- read/print a chunk of nor flash ---\r\n");
   rtn = NOR_read(NOR_SAFE_ADDR, rx_buffer, MAX_BUFFER_SIZE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror reading nor flash: %u\r\n", rtn);
      return (rtn);
   }
   else
   {
      UTIL_printMem(NOR_SAFE_ADDR, (uint8_t *)rx_buffer, MAX_BUFFER_SIZE, PRINT_HEADER);
      printf("\tnor flash read passed\r\n");
   }
   
   printf("\r\n--- unlock a block of nor flash ---\r\n");
   rtn = NOR_unlockBlock(NOR_SAFE_ADDR);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror unlocking nor flash: 0x%08X\r\n", rtn);
      return (rtn);
   }
   else
   {
      printf("\tblock: 0x%08X unlocked!\r\n", NOR_SAFE_ADDR);
   }

   printf("\r\n--- erase a block of nor flash ---\r\n");
   rtn = NOR_eraseBlock(NOR_SAFE_ADDR);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror erasing nor flash: %u\r\n", rtn);
      return (rtn);
   }
   else
   {
      memset((uint8_t *)tx_buffer, 0xFF, MAX_BUFFER_SIZE);
      USTIMER_delay(DELAY_1_SEC);
      rtn = verifyFlashData(NOR_SAFE_ADDR, tx_buffer, MAX_BUFFER_SIZE, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tnor flash erase passed\r\n");
      }
      else
      {
         printf("\tnor flash erase failed\r\n");
         return (rtn);
      }
   }

   printf("\r\n--- write data to nor flash ---\r\n");
   // populate tx buffer.
   for (i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      tx_buffer[i] = (uint16_t)i;
   }

   rtn = NOR_write(NOR_SAFE_ADDR, tx_buffer, MAX_BUFFER_SIZE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror writing nor flash: %u\r\n", rtn);
      return (rtn);
   }
   else
   {
      USTIMER_delay(DELAY_1_SEC);
      printf("\r\n--- verify data in nor flash ---\r\n");
      rtn = verifyFlashData(NOR_SAFE_ADDR, tx_buffer, MAX_BUFFER_SIZE, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tnor flash write passed\r\n");
      }
      else
      {
         printf("\tnor flash write verify failed\r\n");
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
// verifies nor flash write worked.
//-----------------------------------------------------------------------------
uint32_t verifyFlashData(uint32_t flash_start_addr, uint16_t *ram_start_addr, uint32_t length, uint8_t print_data)
{
   uint32_t rtn = ERR_NO_ERROR;
   uint32_t verify_addr = flash_start_addr;
   uint32_t data_idx = 0;
   uint32_t bytes_remaining = length;
   uint32_t verify_length;
   uint8_t first_loop = 1;

   while (bytes_remaining > 0)
   {
      // compute the length for the current verify.
      if (bytes_remaining > MAX_BUFFER_SIZE)
      {
         verify_length = MAX_BUFFER_SIZE;
      }
      else
      {
         verify_length = bytes_remaining;
      }

      rtn = NOR_read(verify_addr, rx_buffer, verify_length);
      if (rtn != ERR_NO_ERROR)
      {
         printf("\terror while reading nor flash during verify");
         break;
      }

      // print data if desired.
      if (print_data)
      {
         if (first_loop)
         {
            UTIL_printMem(verify_addr, (uint8_t *)rx_buffer, verify_length, PRINT_HEADER);
            first_loop = 0;
         }
         else
         {
            UTIL_printMem(verify_addr, (uint8_t *)rx_buffer, verify_length, PRINT_CONTINUE);
         }
      }

      if (memcmp(rx_buffer, (uint8_t *)(ram_start_addr + data_idx), verify_length) != 0)
      {
         rtn = ERR_FLASH_VERIFY;
         printf("\tdata does not match!");
         break;
      }

      verify_addr += verify_length;
      data_idx += verify_length;
      bytes_remaining -= verify_length;
   }

   return (rtn);
}

