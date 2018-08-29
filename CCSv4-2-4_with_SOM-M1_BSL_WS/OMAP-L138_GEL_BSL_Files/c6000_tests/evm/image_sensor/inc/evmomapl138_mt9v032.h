//-----------------------------------------------------------------------------
// \file    evmomapl138_mt9v032.h
// \brief   OMAP-L138 MT9V032 definitions and function prototypes.
//
//-----------------------------------------------------------------------------

#ifndef EVMOMAPL138_MT9V032_H
#define EVMOMAPL138_MT9V032_H

//-----------------------------------------------------------------------------
// Public Defines and Macros
//-----------------------------------------------------------------------------

// MT9V032 register defines.


//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------
uint32_t MT9V032_init(void);
void MT9V032_printAllRegs(void);
uint32_t MT9V032_readRegister(uint8_t in_reg_addr, uint16_t *dest_buffer);
uint32_t MT9V032_writeRegister(uint8_t in_reg_addr, uint16_t in_data);

#endif
