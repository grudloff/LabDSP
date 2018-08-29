//------------------------------------------------------------------------------
// \file    test_lcd_char.c
// \brief   implementation of OMAP-L138 character lcd test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "string.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_lcd_lidd.h"
#include "test_lcd_char.h"


//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

#define CHAR_LCD_DEMO_STR_LEN			50

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

uint32_t TEST_charLcd(void)
{
	uint8_t stringBuffer[CHAR_LCD_DEMO_STR_LEN]    = "OMAP-L138 (LIDD)                        ";
	uint8_t count = 0;

   printf("------------------------------------------------------------\r\n");
   printf("                OMAP-L138 Character LCD Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code scrolls a message across the Character LCD on the\r\n");
   printf("UI board. Be sure the potentiometer next between the display\r\n");
   printf("and the pushbuttons is set to appropriately control the backlight.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

	//-------------------------------------
	// Make sure the UI baord is attached.
	//-------------------------------------
	if (!UTIL_isUIBoardAttached())
	{
		printf("UI Board not attached.\r\n");
		return (ERR_NO_UI_BOARD);
	}

	//-------------------------------------
	// initialize the required bsl modules.
	//-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	LIDD_init();
   
	//--------------
	// execute test.
	//--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
	LIDD_writeByte(LCD_INST_REG, 0x38);	// Function Set
	USTIMER_delay(DELAY_5_MS);

	LIDD_writeByte(LCD_INST_REG, 0x0C);	// Display ON/OFF Control
	USTIMER_delay(DELAY_5_MS);

	LIDD_writeByte(LCD_INST_REG, 0x05);	// Entry Mode Set
	USTIMER_delay(DELAY_5_MS);

	LIDD_writeByte(LCD_INST_REG, 0x18);	// Cursor & Display Shift
	USTIMER_delay(DELAY_5_MS);

	printf("--- about to loop forever with LCD running...\r\n");
	while (1)
	{
		printf("...loop forever with LCD running... %i\r\n", count);

		LIDD_writeByte(LCD_INST_REG, 0x01);	// Clear the screen
		USTIMER_delay(DELAY_5_MS);

		LIDD_writeByte(LCD_INST_REG, 0x98);	// Set the cursor position (char 24, row 1)
		USTIMER_delay(DELAY_5_MS);
	
		// turn led 0 on
		LED_turnOn(LED_1);

		// Write the demo string to the char LCD.
		LIDD_writeString(stringBuffer, strlen((char *)stringBuffer), DELAY_200_MS);
		USTIMER_delay(DELAY_1_SEC/4);

		// turn led 1 off
		LED_turnOff(LED_1);

		USTIMER_delay(DELAY_1_SEC/8);

		count++;

	}

   // currently can't reach this due to the forever loop.
//    return (rtn);
}


