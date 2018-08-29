//-----------------------------------------------------------------------------
// \file    test_usb.c
// \brief   implementation of OMAP-L138 usb test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_usb.h"
#include "evmomapl138_timer.h"
#include "test_usb.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define OTG_HOST_TRY_COUNT	4
#define USB_DEV_TRY_COUNT	4
//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the usb.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_usb(void)
{
	uint32_t rtn = ERR_NO_ERROR;
	uint8_t hcrevision = 0;
	uint8_t loopCount = 0;

	uint32_t INTSRCR_Value;
	uint8_t USB_OTG_DEVCTL_Value;

	printf("------------------------------------------------------------\r\n");
	printf("                OMAP-L138 USB Test\r\n\r\n");

	printf("Additional Equipment\r\n");
	printf("--------------------\r\n");
	printf("- None \r\n\r\n");
	printf(" \r\n");
	printf("Test Description\r\n");
	printf("----------------\r\n");
	printf("This test will perform two checks:\r\n");
	printf("1) check if it can activate USB VBUS and sense it\r\n");
	printf("The code enables VBUS and verifies the appropriate bit is set in\r\n");
	printf("SYSCONFIG to indicate VBUS was detected. A cable should not be inserted during this check.\r\n");
	printf(" \r\n");
   printf("2) check for the connection of a USB OTG device \r\n");
	printf("The code will look for a D+ or D- rising edge event which will trigger\r\n");
	printf("the INTSRCR_SESSREQ interrupt which it polls for. Once detected\r\n");
	printf("it will determine if the processor is on the A or B side of the cable\r\n");
	printf("and the speed of the device.\r\n");
	printf("In case an OTG cable is not available, the test also switches to\r\n");
	printf("check for a connection to a USB host (plug the board into a computer's USB port).\r\n");
   printf("It alternates between the OTG and HOST check every 2 seconds.\r\n");
	printf("------------------------------------------------------------\r\n\r\n");

	//------------------------------------
	// initialize the required bsl modules
	//------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	rtn = USB_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing usb!\r\n");
		return (rtn);
	}

	//--------------
	// Get Host Revision test.
	//--------------
	hcrevision = USB_getHostRevision();
	printf("Host Revision = 0x%02X\r\n", hcrevision);


	//------------------------------------
	// initialize USB OTG
	//------------------------------------
	USB_OTG_init();

	//--------------
	// Infinite Loop printing USB Interrupt reg values.
	// Wait for device insertion detection
	//--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
	rtn = ERR_TIMEOUT;
	do
	{
		/////////////////////////////////////////////////////////////
		//// Check for USB OTG
		/////////////////////////////////////////////////////////////

		// Clear the loop counter
		loopCount = 0;

		//Disable soft configuration for power
		USB_OTG->POWER &= ~USB_OTG_POWER_SOFTCONN;
		USTIMER_delay(DELAY_10TH_SEC); // Many not be needed.

		do
		{
			INTSRCR_Value = USB_OTG->INTSRCR;
			USB_OTG_DEVCTL_Value = USB_OTG->DEVCTL;
			printf("OTG?: %i, INTSRCR = 0x%08X, CFGCHIP[2] = 0x%08X, DEVCTL = 0x%02X  \r\n", 
					loopCount, 
					INTSRCR_Value,
					SYSCONFIG->CFGCHIP[2],
					USB_OTG_DEVCTL_Value);

			USTIMER_delay(DELAY_QUARTER_SEC);
			loopCount++;
		}while (((INTSRCR_Value & INTSRCR_SESSREQ) == 0) && (loopCount < OTG_HOST_TRY_COUNT));

		if ((INTSRCR_Value & INTSRCR_SESSREQ) != 0)
		{
			printf("\r\n\r\n -> OTG DEVICE DETECTED...\r\n");

			//------------------------------------
			// Set the session bit and allow the HW to check the ID bit.
			//------------------------------------
			printf("\r\n -> setting session bit \r\n");
			USB_OTG_DEVCTL |= DEVCTL_SESSION;

			do
			{
				INTSRCR_Value = USB_OTG->INTSRCR;
				USB_OTG_DEVCTL_Value = USB_OTG->DEVCTL;
				printf("LOOP: %i, INTSRCR = 0x%08X, CFGCHIP[2] = 0x%08X, DEVCTL = 0x%02X  \r\n", 
						loopCount, 
						INTSRCR_Value,
						SYSCONFIG->CFGCHIP[2],
						USB_OTG_DEVCTL_Value);

				USTIMER_delay(DELAY_HALF_SEC);
				loopCount++;
			}while (USB_OTG_DEVCTL_Value == POST_SESSION_SET_VALUE);



			//------------------------------------
			// Check the device type.
			//------------------------------------
			if ((USB_OTG_DEVCTL_Value & DEVCTL_DEV_TYPE) == 0)
			{
				printf("\r\n -> ...Freon is Type A end of cable. \r\n");
			}
			else
			{
				printf("\r\n -> ...Freon is Type B end of cable. \r\n");
			}

			//------------------------------------
			// Check the device speed.
			//------------------------------------
			if ((USB_OTG_DEVCTL_Value & DEVCTL_FS_DEV) != 0)
			{
				printf("\r\n -> ...Detected device is Full Speed. \r\n");
			}
			else if ((USB_OTG_DEVCTL_Value & DEVCTL_LS_DEV) != 0)
			{
				printf("\r\n -> ...Detected device is Low Speed. \r\n");
			}
			else
			{
				printf("\r\n -> ...Detected device's speed is unknown. \r\n");
			}
			
			rtn = ERR_NO_ERROR;

			return (rtn);
		}

		/////////////////////////////////////////////////////////////
		//// Check for USB Host
		/////////////////////////////////////////////////////////////

		// Clear the loop counter
		loopCount = 0;

		//Enable soft configuration for power
		USB_OTG->POWER |= USB_OTG_POWER_SOFTCONN;
		USTIMER_delay(DELAY_10TH_SEC); // Many not be needed.

		do
		{
			INTSRCR_Value = USB_OTG->INTSRCR;
			USB_OTG_DEVCTL_Value = USB_OTG->DEVCTL;
			printf("HOST?: %i, INTSRCR = 0x%08X, CFGCHIP[2] = 0x%08X, DEVCTL = 0x%02X  \r\n", 
					loopCount, 
					INTSRCR_Value,
					SYSCONFIG->CFGCHIP[2],
					USB_OTG_DEVCTL_Value);

			USTIMER_delay(DELAY_QUARTER_SEC);
			loopCount++;
		}while (((USB_OTG_DEVCTL_Value & VBUS_ABOVE_VBUS_VALID_MASK) != VBUS_ABOVE_VBUS_VALID_MASK) && (loopCount < USB_DEV_TRY_COUNT));

		if ((USB_OTG_DEVCTL_Value & VBUS_ABOVE_VBUS_VALID_MASK) == VBUS_ABOVE_VBUS_VALID_MASK)
		{
			printf("\r\n\r\n -> USB HOST DEVICE DETECTED...\r\n");

			rtn = ERR_NO_ERROR;

			return (rtn);
		}
		else if (((USB_OTG_DEVCTL_Value & VBUS_ABOVE_VBUS_VALID_MASK) != VBUS_ABOVE_VBUS_VALID_MASK)
					&& ((USB_OTG_DEVCTL_Value & VBUS_ABOVE_VBUS_VALID_MASK) != 0))
		{
			//printf("\r\n\r\n -> USB HOST VBUS ERROR...\r\n");
		}

	}while (rtn == ERR_TIMEOUT);

	return (ERR_TIMEOUT);
}


