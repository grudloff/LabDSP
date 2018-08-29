//-----------------------------------------------------------------------------
// \file    test_nand.c
// \brief   implementation of OMAP-L138 nand test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "string.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_nand.h"
#include "test_nand.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// block/page that should be beyond anything used by u-boot.
#define NAND_SAFE_BLOCK             (16)
#define NAND_SAFE_PAGE              (0)
// size of test buffers.
#define MAX_BUFFER_SIZE             (NAND_PAGE_SIZE)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint8_t tx_buffer[MAX_BUFFER_SIZE];
static uint8_t rx_buffer[MAX_BUFFER_SIZE];

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t verifyFlashData(uint32_t verify_block, uint32_t verify_page, uint8_t *ram_start_addr, uint8_t print_data);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the nand.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_nand(void)
{
   uint32_t rtn;
   uint32_t block;
   uint32_t i;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 NAND Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with NAND \r\n");
   printf("flash on the UI board. Once configured, the code will read  \r\n");
   printf("and write a block of NAND. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = NAND_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing nand: %u\r\n", rtn);
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("\r\n--- check bad block ---\r\n");
   block = NAND_SAFE_BLOCK;
   while (NAND_isBadBlock(block) != ERR_NO_ERROR)
   {
      printf("\tblock: %u is bad!\r\n", block);
      block++;
   }
   printf("\tblock: %u good\r\n", block);
   
   printf("\r\n--- read/print a chunk of nand flash ---\r\n");
   for (i = 0; i < 2; i++)
   {
      rtn = NAND_readPage(block, i, rx_buffer);
      if (rtn != ERR_NO_ERROR)
      {
         printf("\terror reading nand flash: %u\r\n", rtn);
         return (rtn);
      }
      else
      {
         UTIL_printMem(i, rx_buffer, MAX_BUFFER_SIZE, PRINT_HEADER);
         printf("\tnand flash read passed\r\n");
      }
   }
   
   printf("\r\n--- erase a block of nand flash ---\r\n");
   rtn = NAND_eraseBlock(block);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror erasing nand flash: %u\r\n", rtn);
      return (rtn);
   }
   else
   {
      memset(tx_buffer, 0xFF, MAX_BUFFER_SIZE);
      USTIMER_delay(DELAY_1_SEC);
      rtn = verifyFlashData(block, NAND_SAFE_PAGE, tx_buffer, PRINT_NONE);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tnand flash erase passed\r\n");
      }
      else
      {
         printf("\tnand flash erase failed\r\n");
         return (rtn);
      }
   }

   printf("\r\n--- write a page of nand flash ---\r\n");
   // populate tx buffer.
   for (i = 0; i < MAX_BUFFER_SIZE; i++)
   {
      tx_buffer[i] = (uint8_t)i;
   }
   rtn = NAND_writePage(block, NAND_SAFE_PAGE, tx_buffer);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror writing nand flash: %u\r\n", rtn);
      return (rtn);
   }
   else
   {
      USTIMER_delay(DELAY_1_SEC);
      rtn = verifyFlashData(block, NAND_SAFE_PAGE, tx_buffer, PRINT_DURING_VERIFY);
      if (ERR_NO_ERROR == rtn)
      {
         printf("\tnand flash write passed\r\n");
      }
      else
      {
         printf("\tnand flash write failed\r\n");
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
// verifies nand flash write worked.
//-----------------------------------------------------------------------------
uint32_t verifyFlashData(uint32_t verify_block, uint32_t verify_page, uint8_t *ram_start_addr, uint8_t print_data)
{
   uint32_t rtn = ERR_NO_ERROR;

   rtn = NAND_readPage(verify_block, verify_page, rx_buffer);
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror while reading from nand flash during verify");
      return (rtn);
   }

   // print data if desired.
   if (print_data)
   {
      UTIL_printMem(verify_block, rx_buffer, MAX_BUFFER_SIZE, PRINT_HEADER);
   }

   if (memcmp(rx_buffer, ram_start_addr, MAX_BUFFER_SIZE) != 0)
   {
      rtn = ERR_FLASH_VERIFY;
      printf("\tdata does not match!");
   }

   return (rtn);
}

