//-----------------------------------------------------------------------------
// \file    test_spiflash.c
// \brief   implementation of AM-1808 spi flash test.
//
//-----------------------------------------------------------------------------

#include <string.h>
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_timer.h"
#include "evmam1808_spiflash.h"
#include "test_spiflash.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// address that should be beyond anything used by u-boot.
#define SPIFLASH_SAFE_ADDR          (0x00100000)   // 1MB.
// size of test buffers.
#define MAX_SPIFLASH_BUFFER_SIZE    (2 * SPIFLASH_PAGE_SIZE)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint8_t tx_buffer[MAX_SPIFLASH_BUFFER_SIZE];
static uint8_t rx_buffer[MAX_SPIFLASH_BUFFER_SIZE];

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t verifyFlashData(uint32_t flash_start_addr, uint8_t *ram_start_addr, uint32_t length, uint8_t print_data);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the spi flash. reads, erases, writes, and verifies.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_spiFlash(void)
{
   uint32_t rtn;
   uint32_t i;

   printf("------------------------------------------------------------\r\n");
   printf("                  AM-1808 SPI Flash Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the SPI to interface with the \r\n");
   printf("flash on the SOM. Once configured, the code will read  \r\n");
   printf("and write a portion of SPI flash. \r\n");
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
   
   printf("\r\n--- read/print chunk of flash memory ---\r\n");
   rtn = SPIFLASH_read(SPIFLASH_SAFE_ADDR, rx_buffer, MAX_SPIFLASH_BUFFER_SIZE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror reading spi flash!\r\n");
      return (rtn);
   }
   else
   {
      UTIL_printMem(SPIFLASH_SAFE_ADDR, rx_buffer, MAX_SPIFLASH_BUFFER_SIZE, PRINT_HEADER);
      printf("\tspi flash read passed\r\n");
   }

   printf("\r\n--- erase chunk of flash and verify ---\r\n");
   rtn = SPIFLASH_erase(SPIFLASH_SAFE_ADDR, MAX_SPIFLASH_BUFFER_SIZE, 1);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror erasing spi flash!\r\n");
      return (rtn);
   }
   else
   {
      // set rx_buffer to all ones and verify data was erased.
      memset(rx_buffer, 0xFF, MAX_SPIFLASH_BUFFER_SIZE);
      USTIMER_delay(DELAY_1_SEC);
      rtn = verifyFlashData(SPIFLASH_SAFE_ADDR, rx_buffer, MAX_SPIFLASH_BUFFER_SIZE, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tspi flash erase passed\r\n");
      }
      else
      {
         printf("\tspi flash erase failed\r\n");
         return (rtn);
      }
   }
   
   printf("\r\n--- write chunk of flash and verify ---\r\n");
   // populate tx buffer.
   for (i = 0; i < MAX_SPIFLASH_BUFFER_SIZE; i++)
   {
      tx_buffer[i] = (uint8_t)i;
   }
   
   rtn = SPIFLASH_write(SPIFLASH_SAFE_ADDR, tx_buffer, MAX_SPIFLASH_BUFFER_SIZE, 1);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror writing spi flash!\r\n");
      return (rtn);
   }
   else
   {
      // verify the spi flash actully matches tx buff.
      USTIMER_delay(DELAY_1_SEC);
      rtn = verifyFlashData(SPIFLASH_SAFE_ADDR, tx_buffer, MAX_SPIFLASH_BUFFER_SIZE, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tspi flash write passed\r\n");
      }
      else
      {
         printf("\tspi flash write failed\r\n");
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
// verifies spi flash write worked.
//-----------------------------------------------------------------------------
static uint32_t verifyFlashData(uint32_t flash_start_addr, uint8_t *ram_start_addr, uint32_t length, uint8_t print_data)
{
   uint32_t rtn = ERR_NO_ERROR;
   uint32_t verify_addr = flash_start_addr;
   uint32_t data_idx = 0;
   uint32_t bytes_remaining = length;
   uint32_t verify_length;
   uint8_t first_loop = 1;
   uint8_t tmp_buffer[MAX_SPIFLASH_BUFFER_SIZE];

   while (bytes_remaining > 0)
   {
      // compute the length for the current verify.
      if (bytes_remaining > MAX_SPIFLASH_BUFFER_SIZE)
      {
         verify_length = MAX_SPIFLASH_BUFFER_SIZE;
      }
      else
      {
         verify_length = bytes_remaining;
      }

      rtn = SPIFLASH_read(verify_addr, tmp_buffer, verify_length);
      if (rtn != ERR_NO_ERROR)
      {
         printf("\terror while reading from spi flash during verify");
         break;
      }

      // print data if desired.
      if (print_data)
      {
         if (first_loop)
         {
            UTIL_printMem(verify_addr, tmp_buffer, verify_length, PRINT_HEADER);
            first_loop = 0;
         }
         else
         {
            UTIL_printMem(verify_addr, tmp_buffer, verify_length, PRINT_CONTINUE);
         }
      }

      if (memcmp(tmp_buffer, (uint8_t *)(ram_start_addr + data_idx), verify_length) != 0)
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
