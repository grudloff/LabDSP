//-----------------------------------------------------------------------------
// \file    evmam1808_nor.h
// \brief   AM-1808 nand defines and function prototypes.
//
//-----------------------------------------------------------------------------

#ifndef EVMAM1808_NOR_H
#define EVMAM1808_NOR_H

//-----------------------------------------------------------------------------
// Public Defines and Macros
//-----------------------------------------------------------------------------
#define NOR_FLASH_BASE              (0x60000000)
#define NOR_FLASH_BLOCK_SIZE_BYTES  (128 * 1024)

//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------

uint32_t NOR_init(void);
uint32_t NOR_read(uint32_t in_addr, uint16_t *dest_buffer, uint32_t in_length);
uint32_t NOR_write(uint32_t in_addr, uint16_t *src_buffer, uint32_t in_length);
uint32_t NOR_eraseBlock(uint32_t in_block_addr);
uint32_t NOR_unlockBlock(uint32_t in_block_addr);

#endif


