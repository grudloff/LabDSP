//-----------------------------------------------------------------------------
// \file    evmam1808_mt9v032.c
// \brief   implementation of a camera driver for the AM-1808 EVM.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_timer.h"
#include "evmam1808_i2c.h"
#include "evmam1808_i2c_gpio.h"
#include "evmam1808_emac.h"
#include "evmam1808_cdce913.h"
#include "evmam1808_mt9v032.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

// i2c defines.
#define I2C_PORT_MT9V032      (I2C0)
#define I2C_ADDR_MT9V032      (0x004C)
#define I2C_ADDR_BUSSW        (0x0073)
// i2c bus switch defines.
#define I2C_BUSSW_CH0EN       (0x01)
#define I2C_BUSSW_CH1EN       (0x02)

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// .
//-----------------------------------------------------------------------------
uint32_t MT9V032_init(void)
{
   uint32_t rtn;
   uint8_t i2c_data;

   rtn = I2CGPIO_init(I2C_ADDR_GPIO_UI);

   // make sure we are getting a 27MHz clock.
   rtn = CDCE913_init();

   // enable the camera (active low), disable all others on the vpif bus.
   rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELA, 1);
   rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELB, 0);
   rtn |= I2CGPIO_setOutput(I2C_ADDR_GPIO_UI, I2C_GPIO_UI_SELC, 1);
   
   // enable channel 0 on the i2c bus switch, so we can talk to the camera.
   i2c_data = I2C_BUSSW_CH0EN;
   rtn |= I2C_write(I2C_PORT_MT9V032, I2C_ADDR_BUSSW, &i2c_data, 1, SET_STOP_BIT_AFTER_WRITE);
   
   // put camera in power down, then normal operating mode to reset device.
   rtn |= MT9V032_writeRegister(0x0C, 0x0001);
   USTIMER_delay(100);
   rtn |= MT9V032_writeRegister(0x0C, 0x0000);
   USTIMER_delay(100);

   // use settings from aptina.
   rtn |= MT9V032_writeRegister(0x0F, 0x0011);
   rtn |= MT9V032_writeRegister(0x31, 0x001F);
   rtn |= MT9V032_writeRegister(0x32, 0x001A);
   rtn |= MT9V032_writeRegister(0x33, 0x0012);
   rtn |= MT9V032_writeRegister(0x35, 0x0020);
   rtn |= MT9V032_writeRegister(0x70, 0x0014);
   rtn |= MT9V032_writeRegister(0xAF, 0x0000);
   rtn |= MT9V032_writeRegister(0xBA, 0x0020);
   
   return (rtn);
}

void MT9V032_printAllRegs(void)
{
   uint32_t rtn;
   uint32_t i;
   uint16_t reg_u16;
   
   printf("\r\nreg  data   return\r\n");

   for (i = 0; i <= 0x38; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   rtn = MT9V032_readRegister(0x42, &reg_u16);
   printf("0x%02X: 0x%04X, %u\r\n", 0x42, reg_u16, rtn);
   for (i = 0x46; i <= 0x48; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   rtn = MT9V032_readRegister(0x4C, &reg_u16);
   printf("0x%02X: 0x%04X, %u\r\n", 0x4C, reg_u16, rtn);
   for (i = 0x60; i <= 0x6C; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   for (i = 0x70; i <= 0x74; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   for (i = 0x7F; i <= 0xAB; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   for (i = 0xAF; i <= 0xC5; i++)
   {
      rtn = MT9V032_readRegister(i, &reg_u16);
      printf("0x%02X: 0x%04X, %u\r\n", i, reg_u16, rtn);
   }
   rtn = MT9V032_readRegister(0xF0, &reg_u16);
   printf("0x%02X: 0x%04X, %u\r\n", 0xF0, reg_u16, rtn);
   rtn = MT9V032_readRegister(0xFE, &reg_u16);
   printf("0x%02X: 0x%04X, %u\r\n", 0xFE, reg_u16, rtn);
   rtn = MT9V032_readRegister(0xFF, &reg_u16);
   printf("0x%02X: 0x%04X, %u\r\n", 0xFF, reg_u16, rtn);
}

//-----------------------------------------------------------------------------
// reads a register from MT9V032.
//-----------------------------------------------------------------------------
uint32_t MT9V032_readRegister(uint8_t in_reg_addr, uint16_t *dest_buffer)
{
   uint32_t rtn;
   uint8_t i2c_data[2];

   // write the register address that we want to read.
   rtn = I2C_write(I2C_PORT_MT9V032, I2C_ADDR_MT9V032, &in_reg_addr, 1, SKIP_STOP_BIT_AFTER_WRITE);
   if (rtn != ERR_NO_ERROR)
      return (rtn);

   // clock out the register data.
   rtn = I2C_read(I2C_PORT_MT9V032, I2C_ADDR_MT9V032, i2c_data, 2, SKIP_BUSY_BIT_CHECK);
   if (rtn != ERR_NO_ERROR)
      return (rtn);

   *dest_buffer = (uint16_t)((i2c_data[0] << 8) | i2c_data[1]);
   
   return (rtn);
}

//-----------------------------------------------------------------------------
// writes a register to MT9V032.
//-----------------------------------------------------------------------------
uint32_t MT9V032_writeRegister(uint8_t in_reg_addr, uint16_t in_data)
{
   uint32_t rtn;
   uint8_t i2c_data[3];
   
   i2c_data[0] = in_reg_addr;
   i2c_data[1] = (uint8_t)((in_data & 0xFF00) >> 8);
   i2c_data[2] = (uint8_t)(in_data & 0x00FF);

   // write the register that we want to read.
   rtn = I2C_write(I2C_PORT_MT9V032, I2C_ADDR_MT9V032, i2c_data, 3, SET_STOP_BIT_AFTER_WRITE);

   return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------
