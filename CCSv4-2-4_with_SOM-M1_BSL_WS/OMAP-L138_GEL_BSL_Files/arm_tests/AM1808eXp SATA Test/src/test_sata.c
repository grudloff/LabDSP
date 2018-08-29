//-----------------------------------------------------------------------------
// \file    test_usb.c
// \brief   implementation of AM-1808 usb test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmam1808.h"
#include "evmam1808_usb.h"
#include "evmam1808_timer.h"
#include "test_sata.h"
#include "sata_ahci.h"
#include "sata_utilities.h" 
#include "cslr_sata_001.h"


//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define IDENT_DEV_CMD_SLOT_USE1		1
#define IDENT_DEV_CMD_SLOT_USE2		3
#define SET_FEATURE_CMD_SLOT_USE	2

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

Uint16 tempCnt;
extern Uint16 tempCnt;
CSL_PscRegsOvly pscRegs = CSL_PSC_1_REGS;


//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the sata.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_sata(void)
{
	uint32_t rtn = ERR_NO_ERROR;

	printf("------------------------------------------------------------\r\n");
	printf("                AM-1808 SATA Test\r\n\r\n");

	printf("Additional Equipment\r\n");
	printf("--------------------\r\n");
	printf("- An externaly powered SATA disk will need to be connected \r\n\r\n");
	printf("- before the test begins. \r\n\r\n");
	printf(" \r\n");
	printf("Test Description\r\n");
	printf("----------------\r\n");
	printf("This test will check for the connection of a SATA disk and attempt \r\n");
	printf("to spin it up. If the connection is not detected or the spin up \r\n");
	printf("is not completed, the test will fail. \r\n");
	printf("------------------------------------------------------------\r\n\r\n");

	//------------------------------------
	// initialize the required bsl modules
	//------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
	
	rtn = SATA_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing SATA!\r\n");
		return (rtn);
	}

	//--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   rtn = sata_DoTest();

	return (rtn);
}



//-----------------------------------------------------------------------------
// \brief   initialize the usb host and otg for use.
//
// \param   none.
//
// \return  uint32_t
//    ERR_NO_ERROR - everything is ok...pmic ready to use.
//    ERR_INIT_FAIL - something happened during initialization.
//-----------------------------------------------------------------------------
uint32_t SATA_init(void)
{
	uint32_t rtn = ERR_NO_ERROR;

	
    //Used by DV only and doing PLL initalization
	pllInit();

    //Used for both AVV and DV for enable minimal PSC required for Primus
    ////pscInit();

    // setting  force mode to bypass clock acknowledge from module
    //CSL_FINS(pscRegs->MDCTL[LPSC_SATA-PSC1_OFFSET], PSC_MDCTL_FORCE, 0x1) ;
	//pscRegs->MDCTL[LPSC_SATA-PSC1_OFFSET] |= CSL_PSC_MDCTL_FORCE_MASK;
	pscRegs->MDCTL[LPSC_SATA] |= CSL_PSC_MDCTL_FORCE_MASK;

    //Enable the LPSC
    //Enable SATA Clk
	//lpscEnable(0, LPSC_SATA);
	EVMAM1808_lpscTransition(PSC1, DOMAIN0, LPSC_SATA, PSC_ENABLE);

    // powerup LJCB & RCD modules
    *(Uint32 *)SATACLK_PWRDN=0x0;

	return rtn;
}


uint32_t sata_DoTest(void)
{
   uint32_t rtn = ERR_NO_ERROR;
   
	// Firmware HwInit Fields Configuration values.
	//  Need to configure this prior to calling sata_init_and_spin_up();
	swCtrlFeatures.capSMPS=1;   // Input Pin exist for external activity detection presence.
	swCtrlFeatures.capSSS=1;    // Always set to 1 in order to avoid spin up when HBA is powered.
	swCtrlFeatures.piPi=1;      // AM-1808 supports a single HBA Port. This should always requires to be set to 1.
	swCtrlFeatures.p0cmdEsp=0;  // The state of this bit is based on the support for eSATA. CAP.SXS setting is the Logical OR of all Ports PxCMD.ESP. If any of the PxCMD.ESP is set, the CAP.SXS will be set too.
	swCtrlFeatures.p0cmdCpd=1;  // Detection of Bus Power Device is supported.
	swCtrlFeatures.p0cmdMpsp=1; // We have bonded out a pin (input) to detect a change on a switch or line
	swCtrlFeatures.p0cmdHpcp=1; // Since ESP is mutually exclusive with HPCP (as mentioned in spec) then HPCP should be set to 1.


	if(chceckSysMemorySize())
		for(;;);	// If program stays here, need to fix alignment issue.

	// Clear all allocated System Memory
	clearCmdList();		// Clear Cmd List allocated within Sys Mem
	clearCmdTables();	// Clear Cmd Tables allocated within Sys Mem
	clearRcvFis();		// Clear Receive FIS allocated within Sys Mem
	clearDmaBuffers();	// Clear all DMA Buffers

	// Perform HBA Reset regardless because it is necessary for Regression on Silicon to force the Device to re-negotiate speed.
	invokeHBAReset();	// If unable to shut one or both DMAs, Perform HBA Reset.

	// Make sure that both DMAs (Cmd List and Rcv FIS) are not running.
	if (placeHbaInIdle())
		invokeHBAReset();	// If unable to shut one or both DMAs, Perform HBA Reset.

	performFirmwareInit();

	printf("\r\n -> Attempting to spin up SATA disk... \r\n");

	//Start the Disk Drive (spin it up)
	progStatus = spinUpDeviceAndWaitForInitToComplete();

	if (progStatus == ERR_NO_ERROR)
	{
		printf("\r\n -> ...disk spin-up success. \r\n");
		rtn = ERR_NO_ERROR;
	}
	else
	{
		printf("\r\n -> ...disk spin-up failed. \r\n");
		rtn = ERR_FAIL;
	}

	return (rtn);
}


