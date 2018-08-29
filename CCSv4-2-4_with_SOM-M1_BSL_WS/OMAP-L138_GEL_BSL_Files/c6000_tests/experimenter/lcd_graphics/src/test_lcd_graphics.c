//------------------------------------------------------------------------------
// \file    test_lcd_graphics.c
// \brief   implementation of OMAP-L138 graphics lcd test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_lcd_raster.h"
#include "test_lcd_graphics.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define TEST_PATTERN_BARS              (5)
#define TEST_PATTERN_WIDTH             (LCD_WIDTH/TEST_PATTERN_BARS)

#define TEST_PATTERN_COLOR_VALS        (64)

#define TEST_PATTERN_GRADIENT_OFF      (TEST_PATTERN_WIDTH) - TEST_PATTERN_COLOR_VALS //64 //

#define TEST_PATTERN_GRADIENT_WIDTH    -(TEST_PATTERN_WIDTH - TEST_PATTERN_GRADIENT_OFF)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
void testPattern(void);
void testPatternBlackWhiteSplit(uint16_t split);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

uint32_t TEST_graphicsLcd(void)
{
   uint32_t rtn;
   
   printf("------------------------------------------------------------\r\n");
   printf("                OMAP-L138 Graphics LCD Test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- Logic PD Display Kit\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the LCDC to interface with a\r\n");
   printf("4.3in Logic PD Display Kit. Once configured, a test pattern\r\n");
   printf("will be displayed on the LCD. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");

   rtn = RASTER_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("\terror initializing raster controller!\r\n");
      return (rtn);
   }

   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   printf("--- loop forever with LCD running...");
   while (1)
   {
      testPattern();

      // turn led 0 on and wait 1 second.
      LED_turnOn(LED_1);
      USTIMER_delay(DELAY_HALF_SEC);

      // turn led 1 on and wait 1 second.
      LED_turnOn(LED_2);
      USTIMER_delay(DELAY_HALF_SEC);

      // turn led 1 on and wait 1 second.
      LED_turnOff(LED_2);
      USTIMER_delay(DELAY_HALF_SEC);

      // turn led 0 on and wait 1 second.
      LED_turnOff(LED_1);
      USTIMER_delay(DELAY_HALF_SEC);
   }
}

uint32_t TEST_stressTestGraphicsLcd(void)
{
   uint16_t split;

   printf("---------------------------------\r\n");
   printf("--- Execute Graphics LCD test ---\r\n");
   printf("---------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   RASTER_init();

   //--------------
   // execute test.
   //--------------
   printf("--- loop forever with LCD running...");
   while (1)
   {
      // turn led 0 on and wait 1 second.
      LED_turnOn(LED_1);

      for (split = 0; split < LCD_WIDTH; split++)
      {
         testPatternBlackWhiteSplit(split);
      }

      // turn led 0 on and wait 1 second.
      LED_turnOff(LED_1);

      for (split = (LCD_WIDTH - 1); split > 0 ; split--)
      {
         testPatternBlackWhiteSplit(split);
      }
   }
}

void testPattern(void)
{
   uint16_t x, y;
   uint16_t stripColor;

   //Paint a black background
   RASTER_clear(COLOR_BLACK);

   /**** Solid colors ****/

   // Blue
   for (y = 0; y < LCD_HALF_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x, y, COLOR_BLUE);

   //Green
   for (y = 0; y < LCD_HALF_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x + TEST_PATTERN_WIDTH, y, COLOR_GREEN);

   // Red
   for (y = 0; y < LCD_HALF_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x + (TEST_PATTERN_WIDTH * 2), y, COLOR_RED);

   // White
   for (y = 0; y < LCD_HALF_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x + (TEST_PATTERN_WIDTH * 3), y, COLOR_WHITE);

   // Gray
   for (y = 0; y < LCD_HALF_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x + (TEST_PATTERN_WIDTH * 4), y, COLOR_GRAY);

   // Dark Gray
   for (y = LCD_HALF_HEIGHT; y < LCD_HEIGHT; y++)
      for (x = 0; x < TEST_PATTERN_WIDTH; x++)
         RASTER_plot(x + (TEST_PATTERN_WIDTH * 4), y, COLOR_DARK_GRAY);

   // Black divider
   for (y = (LCD_HALF_HEIGHT - 1); y < (LCD_HALF_HEIGHT + 1); y++)
      for (x = 0; x < LCD_WIDTH; x++)
         RASTER_plot(x, y, COLOR_BLACK);

   /**** Gradients ****/

   // Blue
   for (x = 0; x < TEST_PATTERN_GRADIENT_WIDTH; x++)
   {
      stripColor = x >> 1;

      for (y = LCD_HALF_HEIGHT; y < LCD_HEIGHT; y++)
      {
         RASTER_plot(x + TEST_PATTERN_GRADIENT_OFF, y, stripColor);
      }
   }

   // Green
   for (x = 0; x < TEST_PATTERN_GRADIENT_WIDTH; x++)
   {
      stripColor = ((x >> 1) << 6);

      for (y = LCD_HALF_HEIGHT; y < LCD_HEIGHT; y++)
      {
         RASTER_plot(x + TEST_PATTERN_GRADIENT_OFF + TEST_PATTERN_WIDTH, y, stripColor);
      }
   }

   // Red
   for (x = 0; x < TEST_PATTERN_GRADIENT_WIDTH; x++)
   {
      stripColor = ((x >> 1) << 11);

      for (y = LCD_HALF_HEIGHT; y < LCD_HEIGHT; y++)
      {
         RASTER_plot(x + TEST_PATTERN_GRADIENT_OFF + (TEST_PATTERN_WIDTH * 2), y, stripColor);
      }
   }

   // Gray
   for (x = 0; x < TEST_PATTERN_GRADIENT_WIDTH; x++)
   {
      stripColor = (x >> 1) | ((x >> 1) << 6) | ((x >> 1) << 11);

      for (y = LCD_HALF_HEIGHT; y < LCD_HEIGHT; y++)
      {
         RASTER_plot(x + TEST_PATTERN_GRADIENT_OFF + (TEST_PATTERN_WIDTH * 3), y, stripColor);
      }
   }
}

void testPatternBlackWhiteSplit(uint16_t split)
{
   uint32_t x, y;

   //Paint a black background
   for (x = 0; x < split; x++)
      for (y = 0; y < LCD_HEIGHT; y++)
         RASTER_plot(x, y, COLOR_BLACK);

   //Paint a white background
   for (x = split; x < LCD_WIDTH; x++)
      for (y = 0; y < LCD_HEIGHT; y++)
         RASTER_plot(x, y, COLOR_WHITE);
}
