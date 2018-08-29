//***************************************************************************
//  Copyright(c) 2000-2008, Texas Instruments Incorporated. All Rights Reserved
//
//  FILE:   stat_utilities.c Test program for SATA Module
//
//  DESCRIPTION:
//      This file provides functions used by the SATA code to perform common
//      tasks.
//
//  Note: Requires the sata_utilities.h and sata_ahci.h
//
//
//  HISTORY:
//      25July08 Zegeye Alemu - Original version written
//
//***************************************************************************

#include "stdio.h"
#include "types.h"

#include "sata_ahci.h"
#include "sata_utilities.h"
#include "speed_select.h"
#if defined(__TMS470__)
//	#include "arm_int_index.h"
#elif defined(_TMS320C6X)
//	#include "gem_int_index.h"
#endif

#include "test_sata.h"
#include "cslr_sata_001.h"
#include "evmomapl138.h"


// Select one of the following Memory Locations, for the SATA Data to be located at.
// For altering Code Run area, need to modify the .cmd file.
// Select by commenting and uncommenting.
// @@@@@@@@@@@@@@@@@@@@@@@@@@
#if 0	// To place data in GEM L2 Buff set this Directive to 1. To Choose L3 BUF Clear this directive to 0.
	#define GB_L2_BUF
#else
	#define L3_BUF
#endif
// @@@@@@@@@@@@@@@@@@@@@@@@@@

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// -----------  Command Header ----------------
// Command List requires to be 1k Byte Aligned
#ifdef GB_L2_BUF
	#pragma DATA_SECTION(CmdLists,     ".GB_L2_BUF");
#endif
#ifdef L3_BUF
	#pragma DATA_SECTION(CmdLists,     ".far");
#endif
#pragma DATA_ALIGN(CmdLists, 1024);
CmdListHeader CmdLists[32]={0};

// -----------  Command Table -----------------
// Command Table requires 128 Byte Data Alignment
// Make sure each CmdTable Array is 128 Bytes Aligned. For it to happen you
//  need to make sure Structure "CommandTable" size is a minimum of 128 Bytes
//  if using an array to represent your supported number of command tables.
//  Since the size of the PRD length supported controls the size of the
//  Command table, I have forced for Structure "CommandTable" member
//  "prdTable" to be fixed to 8 since this is a minimum size needed to meet
//  the minimum size required. This means that if the PRD Size for a Single
//  Command Header is greater than 8, then this program requires updating.
//  see "CommandTable" Structure definition within "sata_ahci.h" file.
#ifdef GB_L2_BUF
	#pragma DATA_SECTION(CmdTable,     ".GB_L2_BUF");
#endif
#ifdef L3_BUF
	#pragma DATA_SECTION(CmdTable,     ".far");
#endif
#pragma DATA_ALIGN(CmdTable, 128);
CommandTable CmdTable[LISTLENGTH];

/*
#if sizeof(CmdTable[LISTLENGTH-1]) != 128
#error Command Table is not 128 Bytes aligned.
#error Command Table size is sizeof(CmdTable[LISTLENGTH-1])
#endif
*/


// DMA Data Buffer
unsigned char prdTableDataBuff[LISTLENGTH][PRDLENGTH][DATABUFFERLEN];

// -----------  Receive FIS -------------------
// Receive FIS requires 256 bytes alignment
#ifdef GB_L2_BUF
	#pragma DATA_SECTION(RcvFis,     ".GB_L2_BUF");
#endif
#ifdef L3_BUF
	#pragma DATA_SECTION(RcvFis,     ".far");
#endif
#pragma DATA_ALIGN(RcvFis, 256);
ReceiveFis RcvFis;
// --------------------------------------------

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//This should defined (Commented/UnCommented within the speed_select.h file).
//#define _SILICON_

#ifdef _SILICON_
// The below belong to Gen1/2i timing (applies for VDB)
#if 0
		#define MPY                         8 // bits3:0     4 // Both VDB and EVM Clock Sources are 100MHz
		#define LB                          0 // bits5:4     2
		#define LOS                         1 // bit6        1
		#define RXINVPAIR                   0 // bit7        1
		#define RXTERM                      0 // bits9:8     2
		#define RXCDR                       0 // bits12:10   3
		#define RXEQ                        1 // bits16:13   4
		#define TXINVPAIR                   0 // bit17       1
		#define TXCM                        0 // bit18       1
		#define TXSWING                     3 // bits21:19   3
		#define TXDE                        0 // bits25:22   4
		// RESERVED                              bits29:26   4
		#define OVERRIDE                    0 // bit30       1
		#define ENPLL                       1 // bit31       1
#endif
// The below pertains to Gen2m timings (applies to Feon EVM)
#if 1
		#define MPY                         8 // bits3:0     4 // Both EVM and VDB Clock Sources are 100MHz
		#define LB                          0 // bits5:4     2 // Depends on the REFCLK Jitter but start with 00b
		#define LOS                         1 // bit6        1 // Should always be '1', I think
		#define RXINVPAIR                   0 // bit7        1 // Unless swapped by the H/W Designer for routing purposes it should be 0 (not inverted)
		#define RXTERM                      1 // bits9:8     2 // ??? Common Mode Voltage is speced to be 50 mVp-p PHY reg display time parameter not voltage.
		#define RXCDR                       5 // bits12:10   3 // Going by the max jitter spec. ??? Need to look a bit more on this.
		#define RXEQ                        1 // bits16:13   4 // Use Adaptive
		#define TXINVPAIR                   0 // bit17       1 // Unless swapped by the H/W Designer for routing purposes it should be 0 (not inverted)
		#define TXCM                        0 // bit18       1
		#define TXSWING                     3 // bits21:19   3 // Tx Swing for Gen2m 400mvpp <---> 700mvpp =>TXSWING=011b - Good enough for both Short & Backplane architecture
		#define TXDE                        0 // bits25:22   4
		// RESERVED                              bits29:26   4
		#define OVERRIDE                    0 // bit30       1
		#define ENPLL                       1 // bit31       1
#endif

#endif

/////////////////////////////////////////////////
//Added: Jemiah 
//typedef uint32_t		Uint32
//typedef uint16_t		Uint16

/**
 * @brief   Event Handler pointer
 *
 * Event handlers ought to conform to this type
 */
typedef void (* CSL_IntcEventHandler)(void *);

typedef uint16_t          CSL_BitMask16;
typedef uint32_t          CSL_BitMask32;

#define CSL_INTC_EVENTID_CNT        70               /**< Number of Events in the System */

#define   CSL_INTC_SATAINT             24  //SATA

/**
 * @brief   Global Interrupt enable state
 */
typedef uint32_t CSL_IntcGlobalEnableState;

/**
 * @brief   Interrupt Vector IDs
 */
typedef enum {
    CSL_INTC_VECTID_NMI                 =   1,
    CSL_INTC_VECTID_4                   =   4,  
    CSL_INTC_VECTID_5                   =   5,  
    CSL_INTC_VECTID_6                   =   6,  
    CSL_INTC_VECTID_7                   =   7,  
    CSL_INTC_VECTID_8                   =   8,  
    CSL_INTC_VECTID_9                   =   9,  
    CSL_INTC_VECTID_10                  =   10,  
    CSL_INTC_VECTID_11                  =   11,       
    CSL_INTC_VECTID_12                  =   12,  
    CSL_INTC_VECTID_13                  =   13,
    CSL_INTC_VECTID_14                  =   14,      
    CSL_INTC_VECTID_15                  =   15,
    CSL_INTC_VECTID_COMBINE             =   16,
    CSL_INTC_VECTID_EXCEP               =   17
} CSL_IntcVectId;


/**
 * @brief   Interrupt Event IDs
 */
typedef enum {
    CSL_INTC_EVENTID_0                  =   0,  
    CSL_INTC_EVENTID_1                  =   1,  
    CSL_INTC_EVENTID_2                  =   2,  
    CSL_INTC_EVENTID_3                  =   3,  
    CSL_INTC_EVENTID_4                  =   4,  
    CSL_INTC_EVENTID_5                  =   5,  
    CSL_INTC_EVENTID_6                  =   6,  
    CSL_INTC_EVENTID_7                  =   7,  
    CSL_INTC_EVENTID_8                  =   8,  
    CSL_INTC_EVENTID_9                  =   9,  
    CSL_INTC_EVENTID_10                 =   10, 
    CSL_INTC_EVENTID_11                 =   11, 
    CSL_INTC_EVENTID_12                 =   12, 
    CSL_INTC_EVENTID_13                 =   13, 
    CSL_INTC_EVENTID_14                 =   14, 
    CSL_INTC_EVENTID_15                 =   15, 
    CSL_INTC_EVENTID_16                 =   16, 
    CSL_INTC_EVENTID_17                 =   17, 
    CSL_INTC_EVENTID_18                 =   18, 
    CSL_INTC_EVENTID_19                 =   19, 
    CSL_INTC_EVENTID_20                 =   20, 
    CSL_INTC_EVENTID_21                 =   21, 
    CSL_INTC_EVENTID_22                 =   22,  
    CSL_INTC_EVENTID_23                 =   23,  
    CSL_INTC_EVENTID_24                 =   24,  
    CSL_INTC_EVENTID_25                 =   25,  
    CSL_INTC_EVENTID_26                 =   26,  
    CSL_INTC_EVENTID_27                 =   27,  
    CSL_INTC_EVENTID_28                 =   28,  
    CSL_INTC_EVENTID_29                 =   29,  
    CSL_INTC_EVENTID_30                 =   30,  
    CSL_INTC_EVENTID_31                 =   31,  
    CSL_INTC_EVENTID_32                 =   32,  
    CSL_INTC_EVENTID_33                 =   33,  
    CSL_INTC_EVENTID_34                 =   34,
    CSL_INTC_EVENTID_35                 =   35,  
    CSL_INTC_EVENTID_36                 =   36,  
    CSL_INTC_EVENTID_37                 =   37,  
    CSL_INTC_EVENTID_38                 =   38,  
    CSL_INTC_EVENTID_39                 =   39,
    CSL_INTC_EVENTID_40                 =   40,  
    CSL_INTC_EVENTID_41                 =   41,  
    CSL_INTC_EVENTID_42                 =   42,  
    CSL_INTC_EVENTID_43                 =   43,  
    CSL_INTC_EVENTID_44                 =   44,
    CSL_INTC_EVENTID_45                 =   45,  
    CSL_INTC_EVENTID_46                 =   46,  
    CSL_INTC_EVENTID_47                 =   47,  
    CSL_INTC_EVENTID_48                 =   48,  
    CSL_INTC_EVENTID_49                 =   49,
    CSL_INTC_EVENTID_50                 =   50,  
    CSL_INTC_EVENTID_51                 =   51,  
    CSL_INTC_EVENTID_52                 =   52,  
    CSL_INTC_EVENTID_53                 =   53,  
    CSL_INTC_EVENTID_54                 =   54,
    CSL_INTC_EVENTID_55                 =   55,  
    CSL_INTC_EVENTID_56                 =   56,  
    CSL_INTC_EVENTID_57                 =   57,  
    CSL_INTC_EVENTID_58                 =   58,  
    CSL_INTC_EVENTID_59                 =   59,        
    CSL_INTC_EVENTID_60                 =   60,  
    CSL_INTC_EVENTID_61                 =   61,  
    CSL_INTC_EVENTID_62                 =   62,  
    CSL_INTC_EVENTID_63                 =   63,  
    CSL_INTC_EVENTID_64                 =   64,
    CSL_INTC_EVENTID_65                 =   65,  
    CSL_INTC_EVENTID_66                 =   66,  
    CSL_INTC_EVENTID_67                 =   67,  
    CSL_INTC_EVENTID_68                 =   68,  
    CSL_INTC_EVENTID_69                 =   69,  
    CSL_INTC_EVENTID_70                 =   70,
    CSL_INTC_EVENTID_71                 =   71,
    CSL_INTC_EVENTID_72                 =   72,
    CSL_INTC_EVENTID_73                 =   73,
    CSL_INTC_EVENTID_74                 =   74,
    CSL_INTC_EVENTID_75                 =   75,
    CSL_INTC_EVENTID_76                 =   76,
    CSL_INTC_EVENTID_77                 =   77,
    CSL_INTC_EVENTID_78                 =   78,
    CSL_INTC_EVENTID_79                 =   79,
    CSL_INTC_EVENTID_80                 =   80,
    CSL_INTC_EVENTID_81                 =   81,
    CSL_INTC_EVENTID_82                 =   82,
    CSL_INTC_EVENTID_83                 =   83,
    CSL_INTC_EVENTID_84                 =   84,
    CSL_INTC_EVENTID_85                 =   85,
    CSL_INTC_EVENTID_86                 =   86,
    CSL_INTC_EVENTID_87                 =   87,
    CSL_INTC_EVENTID_88                 =   88,
    CSL_INTC_EVENTID_89                 =   89,
    CSL_INTC_EVENTID_90                 =   90,
    CSL_INTC_EVENTID_91                 =   91,
    CSL_INTC_EVENTID_92                 =   92,
    CSL_INTC_EVENTID_93                 =   93,
    CSL_INTC_EVENTID_94                 =   94,
    CSL_INTC_EVENTID_95                 =   95,
    CSL_INTC_EVENTID_96                 =   96,
    CSL_INTC_EVENTID_97                 =   97,
    CSL_INTC_EVENTID_98                 =   98,
    CSL_INTC_EVENTID_99                 =   99,
    CSL_INTC_EVENTID_100                =   100,
    CSL_INTC_EVENTID_101                =   101,
    CSL_INTC_EVENTID_102                =   102,
    CSL_INTC_EVENTID_103                =   103,
    CSL_INTC_EVENTID_104                =   104,
    CSL_INTC_EVENTID_105                =   105,
    CSL_INTC_EVENTID_106                =   106,
    CSL_INTC_EVENTID_107                =   107,
    CSL_INTC_EVENTID_108                =   108,
    CSL_INTC_EVENTID_109                =   109,
    CSL_INTC_EVENTID_110                =   110,
    CSL_INTC_EVENTID_111                =   111,
    CSL_INTC_EVENTID_112                =   112,
    CSL_INTC_EVENTID_113                =   113,
    CSL_INTC_EVENTID_114                =   114,
    CSL_INTC_EVENTID_115                =   115,
    CSL_INTC_EVENTID_116                =   116,
    CSL_INTC_EVENTID_117                =   117,
    CSL_INTC_EVENTID_118                =   118,
    CSL_INTC_EVENTID_119                =   119,
    CSL_INTC_EVENTID_120                =   120,
    CSL_INTC_EVENTID_121                =   121,
    CSL_INTC_EVENTID_122                =   122,
    CSL_INTC_EVENTID_123                =   123,
    CSL_INTC_EVENTID_124                =   124,
    CSL_INTC_EVENTID_125                =   125,
    CSL_INTC_EVENTID_126                =   126,
    CSL_INTC_EVENTID_127                =   127  
} CSL_IntcEventId;

/**
 * @brief   The interrupt handle object
 *
 * This object is used refenced by the handle to identify the event.
 */
typedef struct CSL_IntcObj {
    CSL_IntcEventId eventId;    /**< The event-id */
    CSL_IntcVectId   vectId;    /**< The vector-id */
} CSL_IntcObj;

/**
 * @brief   The interrupt handle
 *
 * This is returned by the CSL_intcOpen(..) API. The handle is used
 * to identify the event of interest in all INTC calls.
 */
typedef struct CSL_IntcObj *    CSL_IntcHandle;


/**
 * @brief   Event Handler Record
 *
 * Used to set-up the event-handler using CSL_intcPlugEventHandler(..)
 */
typedef struct CSL_IntcEventHandlerRecord {
    CSL_IntcEventHandler    handler;    /**< pointer to the event handler */
    void *                  arg;        /**< the argument to be passed to the
                                          handler when it is invoked */
} CSL_IntcEventHandlerRecord;


/**
 * @brief   INTC Module Context
 */
typedef struct {
    CSL_IntcEventHandlerRecord* eventhandlerRecord;
    CSL_BitMask32   eventAllocMask[(CSL_INTC_EVENTID_CNT + 31) / 32];
    uint16_t          numEvtEntries;
    uint8_t            offsetResv[128];
} CSL_IntcContext;



/**
 * @brief   INTC module parameters for open
 *
 * This is equivalent to the Vector Id for the event number.
 */
typedef CSL_IntcVectId CSL_IntcParam;
typedef uint16_t           CSL_Status;

//typedef volatile CSL_SataRegs            *CSL_SataRegsOvly;
#define CSL_SATA_0_REGS                  ((CSL_SataRegs *)   0x01E18000u)
/////////////////////////////////////////////////


unsigned char progStatus=0xFF;
Uint32 Dev28bitLbaAddress;

CSL_SataRegs* sataRegs = CSL_SATA_0_REGS;

FirmwareCtrlFeatures swCtrlFeatures;
/*
  ====================
  0x1F0 = Data Register
  0x1F1 = Features/Error Register
  0x1F2 = Sector Count
  0x1F3 = Sector Number   (LBA Low)
  0x1F4 = Cylinder Low   (LBA Mid)
  0x1F5 = Cylinder High  (LBA High)
  0x1F6 = Drive/Head (Device)
  0x1F7 = Status/Command

  Control Block Registers:
  ====================
  0x3F6 = Dev Ctrl/Alt Status

*/

cmdFis myCmdFis;
dsFis myDsFis;
piosFis myPsFis;
regFis myRegFis;
sdbFis mySdbFis;
uFis myUnknownFis;


Uint32 tempDebug0=0xFFFFFFFF;
Uint32 tempDebug1=0xFFFFFFFF;
Uint32 tempDebug2=0xFFFFFFFF;
Uint32 tempDebug3=0xFFFFFFFF;

// Semaphore for identifying IDENTIFY DEVICE Cmd.
Uint8 PioCmd;

// Command Slot to use for invoking Cmd. Need to be initialized with the slot # prior to configuring Command.
Uint16 cmdSlot2Use = 0;

// Interrupt Count and Semaphore to be set when interrupt rception requires to be communicated.
Uint16 intIsrCnt, intIsrFlag;

interruptHandlingMethod intHandlingMethod; // 	USE_POLLING or USE_INT_HANDLER

// Power Management Capability indicator
pwrMgmtCapability pwrMgmtCap;

// P0CMD.ASP Cfg.
hostPwrMgmtConfig hostPwrMgmtCfg;

//interfaceSpeed intSpeed=GOASFASTASDEVICE; // See enum define within sata_utilitis.h
                                          // Project pre-processor directive may over-ride this setting.

interfaceSpeed intSpeed=DESIRED_SPEED; // See enum define within sata_utilitis.h
                                          // Project pre-processor directive may over-ride this setting.

Uint32 temp=0xABCDEF98;


Uint16 *pIdDevData, IdDevData, sataCapabilities, sataFeaturesSupported, sataFeaturesEnabled;
Uint16 *pIdDevPwrData;
Uint32 p0sctlHighp0sstsLow=0xDEADDEAD, cmdListWrPrdCnt=0xDEADDEAD, cmdListRdPrdCnt=0xDEADDEAD;
sataCapability support4PwrMgmtInitiatedByHost = _NOT_DETERMINED;
sataCapability support4PwrMgmtInitiatedByDevice = _NOT_DETERMINED;
sataCapability support4PwrMgmtInitiatedByDeviceEnabled = _NOT_DETERMINED;
sataCapability support4PwrMgmtInitiatedByDeviceAfterEnable = _NOT_DETERMINED;
sataCapability support4NativeCommandQueuing = _NOT_DETERMINED;
hostPwrMgmtState hbaPwrState;
compareResult wrRdByteCntCompStatus;
interfaceSpeed lineSpeedNegotiated;


Uint32 debugP0sctl[5], debugStat, debugP0stat[5];
extern 	Uint32 debugP0sctl[5], debugStat, debugP0stat[5];


// Some of the features are enabled/disabled during design phase and some features
//  can be enabled/disabled via S/W during power up time. Usually the S/W that does
//  the soft feature configuration is the BIOS. Once it is written, the feature
//  field becomes a "Read Only" field.
void sata_init_and_spin_up(void) {
	//Perform HBA Reset
	//invokeHBAReset();

	//Perform Port Reset
	//invokePortReset();

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// Firmware Initialization
	// Note that you only are able to perform a single write and do want to
	//  agregate all configurations write to take place using a single Write.
//	sataRegs=(CSL_SataRegs *)0x01E18000;
	// Configure CAP[SMPS,SPS=28,27]

	if(chceckSysMemorySize())
		for(;;);	// If program stays here, need to fix alignment issue.

	// Ensure that the Port is idle.
	// Make sure that both DMAs (Cmd List and Rcv FIS) are not running.
	if (progStatus=placeHbaInIdle())
		invokeHBAReset();	// If unable to shut one or both DMAs, Perform HBA Reset.

//	sataRegs->CAP &= 0xE7000000; // Not sure if you are allowed to make multiple
                                 //  Writes so watch out for multiple writes.
	sataRegs->CAP |= ((swCtrlFeatures.capSMPS << 28) |
	                  (swCtrlFeatures.capSSS  << 27)
	                 );

	// Configure PI[31:0]
	sataRegs->PI  |=  (swCtrlFeatures.piPi  << 0);

	// Configure P0CMD[ESP,CPD,MPSP,HPCP=21,20,19,18]
//	sataRegs->P0CMD &= 0xFFC3FFFF;

	sataRegs->P0CMD |= ((swCtrlFeatures.p0cmdEsp << 21) |
                        (swCtrlFeatures.p0cmdCpd << 20) |
                        (swCtrlFeatures.p0cmdMpsp << 19)|
                        (swCtrlFeatures.p0cmdHpcp << 18)
                       );

	//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

	// Configure Line Speed.
//	setSataSpeed(DESIRED_SPEED);  //GOASFASTASDEVICE=0,GEN1=1,GEN2=2

	// Clear Cmd List, Cmd Tables, Receive FIS
//	clearSystemMemory();

	// Clear System Memory Locations in use: Command Table and Receive FISes.
	// Initialize Base Address for the Command List and Receive FISes.
	// Populate Command List Headers.
	// Populate PRD Data Structures.
	associateMem2HBA();


	#ifdef _SILICON_

/*
		#define MPY                         8 // bits3:0     4
		#define LB                          0 // bits5:4     2
		#define LOS                         1 // bit6        1
		#define RXINVPAIR                   0 // bit7        1
		#define RXTERM                      0 // bits9:8     2
		#define RXCDR                       0 // bits12:10   3
		#define RXEQ                        1 // bits16:13   4
		#define TXINVPAIR                   0 // bit17       1
		#define TXCM                        0 // bit18       1
		#define TXSWING                     3 // bits21:19   3
		#define TXDE                        0 // bits25:22   4
		// RESERVED                              bits29:26   4
		#define OVERRIDE                    0 // bit30       1
		#define ENPLL                       1 // bit31       1
*/

		sataRegs->P0PHYCR =  MPY		<< 0	| // 100 MHz Diff Clok Source; Multiply by 15
							 LB			<< 4	| // LB =
							 LOS		<< 6	| // LOS = 1b.
							 RXINVPAIR 	<< 7	|
							 RXTERM		<< 8	|
							 RXCDR		<< 10	|
							 RXEQ		<< 13	| // RXEQ = 0001b (adaptive)
							 TXINVPAIR 	<< 17	|
							 TXCM		<< 18	| // TXCM = 0 because Tx Swing 011b selects 625mV and is less than 700mV
							 TXSWING 	<< 19	| // Tx Swing for Gen2i 400mvpp <---> 700mvpp =>TXSWING=011b
							 TXDE		<< 22	| // Tx DeEmphasis for Gen2i expect to be Zero
							 OVERRIDE 	<< 30	|
							 ENPLL		<< 31;


	#else	// QT
		// Configure PHY Settings
		// Probably want to configure PHY and DMA prior to starting Device (SUD) and enabling
		//  device detection and speed negotiation. Once the speed negotiation finishes
		//  the device is going to send its D2H FIS with its taskfile register content
		//  which needs to be moved onto the Recive FIS space via the Receive DMA.
		sataRegs->P0PHYCR = 0x80000049;
	#endif


	// #define values are in sata_utilities.h
	cfgDmaSetting();

	// Make sure CCC is Disabled.
	disableCCCint();

	// Configure TIMER1MS Register based on your CPU speed to be used by CCC
	initTimer1ms(ONE_MS_TIMER_RESOLUTION); // [0 - 0x3FFFFF], 100000 is the default value.

	// Configure CCC with the values shown below.
	initCCCint(CCC_TIMEOUT_MS_VALUE, CCC_NUM_OF_CMDS); // (timeout Value, number of cmds)

	// Enable CCC Interrupt
//	eanbleCCCint();

	// Enable Receive DMA
	enableRcvFis();


	//Start the Disk Drive (spin it up)
	// If Device Signature is not good, it will stay here.
	if(spinUpDeviceAndWaitForInitToComplete());
		//for(;;);


	// Make sure Interrupt is disabled (Disable at Port Level followed by Global Level).
	// Clear Interrupt at Port Level
	enableDisableInt(PORTint, DISABLE, 0xFFFFFFFF); // clearInt(int type, intState, specificField)
													// int type=GLOBALint or PORTint
													// intState=DISABLE or ENABLE
													// specificField=bit field to Enable or Disable
													//  is used for the RWC feature for PORTint
	// Clear Interrupt at Global Level
	enableDisableInt(GLOBALint, DISABLE, 0);	// clearInt(intType, intState fields2clr)
											// int type=GLOBALint or PORTint
											// intState=DISABLE or ENABLE
											// fields2clr=dontcare for GLOBALint
											//  is used for the RWC feature for PORTint

	// Need to clear interrupts at Port Level followed by Global Level.
	// Ensure all pending Port Error and Status are cleared.
	clearIntOrErrorDiag(ERRORFIELDS, sataRegs->P0SERR);	// Clear P0SERR Register
	// Ensure all pending Port Interrupts and The Single Global Interrupt are cleared.
	clearIntOrErrorDiag(INTFIELDS, sataRegs->P0IS); // Clear P0IS and IS Regs


	// 28 Bit LBA Address of Device. 0xFFFFFFFF is used by test S/W to indicate that it is not initialized
	Dev28bitLbaAddress = 0xFFFFFFFF; // S/W needs to initialize this variable prior to calling
									 //  setupCfisEntriesForDataRdWr();
	// To Enable Interrupts enable Port followed by Global:
	//enableDisableInt(PORTint, ENABLE, 0xFFFFFFFF);;
	//enableDisableInt(GLOBALint, ENABLE, 0);

	// PioCmd is a Semaphore that is set to IDENTIFY DEVICE Cmd when invoking IDENTIFY DEVICE Cmd.
	//  Other times it should be set to another value. Since IDENTIFY DEVICE is a PIO Read Cmd
	//  this semaphore is used to either invoke IDENTIFY DEVICE Cmd or READ SECTORS Cmd.
	//  It should be set prior to calling "setupCfisEntriesForDataRdWr()".
	PioCmd = 0;

	//_INT_DRIVEN_TEST_ is defined within the Project File
	#ifdef _INT_DRIVEN_TEST_
		intHandlingMethod = USE_INT_HANDLER;
	#else
		intHandlingMethod = USE_POLLING;
	#endif

	intIsrCnt=0;
	intIsrFlag=0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

char placeHbaInIdle(void) {
	// To Place HBA In IDLE, need to make sure both DMAs (Cmd List and Rcv FIS) are not running.
	//  Order of Disabling the DMA is important.

// Ensure that the Cmd List DMA is not running
	//  If is running, clear ST and wait for 500ms. Then Check CR.
	if (sataRegs->P0CMD & AHCI_PxCMD_ST) {
		sataRegs->P0CMD &= ~(1 << AHCI_PxCMD_ST_SHIFT);
		waitForXms(WAIT_500_MILLISECONDS);
	}// Wait another 500 Milliseconds for CR to clear. This is twice more than required.
	if (sataRegs->P0CMD & AHCI_PxCMD_CR)
		waitForXms(WAIT_500_MILLISECONDS);

	// If P0CMD.CR is still set, HBA probably has hunged. No need to continue.
	//  Need to perform HBA Reset.
	if (sataRegs->P0CMD & AHCI_PxCMD_CR)
		return(1);

// Ensure that the Receive FIS DMA is running.
	//  If is running, clear FRE and wait for 500ms. Then Check FR.
	if (sataRegs->P0CMD & AHCI_PxCMD_FRE) {
			sataRegs->P0CMD &= ~(1 << AHCI_PxCMD_FRE_SHIFT);
			waitForXms(WAIT_500_MILLISECONDS);
		}// Wait unti FR is Cleared.
	while (sataRegs->P0CMD & AHCI_PxCMD_FR)
		waitForXms(WAIT_500_MILLISECONDS);

	// If P0CMD.FRE is still set, HBA probably has hunged. No need to continue.
	//  Need to perform HBA Reset.
	if (sataRegs->P0CMD & AHCI_PxCMD_FRE)
		return(1);

	return(0);
}

void waitForXms(Uint32 waitValue) {
	Uint32 i=0, j=0;;
	for (i=0; i<waitValue; i++)
		for(j=0; j<ONE_MS_VALUE; j++);
}


void associateMem2HBA(void) {
	cmdSlot2Use = WRITE_CMD_SLOT;

	// Associate Command List Data Structure Base Address to HBA Space.
	// Each Port has its own Command List. CmdLists below implies to a
	//  the number of Command Slots.
	sataRegs->P0CLB=(unsigned int)CmdLists;
	// Associate Command Table with with CmdLists.
	// Since a single HBA Port exists, a Single Command List with a maximum
	//  of 32 Command Header.
	// Here we are using 2 of the 32 Command Header slots so two Command
	//  Tables one for each Command Header is associated.
	CmdLists[WRITE_CMD_SLOT].DW2.CmdTableAddLow=((unsigned int)&CmdTable[WRITE_CMD_SLOT] & 0xFFFFFF80);
	CmdLists[WRITE_CMD_SLOT].DW3.CmdTableAddHigh=0x0;

	CmdLists[READ_CMD_SLOT].DW2.CmdTableAddLow=((unsigned int)&CmdTable[READ_CMD_SLOT] & 0xFFFFFF80);
	CmdLists[READ_CMD_SLOT].DW3.CmdTableAddHigh=0x0;

	// Associate a single PRD Data Buffer with a single Command Header
	// A Single DMA Command could have a scatter/gather requirements and to
	//  demonstrate this capability multiple PRD Descriptors for each command
	//  is necessary. The below demonstrates 2 of the 65536 PRD Descriptors
	//  populated within the Command Table for Command Header 0.

	// Command Header 0 PRD Descriptors 0 & 1 are Initialized.
	CmdTable[WRITE_CMD_SLOT].prdTable[0].DW0.DbaLow=(unsigned int)&prdTableDataBuff[WRITE_CMD_SLOT][0];
	CmdTable[WRITE_CMD_SLOT].prdTable[0].DW1.DbaHigh=0x0;
	CmdTable[WRITE_CMD_SLOT].prdTable[0].DW3.DataBC=DATABUFFERLEN-1;

	CmdTable[WRITE_CMD_SLOT].prdTable[1].DW0.DbaLow=(unsigned int)&prdTableDataBuff[WRITE_CMD_SLOT][1];
	CmdTable[WRITE_CMD_SLOT].prdTable[1].DW1.DbaHigh=0x0;
	CmdTable[WRITE_CMD_SLOT].prdTable[1].DW3.DataBC=DATABUFFERLEN-1;

	// Command Header 1 PRD Descriptors 0 & 1 are Initialized.
	CmdTable[READ_CMD_SLOT].prdTable[0].DW0.DbaLow=(unsigned int)&prdTableDataBuff[READ_CMD_SLOT][0];
	CmdTable[READ_CMD_SLOT].prdTable[0].DW1.DbaHigh=0x0;
	CmdTable[READ_CMD_SLOT].prdTable[0].DW3.DataBC=DATABUFFERLEN-1;

	CmdTable[READ_CMD_SLOT].prdTable[1].DW0.DbaLow=(unsigned int)&prdTableDataBuff[READ_CMD_SLOT][1];
	CmdTable[READ_CMD_SLOT].prdTable[1].DW1.DbaHigh=0x0;
	CmdTable[READ_CMD_SLOT].prdTable[1].DW3.DataBC=DATABUFFERLEN-1;


	// Associate Receive FIS Data Structure Base Address to HBA Space.
	sataRegs->P0FB=(unsigned int)&RcvFis;
}

void enableRcvFis(void) {
	// Enable Receive FIS
	sataRegs->P0CMD |= AHCI_PxCMD_FRE;
	// Wait for the FIS Rcv DMA Engine to start
	while(temp=(sataRegs->P0CMD & AHCI_PxCMD_FR) != AHCI_PxCMD_FR);

}

uint32_t spinUpDeviceAndWaitForInitToComplete(void) {

	uint32_t rtn = ERR_TIMEOUT;
	uint32_t retryCount;


	// Make sure that the HBA is in a Listen Mode prior to Spinning Up Device
	//  Following Configuration is not allowed.
	//      [P0SCTL.DET, P0CMD.SUD] = [1,1] NOT Allowed.
	if((sataRegs->P0SCTL & AHCI_PxSCTL_PxSSTS_DET) != 0)
		sataRegs->P0SCTL &= ~(0xf << AHCI_PxSCTL_PxSSTS_DET_SHIFT);

	// Configure Line Speed.
	setSataSpeed(DESIRED_SPEED);  //GOASFASTASDEVICE=0,GEN1=1,GEN2=2
								  //Preprocessor Directive within Project file.
								  //Other related files @ speed_select.h & sata_utilities.h

// Moved this prior to SUD in order to make sure that HBA updates P0TFD of the
//  status of the Device as it is received.

	// Clear P0SERR.DIAG.X (RWC bit field) so that the P0TFD is updated by HBA.
	sataRegs->P0SERR |= 0x04000000;



	// Spin Up Device.
	sataRegs->P0CMD |= (1 << AHCI_PxCMD_SUD_SHIFT);

	// Wait for Device Detection or/and Speed Negotiation to take place and finish.
//	while (!(((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) == 0x1) ||
//	       ((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) ==0x3))
//	      );

	retryCount = 0;
	while (((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) !=0x3) && (retryCount < DETECT_TEST_RETRY_COUNT))
	{
		USTIMER_delay(DETECT_TEST_RETRY_DELAY);
		retryCount++;
	}

	// Check if the command was successfull or we timed out.
	if ((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) !=0x3)
	{
		rtn = ERR_FAIL;
		return(rtn);
	}	

	// Clear P0SERR.DIAG.X (RWC bit field) so that the P0TFD is updated by HBA.
//	sataRegs->P0SERR |= 0x04000000;

	// Device would send its status and finished power up will send its default
	//  Task file regs and look for Device ready status.
	//while ((sataRegs->P0TFD & AHCI_PxTFD_STS_BSY_DRQ_ERR) != 0);

	//wait for D2H FIS to propagate to Device Memory and avoid early comparison
	// Make sure that the expected Device signature is received.
	retryCount = 0;
	while ((sataRegs->P0SIG != AHCI_P0SIG_SIG_ATA_DEV_GOOD_STAT) && (retryCount < SPINUP_TEST_RETRY_COUNT)) // LBAhigh:LBAmid:LBAlow:SECcnt
	{
		USTIMER_delay(SPINUP_TEST_RETRY_DELAY);
		retryCount++;
	}

	// Check if the command was successfull or we timed out.
	if (sataRegs->P0SIG == AHCI_P0SIG_SIG_ATA_DEV_GOOD_STAT)
	{
		rtn = ERR_NO_ERROR;
	}


	// This also means that Device Signature has been received from Device (D2H FIS)
	// You can check RcvFIS[n]+0x40.

	return(rtn);

}



void setSataSpeed(unsigned char iSpeed) {
	sataRegs->P0SCTL &= ~(AHCI_PxSCTL_PxSSTS_SPD);
	sataRegs->P0SCTL |= (iSpeed << AHCI_PxSCTL_PxSSTS_SPD_SHIFT);
	waitForXms(5); // This might not be necessary: wait a bit
}

void enableDisableInt(Uint8 intType, Uint8 enableDisable, Uint32 intPosition){
	switch (intType) {
		case GLOBALint:
			if(enableDisable==ENABLE)
				sataRegs->GHC |= (1<<1);
			else
				sataRegs->GHC &= ~(1<<1);
			break;

		case PORTint: // Required to Clear P0IS prior to IS.
			if(enableDisable==ENABLE)
				sataRegs->P0IE |= intPosition;
			else
				sataRegs->P0IE &= ~intPosition;
			break;

		default:
			break;
	}
}

void clearIntOrErrorDiag(Uint8 intOrErrorDiagType, Uint32 intErrorPosition){
	switch (intOrErrorDiagType) {
		case ERRORFIELDS:
			sataRegs->P0SERR |= intErrorPosition;
			break;

		case INTFIELDS: // Required to Clear P0IS prior to IS.
			sataRegs->P0IS |= intErrorPosition;
			sataRegs->IS |= 0x1; // RWC bit field: Only Port 0 is Populated.
			break;

		default:
			break;
	}
}



void clearError(Uint32 errorPosition){
	sataRegs->P0SERR |= errorPosition;
}

void initMemory(Uint32 *startAddress, Uint32 length, Uint32 seedWord, Uint32 modifyVal) {
	Uint32 i;
	*startAddress++ = seedWord;
	for (i=0; i<length-1; i++) {
		seedWord += modifyVal;
		*startAddress++ = seedWord;
	}
}


void clearCmdTableAndDmaBuff() {
	//clear Host to Device (Command FIS) Space
	initMemory((Uint32*)CmdLists, (LISTLENGTH*(sizeof(CmdListHeader)/4)), 0, 0);
	//Clear Command FIS and ATAPI Command Spaces for Command Header 0
	initMemory((Uint32 *)&CmdTable[WRITE_CMD_SLOT], (sizeof(CommandFIS)/4)+(sizeof(Atapi)/4), 0, 0);
	//Clear PRD Descriptor Locations for Command Header 0
	initMemory((Uint32*)((Uint32)&CmdTable[WRITE_CMD_SLOT]+0x80), ((sizeof(PRDT)/4)*PRDLENGTH), 0, 0);
	//Clear Command FIS and ATAPI Command Spaces for Command Header 0

	initMemory((Uint32 *)&CmdTable[READ_CMD_SLOT], (sizeof(CommandFIS)/4)+(sizeof(Atapi)/4), 0, 0);
	//Clear PRD Descriptor Locations for Command Header 0
	initMemory((Uint32*)((Uint32)&CmdTable[READ_CMD_SLOT]+0x80), ((sizeof(PRDT)/4)*PRDLENGTH), 0, 0);
	//Clear PRD Data Buffer Memory
	initMemory((Uint32 *)prdTableDataBuff, (LISTLENGTH*PRDLENGTH*DATABUFFERLEN/4), 0, 0);
}

void clearRcvFis() {
	//clear Receive DMA Setup FIS Space.
	initMemory((Uint32*)&RcvFis, (sizeof(DMASetupFis)/4), 0, 0);
	//clear Receive PIO Setup FIS Space.
	initMemory((Uint32*)((Uint32)&RcvFis+0x20), (sizeof(PIOSetupFis)/4), 0, 0);
	//clear Receive Device to Host (D2H) Register FIS Space.
	initMemory((Uint32*)((Uint32)&RcvFis+0x40), (sizeof(D2HRegFis)/4), 0, 0);
	//clear Set Device Bits FIS Space.
	initMemory((Uint32*)((Uint32)&RcvFis+0x58), (sizeof(SetDevBitsFis)/4), 0, 0);
	//clear Unknow FIS Space.
	initMemory((Uint32*)((Uint32)&RcvFis+0x60), (sizeof(UnknownFis)/4), 0, 0);
}

void invokeHBAReset() {
    // HBA Reset will not affect the following Registers settings of PxFB and PxCLB
    //  regs and HwInit fields of Port Registers are not affected.
    // To Do: Check if the Global Registers are affected. Spec mentions not affected.

	// Note: COMRESET OOB will not be sent to attached Device because Freon supports
	//  Staggered Spinup capability and P0CMD.SUD is cleared to Zero when HBA Reset
	//  takes place. Software needs to invoke this if needed.

	// Most likely user want to ensure HBA comes up in its default operation state
	//  or has hung and are unable to idle the port when needing to perform an HBA
	//  reset. Regardless, there is no need to attempt to idle the HBA from
	//  running
	sataRegs->GHC |= (1 << AHCI_GHC_HR_SHIFT);

	// Max Spec time is 1 Second for Reset to complete.
	while((sataRegs->GHC & AHCI_GHC_HR) != 0) {
		waitForXms(WAIT_500_MILLISECONDS);
		waitForXms(WAIT_500_MILLISECONDS);
	}
}

void invokePortReset(void) {
	// Uses OOB COMRESET patern to reset port and device.
	// Set HBA to idle.

/*
When performing a PORT Reset, the physical communication between the HBA and
all device on a port are disabled. This is more intrusive than Software Reset.

When an HBA or port reset occurs, Phy communication shall be re-established
with the device through a COMRESET followed by the normal out-of-band
communication sequence defined in Serial ATA. At the end of the reset, the
device, if working properly, will send a D2H Register FIS, which contains the
device signature.

Steps for perofmring Port Reset. (Steps 1 can NOT be omitted however, Step 2
  is optional, i.e., it is not Mandatory for PxCMD.CR to clear to 0 to perform
  Reset. The Port Reset procedure within the AHCI spec does not explistly
  mention this requirement. However, the NOTE within the description of
  PxSCTL.DET mentions that the DET field can only be written when P0CMD.ST is
  0. Any other time would result in unknown behaviour).

 1. Clear PxCMD.ST
 2. Wait for some time for PxCMD.CR to clear to Zero. (Should clear within 500 Milli Seconds)
 3. Force Controller to issue COMRESET by setting P0SCTL.DET
 4. Wait for at least 1 Milli Second. (This ensures at least a minimum of 1
     COMRESET to be issued)
 5. Clear P0SCTL.DET (Stops Controller from issuing COMRESET, want to move
     beyond COMRESET)
 6. Clear P0SERR register (this allows Task File Register to be updated by HBA)
*/
	debugP0sctl[0]=0;
	debugP0sctl[1]=0;
	debugP0sctl[2]=0;
	debugP0sctl[3]=0;
	debugP0sctl[4]=0;

	debugP0stat[0]=0;
	debugP0stat[1]=0;
	debugP0stat[2]=0;
	debugP0stat[3]=0;
	debugP0stat[4]=0;

	debugStat=0;
	//
	sataRegs->P0CMD &= ~AHCI_PxCMD_ST;	// Make Sure ST is cleared.

	waitForXms(WAIT_500_MILLISECONDS);

	if ((sataRegs->P0CMD & AHCI_PxCMD_CR) == AHCI_PxCMD_CR)
		debugStat=0xFFFFFFFF;	// If CR has not cleared, Port requires Reset.
								//  Just flag the case and continue. As mentioned
								//  above is not mandatory for CR to clear to
								//  continue.

	debugP0sctl[0]=sataRegs->P0SCTL;	// Debug P0SCTL status prior to clearing DET.
	debugP0stat[0]=sataRegs->P0SSTS;	// Debug P0STAT status prior to clearing DET.


	// Choose one of the methods below.
#if 1
	sataRegs->P0SCTL &= ~AHCI_PxSCTL_PxSSTS_DET;	// Clear/Ensure DET field is cleared
#else		// or
	sataRegs->P0SCTL |= 0x4;						// Disable SATA interface and put PHY in Offline mode
#endif

	waitForXms(WAIT_500_MILLISECONDS);

	debugP0sctl[1]=sataRegs->P0SCTL;	// Debug P0SCTL status: Confirm that DET is either 0x0 or 0x4
	debugP0stat[1]=sataRegs->P0SSTS;	// Debug P0STAT status after clearing DET or placing PHY Offline.


	// Perform Interface Communication Initialization sequence.
	//  This will force the controller to send COMRESET continuously.(Equivalent to Hard Reset)
	sataRegs->P0SCTL |= (1 << AHCI_PxSCTL_PxSSTS_DET_SHIFT);
	// wait for at least 1 milli-second or more to make sure one or more COMRESET pattern
	//  is sent to the device.
	waitForXms(WAIT_1_MILLISECOND);

	debugP0sctl[2]=sataRegs->P0SCTL;					// Debug P0SCTL status: Confirm that the Start Initialization request has been coded in
													//  Expect to see P0SCTL.DET to be 0x1.
	debugP0stat[2]=sataRegs->P0SSTS;	// Debug P0STAT status after sending COMRESET and continuing sending COMRESET.


	sataRegs->P0SCTL &= ~(1 << AHCI_PxSCTL_PxSSTS_DET_SHIFT);	// Clear DET bit field.
	waitForXms(WAIT_1_MILLISECOND);

	debugP0sctl[3]=sataRegs->P0SCTL;		// Debug P0SCTL status: Confirm that P0SCTL.DET is cleared to 0.
	debugP0stat[3]=sataRegs->P0SSTS;	// Debug P0STAT status after stoping sending COMRESET.

	clearError(sataRegs->P0SERR);	// Clear P0SERR Register

	waitForXms(WAIT_1_MILLISECOND);

	while((sataRegs->P0SSTS & 0x3) != 0x3);	// Wait at least Device Presence is detected.

	debugP0sctl[4]=sataRegs->P0SCTL;		// Debug P0SCTL status: Confirm P0SCTL.DET state after Port Reset Completion
	debugP0stat[4]=sataRegs->P0SSTS;	// Debug P0STAT status after Port Reset Completion

}

char startCmdListProcessing(void) {
	// Make sure that a device is present and HBA has established communications.
	while ((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) !=0x3);

	// Clear P0SERR.DIAG.X (RWC bit field) so that the P0TFD is updated by HBA.
	// Make sure it is cleared.
	sataRegs->P0SERR |= 0x04000000;

	// Make sure the Command List is not Running.

//	if (sataRegs->P0CMD & AHCI_PxCMD_CR)
//		return(1);
	//  Task file regs and look for Device ready status.
	while ((sataRegs->P0TFD & AHCI_PxTFD_STS_BSY_DRQ_ERR) != 0);

	// Make sure the the Receive FIS DMA is running.
	if ((sataRegs->P0CMD & (AHCI_PxCMD_FRE | AHCI_PxCMD_FRE)) !=
	                         (AHCI_PxCMD_FRE | AHCI_PxCMD_FRE))
		return(1);

	// Enable the Cmd List DMA Engine.
	sataRegs->P0CMD |= AHCI_PxCMD_ST;

	// Wait here a bit until the Command List DMA Engine has started to run
	while ((sataRegs->P0CMD & AHCI_PxCMD_CR) == 0)
		waitForXms(1);

	return(0);
}

Uint32 debugP0CIreg;
extern Uint32 debugP0CIreg;

char submitCmd(Uint8 commandType, Uint8 commandSlot) {
	// Make sure both the Command List and Receive FIS DMAs are eanbled and running prior to
	//  submiting command
	if ((sataRegs->P0CMD & (AHCI_PxCMD_FRE | AHCI_PxCMD_FRE | AHCI_PxCMD_CR | AHCI_PxCMD_ST)) !=
	                         (AHCI_PxCMD_FRE | AHCI_PxCMD_FRE | AHCI_PxCMD_CR | AHCI_PxCMD_ST))
		return(1);

	switch (commandType) {
			case NON_QUEUED_CMD:
				sataRegs->P0CI |= (0x1 << commandSlot);
				break;

			case QUEUED_CMD:
				sataRegs->P0SACT |= (0x1 << commandSlot);
				sataRegs->P0CI |= (0x1 << commandSlot);
				break;

			default:
				break;
	}
	debugP0CIreg=sataRegs->P0CI;
	return(0);
}

void submitBatchNCQcmds(Uint32 ncqCmd) {
	sataRegs->P0SACT |= ncqCmd;
	sataRegs->P0CI |= ncqCmd;
}


Uint32 getRegStatus(Uint32 * pIntReg, Uint32 intFieldMask) {
	return(*pIntReg & intFieldMask);
}


char setupCfisEntriesForDataRdWr(CmdListHeader *CmdListNum, dataXferDir readOrWrite, xferProtocol xferType) {
    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command Header *******
	// Other part of the Command List Structure, with the exception of Word 0 for the
	//  Command Slots[0] and [1] are already initialized when invoking sata_init_and_spin_up()
	//  function via associateMem2HBA() function.
	// Configure Word 0 of Command List
	CmdListNum->DW0.CmdLen=5;   // This is the length of H2D FIS. This might need changing
							    //   based on the Command issued to Device. Need to Check.
    CmdListNum->DW0.Atapi=0;    // Command is destined to HDD Like Device.
	CmdListNum->DW0.Prefetch=1; // Doesn't hurt prefetching so do it.
								// WARNING: Do Not Prefetch if using:
								//          => Command Queuing
								//          => Port Multiplier
	CmdListNum->DW0.Reset=0;    // This is normally set to Zero unless a Soft Reset is required.
	CmdListNum->DW0.Bist=0;     // This is for entering test mode and should be cleared for normal operation.
	CmdListNum->DW0.Rok=0;      // For Normal operation require to Clear this bit so P0TFD and P0CI are modified by HBA as appropriate.
								//  Rok should be set for S/W Reset Command.
	CmdListNum->DW0.Pmp=0x0;    // Used only if an external Port Multiplier is attached and selects the Port of the Port Multiplier.
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	// The above DW0 fields usually would not change for Normal operation.

	if (readOrWrite == DATA_DIR_WR) // The Write setting here is based on the Data FIS direction.
		CmdListNum->DW0.Write=1;    //  Write=1/0=>Write/Read;
	else if (readOrWrite == DATA_DIR_RD) // The Write setting here is based on the Data FIS direction.
		CmdListNum->DW0.Write=0;    //  Write=1/0=>Write/Read;
	else return(1);

	CmdListNum->DW0.Prdtl=PRDLENGTH;    // Need to update this when using DMA for Data transfer.

    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command FIS (H2D FIS) ******

	// Cmd FIS is made of a 20 Bytes size FIS.
	// FIS Fieds are Initialized into the alloted data buffers.
	// Strucutre type 'cmdFis' holds 8 of the 20 bytes of the FIS. These
	//  seven elements of the cfis are good enough for majority of command
	//  building tasks.
	// If need to access other cfis members, need to access the cfis member
	//  directly. Example of how to access the FIS Type, cfis Byte0.
	// 	CmdTable[n].cfis.DW0.B0FisType=0x27;
	// FIS Type is hard coded within buildCmdFis function and Reserved Locations
	//  are also cleared to Zeros, so no need of iniitalizing this bytes is necessary here.

	// Make sure the Device 28 Bit LBA Address is initialize prior to calling this function.
	//  Dev28bitLbaAddress               = 0x01234567; // 28-Bit LBA Address
	if (Dev28bitLbaAddress == 0xFFFFFFFF)
		return(1);

	myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CMD_UPDATE;	// Bit7 of Byte1 is set for Command Wr and Cleared for Control Wr
	//myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CTRL_UPDATE; // Bit7 of Byte1 is Cleared for Command Control Wr.

	if (readOrWrite == DATA_DIR_WR) {
		if (xferType == DMA_PROTOCOL) {
			myCmdFis.cfisCmd               = ATA_CMD_WRITE_DMA; // Uses 28-Bit Addressing.
			//myCmdFis.cfisCmd               = ATA_CMD_WRITE_DMA_EXT; // Uses 48-Bit Addressing.
		} else {
			myCmdFis.cfisCmd               = ATA_CMD_WRITE_SECTOR; // PIO Write Command: Uses 28-Bit Addressing
		}
	}
	else {
		if (xferType == DMA_PROTOCOL) {
			myCmdFis.cfisCmd               = ATA_CMD_READ_DMA; // Uses 28-Bit Addressing.
			//myCmdFis.cfisCmd               = ATA_CMD_READ_DMA_EXT; // Uses 48-Bit Addressing.
		} else {
			if (PioCmd == ATA_CMD_IDENTIFY_DEVICE)
				myCmdFis.cfisCmd               = ATA_CMD_IDENTIFY_DEVICE; // PIO Read Command: Uses 28-Bit Addressing.
			else
				myCmdFis.cfisCmd               = ATA_CMD_READ_SECTOR; // PIO Read Command: Uses 28-Bit Addressing.
		}
	}

	myCmdFis.cfisFeature           = 0x00;
	myCmdFis.cfisDw1SecNumLbaLow   = (Uint8) Dev28bitLbaAddress;
	myCmdFis.cfisDw1CylLowLbaMid   = (Uint8)(Dev28bitLbaAddress>>8);
	myCmdFis.cfisDw1CylHighLbahigh = (Uint8)(Dev28bitLbaAddress>>16);
	myCmdFis.cfisDw1Dev            = ( DEVICE_REG_USE_LBA_ADDRESSING |
	                                  (Uint8)(Dev28bitLbaAddress>>24)
	                                 );
	myCmdFis.cfisDw3SecCnt         = (PRDLENGTH*DATABUFFERLEN)/512;
	myCmdFis.cfisDw3Ctrl           = 0x00;

	// The below require to be initialized at least once and can be ignored especially if
	//  not using 48-Bit Addressing. If use 48-Bit Addressing, then require constant
	//  maintenance prior to invoking a command.
	myCmdFis.cfisDw2SecNumLbaLowExp=0x00;
	myCmdFis.cfisDw2CylLowLbaMidExp=0x00;
	myCmdFis.cfisDw2CylHighLbahighExp=0x00;
	myCmdFis.cfisDw2FeatureExp=0x00;
	myCmdFis.cfisDw3SecCntExp=0x00;

	// Invalidate for future use.
	Dev28bitLbaAddress = 0xFFFFFFFF;

	return(0);
}


// Call the below when using only Native Command Queuing right after calling
//  setupCfisEntriesForDataRdWr();
void modifyCfisEntries4NCQ(CmdListHeader *CmdListNum, dataXferDir readOrWrite) {
	CmdListNum->DW0.Prefetch=1; // WARNING: Do Not Prefetch if using:
								//          => Command Queuing
	if (readOrWrite == DATA_DIR_WR)
		myCmdFis.cfisCmd               = ATA_CMD_FPDMA_WRITE;
	else
		myCmdFis.cfisCmd               = ATA_CMD_FPDMA_READ;


	myCmdFis.cfisFeature           = (PRDLENGTH*DATABUFFERLEN)/512; // Feature holds Sector Count 7:0
	myCmdFis.cfisDw2FeatureExp     = 0x0; // Feature Exp holds Sector Count 15:8

	myCmdFis.cfisDw1SecNumLbaLow   = (Uint8) Dev28bitLbaAddress;
	myCmdFis.cfisDw1CylLowLbaMid   = (Uint8)(Dev28bitLbaAddress>>8);
	myCmdFis.cfisDw1CylHighLbahigh = (Uint8)(Dev28bitLbaAddress>>16);
	myCmdFis.cfisDw1Dev            = 0x40; // Device[7,6,5,4,3:0] = [FUA,1,Reserved,0,Reserved] respectively.
	myCmdFis.cfisDw3SecCnt         = cmdSlot2Use << 3; // Sector Counts 7:3 Holds TAG, Sector Count 2:0 is Reserved
	myCmdFis.cfisDw3Ctrl           = 0x00;
}



void buildCmdFis(CommandTable *CmdSlotNum) {
//    +----------+----------+--------------+-----------+
// DW0|  FEATURE | COMMAND  | c r r r port |FISTYPE 27h|
//    +----------+----------+--------------+-----------+
// DW1|  DEVICE  | LBA HIGH |  LBA MID     |  LBA LOW  |
//    +----------+----------+--------------+-----------+
// DW2|FETURESexp|LBAHIGHexp|  LBAMIDexp   | LBALOWexp |
//    +----------+----------+--------------+-----------+
// DW3| CONTROL  | RESERVED | SEC CNTexp   |  SEC CNT  |
//    +----------+----------+--------------+-----------+
// DW4| RESERVED | RESERVED |  RESERVED    | RESERVED  |
//    +----------+----------+--------------+-----------+
	CmdSlotNum->cfis.DW0.B0FisType=0x27;
	CmdSlotNum->cfis.DW0.BYTE1=myCmdFis.cfisByte1;                 //Make Sure the 'C' bit field is correctly set or cleared.
	CmdSlotNum->cfis.DW0.B2Cmd=myCmdFis.cfisCmd;
	CmdSlotNum->cfis.DW0.B3Feature=myCmdFis.cfisFeature;

	CmdSlotNum->cfis.DW1.B0LbaLow=myCmdFis.cfisDw1SecNumLbaLow;
	CmdSlotNum->cfis.DW1.B1LbaMid=myCmdFis.cfisDw1CylLowLbaMid;
	CmdSlotNum->cfis.DW1.B2LbaHigh=myCmdFis.cfisDw1CylHighLbahigh;
	CmdSlotNum->cfis.DW1.B3Device=myCmdFis.cfisDw1Dev;          //Make Sure 48-Bit or 28-Bit Addressing is indicated here.


	CmdSlotNum->cfis.DW2.B0LbaLowExp=myCmdFis.cfisDw2SecNumLbaLowExp;    //0x0;
	CmdSlotNum->cfis.DW2.B1LbaMidExp=myCmdFis.cfisDw2CylLowLbaMidExp;    //0x0;
	CmdSlotNum->cfis.DW2.B2LbaHighExp=myCmdFis.cfisDw2CylHighLbahighExp; //0x0;
	CmdSlotNum->cfis.DW2.B3FeatureExp=myCmdFis.cfisDw2FeatureExp;        //0x0;

	CmdSlotNum->cfis.DW3.B0SecCnt=myCmdFis.cfisDw3SecCnt;
	CmdSlotNum->cfis.DW3.B1SecCntExp=myCmdFis.cfisDw3SecCntExp;          //0x0;
	CmdSlotNum->cfis.DW3.B2Rsv=0x0;
	CmdSlotNum->cfis.DW3.B3Control=myCmdFis.cfisDw3Ctrl;

	CmdSlotNum->cfis.DW4.DWResv=0x0;
}

void getCmdFis(CommandTable *CmdSlotNum) {
// Host to Device (H2D) or Command FIS.
//    +----------+----------+--------------+-----------+
// DW0|  FEATURE | COMMAND  | c r r r port |FISTYPE 27h|
//    +----------+----------+--------------+-----------+
// DW1|  DEVICE  | LBA HIGH |  LBA MID     |  LBA LOW  |
//    +----------+----------+--------------+-----------+
// DW2|FETURESexp|LBAHIGHexp|  LBAMIDexp   | LBALOWexp |
//    +----------+----------+--------------+-----------+
// DW3| CONTROL  | RESERVED | SEC CNTexp   |  SEC CNT  |
//    +----------+----------+--------------+-----------+
// DW4| RESERVED | RESERVED |  RESERVED    | RESERVED  |
//    +----------+----------+--------------+-----------+
	myCmdFis.cfisType                 = CmdSlotNum->cfis.DW0.B0FisType;
	myCmdFis.cfisByte1                = CmdSlotNum->cfis.DW0.BYTE1;
	myCmdFis.cfisCmd                  = CmdSlotNum->cfis.DW0.B2Cmd;
	myCmdFis.cfisFeature              = CmdSlotNum->cfis.DW0.B3Feature;
	myCmdFis.cfisDw1SecNumLbaLow      = CmdSlotNum->cfis.DW1.B0LbaLow;
	myCmdFis.cfisDw1CylLowLbaMid      = CmdSlotNum->cfis.DW1.B1LbaMid;
	myCmdFis.cfisDw1CylHighLbahigh    = CmdSlotNum->cfis.DW1.B2LbaHigh;
	myCmdFis.cfisDw1Dev               = CmdSlotNum->cfis.DW1.B3Device;
	myCmdFis.cfisDw3SecCnt            = CmdSlotNum->cfis.DW3.B0SecCnt;
	myCmdFis.cfisDw3Ctrl              = CmdSlotNum->cfis.DW3.B3Control;

	myCmdFis.cfisDw2SecNumLbaLowExp   = CmdSlotNum->cfis.DW2.B0LbaLowExp;
	myCmdFis.cfisDw2CylLowLbaMidExp   = CmdSlotNum->cfis.DW2.B1LbaMidExp;
	myCmdFis.cfisDw2CylHighLbahighExp = CmdSlotNum->cfis.DW2.B2LbaHighExp;
	myCmdFis.cfisDw2FeatureExp        = CmdSlotNum->cfis.DW2.B3FeatureExp;
	myCmdFis.cfisDw3SecCntExp         = CmdSlotNum->cfis.DW3.B1SecCntExp;
}

void getDmaSetupFis(ReceiveFis *RcvFisPtr) {
// DMA Setup FIS
//    +----------+----------+--------------+-----------+
// DW0| RESERVED | RESERVED | a i d r port |FISTYPE 41h|
//    +----------+----------+--------------+-----------+
// DW1|         DMA BUFFER IDENTIFIER LOW              |
//    +------------------------------------------------+
// DW2|         DMA BUFFER IDENTIFIER HIGH             |
//    +------------------------------------------------+
// DW3|                RESERVED                        |
//    +------------------------------------------------+
// DW4|            DMA BUFFER OFFSET                   |
//    +------------------------------------------------+
// DW5|            DMA TRANSFER COUNT                  |
//    +------------------------------------------------+
// DW6|                RESERVED                        |
//    +------------------------------------------------+
	myDsFis.dsfisType             = RcvFisPtr->DSFIS.DW0.B0FisType;
	myDsFis.dsfisByte1            = RcvFisPtr->DSFIS.DW0.BYTE1;
	myDsFis.dsfisDw1DmaBuffLow    = RcvFisPtr->DSFIS.DW1DmaBuffLow;
	myDsFis.dsfisDw2DmaBuffHigh   = RcvFisPtr->DSFIS.DW2DmaBuffHigh;
	myDsFis.dsfisDw4DmaBuffOffset = RcvFisPtr->DSFIS.DW4DmaBuffOffset;
	myDsFis.dsfisDw5DmaXferCnt    = RcvFisPtr->DSFIS.DW5DmaXfrCnt;
}

void getPioSetupFis(ReceiveFis *RcvFisPtr) {
	// Receive PIO Setup FIS
//    +----------+----------+--------------+-----------+
// DW0|   ERROR  |  STATUS  | r i d r port |FISTYPE 5Fh|
//    +----------+----------+--------------+-----------+
// DW1|  DEVICE  | LBA HIGH |  LBA MID     |  LBA LOW  |
//    +----------+----------+--------------+-----------+
// DW2| RESERVED |LBAHIGHexp|  LBAMIDexp   | LBALOWexp |
//    +----------+----------+--------------+-----------+
// DW3| E_STATUS | RESERVED | SEC CNTexp   |  SEC CNT  |
//    +----------+----------+--------------+-----------+
// DW4| RESERVED | RESERVED |     TRANSFER COUNT       |
//    +----------+----------+--------------------------+
	myPsFis.psfisType               = RcvFisPtr->PSFIS.DW0.B0FisType;
	myPsFis.psfisByte1              = RcvFisPtr->PSFIS.DW0.BYTE1;
	myPsFis.psfisStatus             = RcvFisPtr->PSFIS.DW0.B2Status;
	myPsFis.psfisError              = RcvFisPtr->PSFIS.DW0.B3Errror;
	myPsFis.psfisDw1SecNumLbaLow    = RcvFisPtr->PSFIS.DW1.B0LbaLow;
	myPsFis.psfisDw1CylLowLbaMid    = RcvFisPtr->PSFIS.DW1.B1LbaMid;
	myPsFis.psfisDw1CylHighLbahigh  = RcvFisPtr->PSFIS.DW1.B2LbaHigh;
	myPsFis.psfisDw1Dev             = RcvFisPtr->PSFIS.DW1.B3Device;
	myPsFis.psfisDw3SecCnt          = RcvFisPtr->PSFIS.DW3.B0SecCnt;
	myPsFis.psfisDw3Estatus         = RcvFisPtr->PSFIS.DW3.B3Estatus;
	myPsFis.psfisDw4XferCnt         = RcvFisPtr->PSFIS.DW4.HW0XferCnt;
}

void getRegDev2HostFis(ReceiveFis *RcvFisPtr) {
	// Receive Device to Host FIS (RFIS)
//    +----------+----------+--------------+-----------+
// DW0|   ERROR  |  STATUS  | r i r r port |FISTYPE 34h|
//    +----------+----------+--------------+-----------+
// DW1|  DEVICE  | LBA HIGH |  LBA MID     |  LBA LOW  |
//    +----------+----------+--------------+-----------+
// DW2| RESERVED |LBAHIGHexp|  LBAMIDexp   | LBALOWexp |
//    +----------+----------+--------------+-----------+
// DW3| RESERVED | RESERVED | SEC CNTexp   |  SEC CNT  |
//    +----------+----------+--------------+-----------+
// DW4| RESERVED | RESERVED |   RESERVED   | RESERVED  |
//    +----------+----------+--------------+-----------+
	myRegFis.regfisType               = RcvFisPtr->RFIS.DW0.B0FisType;
	myRegFis.regfisByte1              = RcvFisPtr->RFIS.DW0.BYTE1;
	myRegFis.regfisStatus             = RcvFisPtr->RFIS.DW0.B2Status;
	myRegFis.regfisError              = RcvFisPtr->RFIS.DW0.B3Errror;
	myRegFis.regfisDw1SecNumLbaLow    = RcvFisPtr->RFIS.DW1.B0LbaLow;
	myRegFis.regfisDw1CylLowLbaMid    = RcvFisPtr->RFIS.DW1.B1LbaMid;
	myRegFis.regfisDw1CylHighLbahigh  = RcvFisPtr->RFIS.DW1.B2LbaHigh;
	myRegFis.regfisDw1Dev             = RcvFisPtr->RFIS.DW1.B3Device;
	myRegFis.regfisDw3SecCnt          = RcvFisPtr->RFIS.DW3.B0SecCnt;
}

void getSetDevBitsFis(ReceiveFis *RcvFisPtr) {
	// Set Device Bits FIS
//    +----------+----------+--------------+-----------+
// DW0|   ERROR  |  STATUS  | n i r r port |FISTYPE A1h|
//    +------------------------------------------------+
// DW1|                  RESERVED                      |
//    +------------------------------------------------+
	mySdbFis.sdbfisType   = RcvFisPtr->SDBFIS.DW0.B0FisType;
	mySdbFis.sdbfisByte1  = RcvFisPtr->SDBFIS.DW0.BYTE1;
	mySdbFis.sdbfisStatus = RcvFisPtr->SDBFIS.DW0.B2Status;
	mySdbFis.sdbfisError  = RcvFisPtr->SDBFIS.DW0.B3Errror;
}

void getUnkownFis(ReceiveFis *RcvFisPtr) {
	// Unkown FIS
	unsigned char i;
	for (i=0; i<16; i++)
		myUnknownFis.ufisWord[i] = RcvFisPtr->UFIS.DW[i].UserDefined;
}

void testWriteRegFis(ReceiveFis *RcvFisPtr) {
	// Register FIS - Device to Host (D2H)
	RcvFisPtr->RFIS.DW0.B0FisType=0x34;
	RcvFisPtr->RFIS.DW0.BYTE1=0xEF;
	RcvFisPtr->RFIS.DW0.B2Status=0x11;
	RcvFisPtr->RFIS.DW0.B3Errror=0x81;

	RcvFisPtr->RFIS.DW1.B0LbaLow=0x21;
	RcvFisPtr->RFIS.DW1.B1LbaMid=0x43;
	RcvFisPtr->RFIS.DW1.B2LbaHigh=0x65;
	RcvFisPtr->RFIS.DW1.B3Device=0x87;

	RcvFisPtr->RFIS.DW2.B0LbaLowExp=0xBA;
	RcvFisPtr->RFIS.DW2.B1LbaMidExp=0xDC;
	RcvFisPtr->RFIS.DW2.B2LbaHighExp=0xFE;
	RcvFisPtr->RFIS.DW2.B3Rsv=0x21;

	RcvFisPtr->RFIS.DW3.B0SecCnt=0x12;
	RcvFisPtr->RFIS.DW3.B1SecCntExp=0xEF;
	RcvFisPtr->RFIS.DW3.HW1Rsv=0xCDBA;

	RcvFisPtr->RFIS.DW4.W0Rsv=0xACEDDECA;
}


void testWriteDsFis(ReceiveFis *RcvFisPtr) {
	// DMA Setup FIS
	RcvFisPtr->DSFIS.DW0.B0FisType=0x41;
	RcvFisPtr->DSFIS.DW0.BYTE1=0xFE;
	RcvFisPtr->DSFIS.DW0.B2Rsv=0xDD;
	RcvFisPtr->DSFIS.DW0.B3Rsv=0xEE;


	RcvFisPtr->DSFIS.DW1DmaBuffLow=0xFEDCBA98;
	RcvFisPtr->DSFIS.DW2DmaBuffHigh=0x89ABCDEF;
	RcvFisPtr->DSFIS.DW3Rsv=0xDEADBEEF;
	RcvFisPtr->DSFIS.DW4DmaBuffOffset=0x76543218;
	RcvFisPtr->DSFIS.DW5DmaXfrCnt=0x99994444;
	RcvFisPtr->DSFIS.DW6Rsv=0xBEEFDEAD;
}


void testWritePsFis(ReceiveFis *RcvFisPtr) {
	// PIO Setup FIS
	RcvFisPtr->PSFIS.DW0.B0FisType=0x5F;
	RcvFisPtr->PSFIS.DW0.BYTE1=0xDC;
	RcvFisPtr->PSFIS.DW0.B2Status=0x88;
	RcvFisPtr->PSFIS.DW0.B3Errror=0x99;

	RcvFisPtr->PSFIS.DW1.B0LbaLow=0x4E;
	RcvFisPtr->PSFIS.DW1.B1LbaMid=0xBA;
	RcvFisPtr->PSFIS.DW1.B2LbaHigh=0x77;
	RcvFisPtr->PSFIS.DW1.B3Device=0x66;

	RcvFisPtr->PSFIS.DW2.B0LbaLowExp=0x3D;
	RcvFisPtr->PSFIS.DW2.B1LbaMidExp=0xBA;
	RcvFisPtr->PSFIS.DW2.B2LbaHighExp=0xAA;
	RcvFisPtr->PSFIS.DW2.B3Rsv=0x55;

	RcvFisPtr->PSFIS.DW3.B0SecCnt=0x2F;
	RcvFisPtr->PSFIS.DW3.B1SecCntExp=0xCD;
	RcvFisPtr->PSFIS.DW3.B2Rsv=0x22;
	RcvFisPtr->PSFIS.DW3.B3Estatus=0x44;

	RcvFisPtr->PSFIS.DW4.HW0XferCnt=0xABCD;
	RcvFisPtr->PSFIS.DW4.HW1Rsv=0xDEAD;
}

void testWriteSdbFis(ReceiveFis *RcvFisPtr) {
	// Set Device Bits FIS
	RcvFisPtr->SDBFIS.DW0.B0FisType=0xA1;
	RcvFisPtr->SDBFIS.DW0.BYTE1=0x64;
	RcvFisPtr->SDBFIS.DW0.B2Status=0x75;
	RcvFisPtr->SDBFIS.DW0.B3Errror=0x86;
}

void testWriteUnknownFis(ReceiveFis *RcvFisPtr) {
// Unknown FIS
	RcvFisPtr->UFIS.DW[0].UserDefined=0x11111111;
	RcvFisPtr->UFIS.DW[1].UserDefined=0x22222222;
	RcvFisPtr->UFIS.DW[2].UserDefined=0x33333333;
	RcvFisPtr->UFIS.DW[3].UserDefined=0x44444444;
	RcvFisPtr->UFIS.DW[4].UserDefined=0x55555555;
	RcvFisPtr->UFIS.DW[5].UserDefined=0x66666666;
	RcvFisPtr->UFIS.DW[6].UserDefined=0x77777777;
	RcvFisPtr->UFIS.DW[7].UserDefined=0x88888888;
	RcvFisPtr->UFIS.DW[8].UserDefined=0x99999999;
	RcvFisPtr->UFIS.DW[9].UserDefined=0xAAAAAAAA;
	RcvFisPtr->UFIS.DW[10].UserDefined=0xBBBBBBBB;
	RcvFisPtr->UFIS.DW[11].UserDefined=0xCCCCCCCC;
	RcvFisPtr->UFIS.DW[12].UserDefined=0xDDDDDDDD;
	RcvFisPtr->UFIS.DW[13].UserDefined=0xEEEEEEEE;
	RcvFisPtr->UFIS.DW[14].UserDefined=0xFFFFFFFF;
	RcvFisPtr->UFIS.DW[15].UserDefined=0x12345678;
}

void initTimer1ms(Uint32 msTimeValue) {
	if (msTimeValue > 0x3FFFFF)
		msTimeValue = 0x3FFFFF;
	sataRegs->TIMER1MS = msTimeValue;
}


void initCCCint(Uint16 timeOutValue, Uint16 numOfCommands) {
	enableDisableInt(PORTint, ENABLE,
	                 ( AHCI_P0IE_ALL_FIELDS &
					   (~(AHCI_P0IE_DHRE |
	                   AHCI_P0IE_SDBE))
					 )); // Turn off D2H and SDBE Interrupt
	// Make sure CCC is not enabled
//	sataRegs->CCCCTL   &= (~AHCI_CCCCTL_EN);
	sataRegs->CCCPORTS = 0x1; // There is only a single port available
	// Initialize Time Out Value
	sataRegs->CCCCTL   |= (timeOutValue & 0xFFFF) << AHCI_CCCCTL_TV_SHIFT;
	// Initialize Num of Commands to complete
	sataRegs->CCCCTL   |= (numOfCommands & 0xFF) << AHCI_CCCCTL_CC_SHIFT;
	// Enable CCC
//	sataRegs->CCCCTL   |= AHCI_CCCCTL_EN;
}

void eanbleCCCint(void) {
	// Enable CCC
	sataRegs->CCCCTL   |= AHCI_CCCCTL_EN;
}

void disableCCCint(void) {
	// Disable CCC
	sataRegs->CCCCTL   &= (~AHCI_CCCCTL_EN);
}


void cfgDmaSetting(void) {
	sataRegs->P0DMACR   |= ((DMA_BURST_LENGTH << AHCI_P0DMACR_TXTS_SHIFT)       |
	                        (DMA_BURST_LENGTH << AHCI_P0DMACR_RXTS_SHIFT)       |
	                        (DMA_TRANACTION_LENGTH << AHCI_P0DMACR_TXABL_SHIFT) |
	                        (DMA_TRANACTION_LENGTH << AHCI_P0DMACR_RXABL_SHIFT)
	                       );
}

char chceckSysMemorySize(void) {
	Uint16 i;
	for (i=0; i<LISTLENGTH; i++) {
		if (((Uint32)&CmdTable[i] & 0x7F) != 0)
			break; // If not 128 bytes aligned, fix alignment issue.
		}
	if (i != LISTLENGTH)
		return(1);
	return(0);
}


char compMemContent(Uint32 *mem1, Uint32 *mem2, Uint32 dataSize) {
	Uint32 i;
	Uint16 j;
	j=0;

	for (i=0; i<dataSize; i++) {
		if(*mem1++ != *mem2++) j++;
			//if(j!=0) for(;;); // debug stay here.
		if (j==0xff) break;
	}
	return(j);
}

char setupCfisEntriesForSetFeatureAll(CmdListHeader *CmdListNum,
								   Uint8 featureReg,
								   Uint8 secCountReg) {
    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command Header *******
	// SET FEATURES Command:
	//  COMMAND      = 0xEF (SET FEATURE Command)
	//  FEATURES     = 0x03 (Set Transfer Mode: See Table 52 of ATA/ATAPI-6-Spec)
	//  SECTOR COUNT = 0x01 (Disable IORDY: See Section 8.49.11 Table 53 ATA/ATAPI-6 Spec)
	//  LBA LOW/MID/HIGH = 0x00/0x00/0x00 (Not Applicable for Disabling IORDY)
	//  DEVICE       = 0x00 (Only DEV bit field (bit 4) meaningful for Legacy ATA most
	//                        likely Not Applicable for AHCI)
	//
	// Configure Word 0 of Command List
	CmdListNum->DW0.CmdLen=5;   // This is the length of H2D FIS. This might need changing
							    //   based on the Command issued to Device. Need to Check.
    CmdListNum->DW0.Atapi=0;    // Command is destined to HDD Like Device.
	CmdListNum->DW0.Prefetch=1; // Doesn't hurt prefetching so do it.
								// WARNING: Do Not Prefetch if using:
								//          => Command Queuing
								//          => Port Multiplier
	CmdListNum->DW0.Reset=0;    // This is normally set to Zero unless a Soft Reset is required.
	CmdListNum->DW0.Bist=0;     // This is for entering test mode and should be cleared for normal operation.
	CmdListNum->DW0.Rok=0;      // For Normal operation require to Clear this bit so P0TFD and P0CI are modified by HBA as appropriate.
								//  Rok should be set for S/W Reset Command.
	CmdListNum->DW0.Pmp=0x0;    // Used only if an external Port Multiplier is attached and selects the Port of the Port Multiplier.
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	CmdListNum->DW0.Write=1;    // This should be a Don't Care for SET Feature. NO Data is transferred. Write=1/0=>Write/Read;
								//  ???? Don't know what to do here for sure.

	CmdListNum->DW0.Prdtl=0;    // No Scatter/Gather Descriptor Table entries

    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command FIS (H2D FIS) ******

	// Cmd FIS is made of a 20 Bytes size FIS.
	// FIS Fieds are Initialized into the alloted data buffers.
	// Strucutre type 'cmdFis' holds 8 of the 20 bytes of the FIS. These
	//  seven elements of the cfis are good enough for majority of command
	//  building tasks.
	// If need to access other cfis members, need to access the cfis member
	//  directly. Example of how to access the FIS Type, cfis Byte0.
	// 	CmdTable[n].cfis.DW0.B0FisType=0x27;
	// FIS Type is hard coded within buildCmdFis function and Reserved Locations
	//  are also cleared to Zeros, so no need of iniitalizing this bytes is necessary here.

	myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CMD_UPDATE;	// Bit7 of Byte1 is set for Command Wr and Cleared for Control Wr
	//myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CTRL_UPDATE; // Bit7 of Byte1 is Cleared for Command Control Wr.

	myCmdFis.cfisCmd               = ATA_CMD_SET_FEATURES; // SET FEATURES Cmd Used to Disable IORDY
														   //  Invoking this Cmd to check SATA Core
														   //  Command Layer of handling Non-Data Cmd.

	myCmdFis.cfisFeature           = featureReg; // Sub Command of SET FEATURES
	myCmdFis.cfisDw1SecNumLbaLow   = 0x00;	// N/A Initialize to  to Zero
	myCmdFis.cfisDw1CylLowLbaMid   = 0x00;	// N/A
	myCmdFis.cfisDw1CylHighLbahigh = 0x00;	// N/A
	myCmdFis.cfisDw1Dev            = 0x00;	// N/A
	myCmdFis.cfisDw3SecCnt         = secCountReg; // Transfer Mode Value for Disabling IORDY
	myCmdFis.cfisDw3Ctrl           = 0x00;

	// The below require to be initialized at least once and can be ignored especially if
	//  not using 48-Bit Addressing. If use 48-Bit Addressing, then require constant
	//  maintenance prior to invoking a command.
	myCmdFis.cfisDw2SecNumLbaLowExp=0x00;
	myCmdFis.cfisDw2CylLowLbaMidExp=0x00;
	myCmdFis.cfisDw2CylHighLbahighExp=0x00;
	myCmdFis.cfisDw2FeatureExp=0x00;
	myCmdFis.cfisDw3SecCntExp=0x00;

	// Invalidate for future use.
	Dev28bitLbaAddress = 0xFFFFFFFF;

	return(0);
}



char setupCfisEntriesForSetFeature(CmdListHeader *CmdListNum) {
    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command Header *******
	// SET FEATURES Command:
	//  COMMAND      = 0xEF (SET FEATURE Command)
	//  FEATURES     = 0x03 (Set Transfer Mode: See Table 52 of ATA/ATAPI-6-Spec)
	//  SECTOR COUNT = 0x01 (Disable IORDY: See Section 8.49.11 Table 53 ATA/ATAPI-6 Spec)
	//  LBA LOW/MID/HIGH = 0x00/0x00/0x00 (Not Applicable for Disabling IORDY)
	//  DEVICE       = 0x00 (Only DEV bit field (bit 4) meaningful for Legacy ATA most
	//                        likely Not Applicable for AHCI)
	//
	// Configure Word 0 of Command List
	CmdListNum->DW0.CmdLen=5;   // This is the length of H2D FIS. This might need changing
							    //   based on the Command issued to Device. Need to Check.
    CmdListNum->DW0.Atapi=0;    // Command is destined to HDD Like Device.
	CmdListNum->DW0.Prefetch=1; // Doesn't hurt prefetching so do it.
								// WARNING: Do Not Prefetch if using:
								//          => Command Queuing
								//          => Port Multiplier
	CmdListNum->DW0.Reset=0;    // This is normally set to Zero unless a Soft Reset is required.
	CmdListNum->DW0.Bist=0;     // This is for entering test mode and should be cleared for normal operation.
	CmdListNum->DW0.Rok=0;      // For Normal operation require to Clear this bit so P0TFD and P0CI are modified by HBA as appropriate.
								//  Rok should be set for S/W Reset Command.
	CmdListNum->DW0.Pmp=0x0;    // Used only if an external Port Multiplier is attached and selects the Port of the Port Multiplier.
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
	CmdListNum->DW0.Write=1;    // This should be a Don't Care for SET Feature. NO Data is transferred. Write=1/0=>Write/Read;
								//  ???? Don't know what to do here for sure.

	CmdListNum->DW0.Prdtl=0;    // No Scatter/Gather Descriptor Table entries

    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command FIS (H2D FIS) ******

	// Cmd FIS is made of a 20 Bytes size FIS.
	// FIS Fieds are Initialized into the alloted data buffers.
	// Strucutre type 'cmdFis' holds 8 of the 20 bytes of the FIS. These
	//  seven elements of the cfis are good enough for majority of command
	//  building tasks.
	// If need to access other cfis members, need to access the cfis member
	//  directly. Example of how to access the FIS Type, cfis Byte0.
	// 	CmdTable[n].cfis.DW0.B0FisType=0x27;
	// FIS Type is hard coded within buildCmdFis function and Reserved Locations
	//  are also cleared to Zeros, so no need of iniitalizing this bytes is necessary here.

	myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CMD_UPDATE;	// Bit7 of Byte1 is set for Command Wr and Cleared for Control Wr
	//myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CTRL_UPDATE; // Bit7 of Byte1 is Cleared for Command Control Wr.

	myCmdFis.cfisCmd               = ATA_CMD_SET_FEATURES; // SET FEATURES Cmd Used to Disable IORDY
														   //  Invoking this Cmd to check SATA Core
														   //  Command Layer of handling Non-Data Cmd.

	myCmdFis.cfisFeature           = ATA_SUB_CMD_XFER_MODE; // Sub Command of SET FEATURES
	myCmdFis.cfisDw1SecNumLbaLow   = 0x00;	// N/A Initialize to  to Zero
	myCmdFis.cfisDw1CylLowLbaMid   = 0x00;	// N/A
	myCmdFis.cfisDw1CylHighLbahigh = 0x00;	// N/A
	myCmdFis.cfisDw1Dev            = 0x00;	// N/A
	myCmdFis.cfisDw3SecCnt         = ATA_SUB_CMD_XFER_MODE_VALUE; // Transfer Mode Value for Disabling IORDY
	myCmdFis.cfisDw3Ctrl           = 0x00;

	// The below require to be initialized at least once and can be ignored especially if
	//  not using 48-Bit Addressing. If use 48-Bit Addressing, then require constant
	//  maintenance prior to invoking a command.
	myCmdFis.cfisDw2SecNumLbaLowExp=0x00;
	myCmdFis.cfisDw2CylLowLbaMidExp=0x00;
	myCmdFis.cfisDw2CylHighLbahighExp=0x00;
	myCmdFis.cfisDw2FeatureExp=0x00;
	myCmdFis.cfisDw3SecCntExp=0x00;

	// Invalidate for future use.
	Dev28bitLbaAddress = 0xFFFFFFFF;

	return(0);
}




void invokeIdentifyDevCmd(Uint16 cmdSlot) {
/////////////////////////////////////////////////////////////////////////////////////////////////
//// IDENTIFY DEVICE Command (PIO Read Command)
/////////////////////////////////////////////////////////////////////////////////////////////////
/*
  IDENTIFY DEVICE Command is executed in a same way that a PIO Read Command "READ SECTOR" is
   executed. The Device upon decoding an IDENTIFY Device Command, it sends the HBA its
   Device Parameters, 512 bytes of it.

  HBA ==> Device    H2D FIS (Command FIS that has the "IDENTIFY DEVICE" Command within H2D.Command field).
  Device ==> HBA    PIO Setup FIS (Device informs the HBA that Data Transfer is Ready). Int is Generated Here.
  Device ==> HBA    Data FIS (Device Data Requested and pointed by the DMA/PRD Contents within Command Table).

  Note: When performing a PIO Read, Interrupt is generated when Data is Ready to Transfer not when
         data transfer has completed. For a PIO Write, however, unlike PIO Read, Interrupt is
         generated at the end of the data transfer.

  If need to capture Interrupt (just have only the Flag set) at:
    Global Level (IS Register), need to enable P0IE.Interrupt field.
    Port Level (No Enable bit needs to be set. P0IS will be set if D2H FIS DW0.Byte1.bit6 is set).

  If need to generate Interrupt at:
    Global Level (enable interrupt by setting GHC.IE bit field and P0IE.xxx should be set to enable particular interrupt). Port Level interrupt handling is a subset of Global Level.
    Port Level (So long as Global Level interrupt is not enabled, GHC.IE is set, Interrupt will not be sent to CPU just from Port Level only).
*/

	if (cmdSlot >= LISTLENGTH)
		for(;;); // Have resource issue to fix.

// Read from Disk
	// Configure Device 28 bit LBA Address (Start Address for Rd/Wr Transfer);
	Dev28bitLbaAddress               = 0; // 28-Bit LBA Address

	// Need to set PioCmd to the below when executing IDENTIFY DEVICE Cmd.
	// For all other PIO Read Commands, Set it to any value but not IDENTIFY DEVICE Cmd.
	PioCmd = ATA_CMD_IDENTIFY_DEVICE;

	// If problem exist when setuping CmdList, stay here. If not continue
	//   Make Sure PioCmd is initialized with IDENTIFY DEVICE Cmd value for
	if(setupCfisEntriesForDataRdWr(&CmdLists[cmdSlot], DATA_DIR_RD, PIO_PROTOCOL)) // Usage: setupCfisEntriesForDataRdWr(CmdListHeader *, DataDir, xferProtocol)
		for(;;);                                                                       // DataDir = DATA_DIR_RD or DATA_DIR_WR, xferProtocol = PIO/DMA_PROTOCOL


	// Write cfis Data
	buildCmdFis(&CmdTable[cmdSlot]);


	if (startCmdListProcessing()); // Stay here if unable to start processing command list.


	enableDisableInt(PORTint, ENABLE, 0xFFC000FF); // enableDisableInt(int type, intState, specificField)
														// int type=GLOBALint or PORTint
														// intState=DISABLE or ENABLE

	if(submitCmd (NON_QUEUED_CMD, cmdSlot))
		for(;;); // Usage: CommandType (QUEUED(NON_QUEUED)_CMD, Command Slot);
	                                   //  Stay here if Cmd List and Rx FIS DMAs are not enabled and running.

	// Wait until a PIO Setup FIS is received from device prior to continuing.
	while(sataRegs->P0IS & AHCI_P0IS_PSS == 0x00000000);

	// Check for the Data Transfer Direction (BYTE1.D = 1/0 => Read/Write)
	while (((RcvFis.PSFIS.DW0.BYTE1 & 0x20) != 0x20)); // Wait here until RFIS is data reaches system memory.
													   //  Identify Device is always a Read PIO Cmd and field 'D' should be set.

	// At this point Command has not finished processing yet P0CI should be set.
	// P0CI Should be cleared prior to Interrupt Being Set
	while(getRegStatus((Uint32*)&sataRegs->P0CI, cmdSlot) == (0x1<<cmdSlot));	// Wait Until P0CI[ChHeader] to be cleared by HBA

	// Note: If P0IE is not enable, IS Register will not be set. Standard rule is to check IS register
	//        to figure out the Port that generates interrupt, which is where IS comes in. If P0IE is not
	//        set, interrupt will not be captured at the Global Status level.
	while(sataRegs->IS == 0);	                         // Stay here until an interrupt is received.

	// Clears both P0IS and IS register. Only Clears RWC bit fields. So, it is OK for this task.
	clearIntOrErrorDiag(INTFIELDS, sataRegs->P0IS);      // Clear P0IS Register


	// Need to set PioCmd to the below when executing IDENTIFY DEVICE Cmd.
	// For all other PIO Read Commands, Set it to any value but not IDENTIFY DEVICE Cmd.
	// Invalidate for other PIO Commands.
	PioCmd = 0;	// Invalidate for non-SET FEATURE Commands.

}


void invokeSetFeatureCmd(Uint16 cmdSlot) {
/////////////////////////////////////////////////////////////////////////////////////////////////
//// Non-Data Command - SET FEATURES to Disabling IORDY
/////////////////////////////////////////////////////////////////////////////////////////////////
// SET FEATURE Command
//  See ATA/ATAPI-6 Spec Section 8.49.
//  See SATA Revision 2.6 Section 13.2.1
/*
  When performing the Non-Data-Transfer "SET FEATURE" Command the following captures the FISes
   that are communicated between Host (HBA) and Device (SpeedBridge or SATA Drive).

  HBA ==> Device    H2D FIS (Command FIS that has the "SET FEATURE" Cmd, "Transfer Value" for
                               Feature Reg, and "Transfer Mode Value" in Sector Reg.).
  Device ==> HBA    D2H FIS (Completion Status from Device)

  If need to capture Interrupt (just have only the Flag set) at:
    Global Level (IS Register), need to enable P0IE.Interrupt field.
    Port Level (No Enable bit needs to be set. P0IS will be set if D2H FIS DW0.Byte1.bit6 is set).

  If need to generate Interrupt at:
    Global Level (enable interrupt by setting GHC.IE bit field and P0IE.xxx should be set to enable particular interrupt). Port Level interrupt handling is a subset of Global Level.
    Port Level (So long as Global Level interrupt is not enabled, GHC.IE is set, Interrupt will not be sent to CPU just from Port Level only).
*/
	if (cmdSlot >= LISTLENGTH)
		for(;;); // Have resource issue to fix.

	// If problem exist when setuping CmdList, stay here. If not continue
//	if(setupCfisEntriesForSetFeature(&CmdLists[cmdSlot]))	// Usage: setupCfisEntriesForSetFeature(CmdListHeader *)
//		for(;;);

	// Write cfis Data
	buildCmdFis(&CmdTable[cmdSlot]);


	if (startCmdListProcessing()); // Stay here if unable to start processing command list.

	enableDisableInt(PORTint, ENABLE, 0xFFC000FF); // enableDisableInt(int type, intState, specificField)
														// int type=GLOBALint or PORTint
														// intState=DISABLE or ENABLE

	// Submit Command and if have a problem with it stay here.
	if(submitCmd (NON_QUEUED_CMD, cmdSlot))
		for(;;); // Usage: CommandType (QUEUED(NON_QUEUED)_CMD, Command Slot);
	                                   //  Stay here if Cmd List and Rx FIS DMAs are not enabled and running.

	// Wait until a D2H Register FIS is received from device prior to continuing.
	while(sataRegs->P0IS & AHCI_P0IS_DHRS == 0x00000000);

	// Wait until HBA Clears the Command
	while(getRegStatus((Uint32*)&sataRegs->P0CI, cmdSlot) == (0x1<<cmdSlot));	// Wait Until P0CI[ChHeader] to be cleared by HBA

	while(sataRegs->IS == 0);	                         // Stay here until an interrupt is received.

	// Clears both P0IS and IS register. Only Clears RWC bit fields. So, it is OK for this task.
	clearIntOrErrorDiag(INTFIELDS, sataRegs->P0IS);      // Clear P0IS Register
}

void initBaseAddresses(void) {
	sataRegs->P0CLB=(unsigned int)CmdLists;
	// Associate Receive FIS Data Structure Base Address to HBA Space.
	sataRegs->P0FB=(unsigned int)&RcvFis;
}

void performFirmwareInit(void) {
//	sataRegs->CAP &= 0xE7000000; // Not sure if you are allowed to make multiple
                                 //  Writes so watch out for multiple writes.
	sataRegs->CAP |= ((swCtrlFeatures.capSMPS << 28) |
	                  (swCtrlFeatures.capSSS  << 27)
	                 );

	// Configure PI[31:0]
	sataRegs->PI  |=  (swCtrlFeatures.piPi  << 0);

	// Configure P0CMD[ESP,CPD,MPSP,HPCP=21,20,19,18]
//	sataRegs->P0CMD &= 0xFFC3FFFF;

	sataRegs->P0CMD |= ((swCtrlFeatures.p0cmdEsp << 21) |
                        (swCtrlFeatures.p0cmdCpd << 20) |
                        (swCtrlFeatures.p0cmdMpsp << 19)|
                        (swCtrlFeatures.p0cmdHpcp << 18)
                       );

	initBaseAddresses(); // Initialize Command List (P0CLB) and Receive FIS (P0FS)

	initPhyAndDmaPars(); // Initialize PHY and DMA Parameters

	enableRcvFis();	// Enable Receive DMA




// The below are general Semaphores/Flags used and want to make sure they are initialized.
	// 28 Bit LBA Address of Device. 0xFFFFFFFF is used by test S/W to indicate that it is not initialized
	Dev28bitLbaAddress = 0xFFFFFFFF; // S/W needs to initialize this variable prior to calling

	// PioCmd is a Semaphore that is set to IDENTIFY DEVICE Cmd when invoking IDENTIFY DEVICE Cmd.
	//  Other times it should be set to another value. Since IDENTIFY DEVICE is a PIO Read Cmd
	//  this semaphore is used to either invoke IDENTIFY DEVICE Cmd or READ SECTORS Cmd.
	//  It should be set prior to calling "setupCfisEntriesForDataRdWr()".
	PioCmd = 0;

	//_INT_DRIVEN_TEST_ is defined within the Project File
	#ifdef _INT_DRIVEN_TEST_
		intHandlingMethod = USE_INT_HANDLER;
	#else
		intHandlingMethod = USE_POLLING;
	#endif

	intIsrCnt=0;
	intIsrFlag=0;
	pwrMgmtCap=notDetermined; // Call funtion detPwrMgmtSupport() to identify Supported Power Mgmt
	hostPwrMgmtCfg=notAccessed; // Call function detHostPwrMgmtStateCfg() to identify pwr state it will enter when invoked

}


void clearSystemMemory(void) {
	// Clear Command Lists, Command Tables, and DMA Buffer Location.
	clearCmdTableAndDmaBuff();
	// Clear Receive FISs
	clearRcvFis();
}

void clearCmdList(void) {
	//clear Host to Device (Command FIS) Space
	initMemory((Uint32*)CmdLists, (LISTLENGTH*(sizeof(CmdListHeader)/4)), 0, 0);
}

void clearIndividualCmdList(CmdListHeader *CmdListNum) {
	//clear Host to Device (Command FIS) Space
	initMemory((Uint32*)CmdListNum, (sizeof(CmdListHeader)/4), 0, 0);
}

void clearCmdTables(void) {
	Uint16 cmdSlot;
	for (cmdSlot=0; cmdSlot<LISTLENGTH; cmdSlot++) {
		//Clear Command FIS and ATAPI Command Spaces for Command Header 0
		initMemory((Uint32 *)&CmdTable[cmdSlot], (sizeof(CommandFIS)/4)+(sizeof(Atapi)/4), 0, 0);
		//Clear PRD Descriptor Locations for Command Header 0
		initMemory((Uint32*)((Uint32)&CmdTable[cmdSlot]+0x80), ((sizeof(PRDT)/4)*PRDLENGTH), 0, 0);
		//Clear Command FIS and ATAPI Command Spaces for Command Header 0
	}
}

void clearIndividualCmdTable(Uint16 cmdSlot) {
		//Clear Command FIS and ATAPI Command Spaces for Command Header 0
		initMemory((Uint32 *)&CmdTable[cmdSlot], (sizeof(CommandFIS)/4)+(sizeof(Atapi)/4), 0, 0);
		//Clear PRD Descriptor Locations for Command Header 0
		initMemory((Uint32*)((Uint32)&CmdTable[cmdSlot]+0x80), ((sizeof(PRDT)/4)*PRDLENGTH), 0, 0);
		//Clear Command FIS and ATAPI Command Spaces for Command Header 0
}


void clearDmaBuffers(void) {
	//Clear PRD Data Buffer Memory
	initMemory((Uint32 *)prdTableDataBuff, (LISTLENGTH*PRDLENGTH*DATABUFFERLEN/4), 0, 0);
}

void associtateCmdSlotWithCmdTable(Uint16 cmdSlot) {
	CmdLists[cmdSlot].DW2.CmdTableAddLow=((unsigned int)&CmdTable[cmdSlot] & 0xFFFFFF80);
	CmdLists[cmdSlot].DW3.CmdTableAddHigh=0x0;
}



void associtateCmdTableWithPrds(Uint16 cmdSlot) {
	Uint16 prdLength;
	for (prdLength=0; prdLength<PRDLENGTH; prdLength++) {
		// Command Header 0 PRD Descriptors 0 & 1 are Initialized.
		CmdTable[cmdSlot].prdTable[prdLength].DW0.DbaLow=(unsigned int)&prdTableDataBuff[cmdSlot][prdLength];
		CmdTable[cmdSlot].prdTable[prdLength].DW1.DbaHigh=0x0;
		CmdTable[cmdSlot].prdTable[prdLength].DW3.DataBC=DATABUFFERLEN-1;
	}
}


void initPhyAndDmaPars(void) {

	#ifdef _SILICON_
		sataRegs->P0PHYCR =  MPY		<< 0	|
							 LB			<< 4	|
							 LOS		<< 6	|
							 RXINVPAIR 	<< 7	|
							 RXTERM		<< 8	|
							 RXCDR		<< 10	|
							 RXEQ		<< 13	|
							 TXINVPAIR 	<< 17	|
							 TXCM		<< 18	|
							 TXSWING 	<< 19	|
							 TXDE		<< 22	|
							 OVERRIDE 	<< 30	|
							 ENPLL		<< 31;
	#else

		// Configure PHY Settings
		// Probably want to configure PHY and DMA prior to starting Device (SUD) and enabling
		//  device detection and speed negotiation. Once the speed negotiation finishes
		//  the device is going to send its D2H FIS with its taskfile register content
		//  which needs to be moved onto the Recive FIS space via the Receive DMA.
		sataRegs->P0PHYCR = 0x80000049;
	#endif

	// #define values are in sata_utilities.h
	cfgDmaSetting();

}


void initIntAndClearFlags(void) {
	// Make sure Interrupt is disabled (Disable at Port Level followed by Global Level).
	// Clear Interrupt at Port Level
	enableDisableInt(PORTint, DISABLE, 0xFFFFFFFF); // clearInt(int type, intState, specificField)
													// int type=GLOBALint or PORTint
													// intState=DISABLE or ENABLE
													// specificField=bit field to Enable or Disable
													//  is used for the RWC feature for PORTint
	// Disable interrrupt at Global Level
	enableDisableInt(GLOBALint, DISABLE, 0);	// clearInt(intType, intState fields2clr)
											// int type=GLOBALint or PORTint
											// intState=DISABLE or ENABLE
											// fields2clr=dontcare for GLOBALint
											//  is used for the RWC feature for PORTint

	// Need to clear interrupts at Port Level followed by Global Level.
	// Ensure all pending Port Error and Status are cleared.
	clearIntOrErrorDiag(ERRORFIELDS, sataRegs->P0SERR);	// Clear P0SERR Register
	// Ensure all pending Port Interrupts and The Single Global Interrupt are cleared.
	clearIntOrErrorDiag(INTFIELDS, sataRegs->P0IS); // Clear P0IS and IS Regs
}



void associateSysMem2Hba(Uint16 cmdSlot) {
	associtateCmdSlotWithCmdTable(cmdSlot);	// Assign Sys Mem allocated for Cmd Table to Cmd List Slot
	associtateCmdTableWithPrds(cmdSlot); 	// Assign PRD info to Cmd Table
}

#if 0
void sata_intc_setup(void) {
	#if defined(__TMS470__)
		dvSetIsr(&sataIsr, CSL_INTC_SATAINT);	// SATA INTC Interrupt Event # 67: See arm_int_index.h or Freon_ch_14_interrupt_????.pdf

    #elif defined(_TMS320C6X)
		CSL_IntcParam vectId;
		CSL_Status intStat;
		CSL_IntcContext intcContext;
		CSL_IntcEventHandlerRecord EventHandler[30];
		CSL_IntcGlobalEnableState state;
		CSL_IntcHandle hIntc;
		CSL_IntcObj myIntcObj;
		CSL_IntcEventHandlerRecord myIntcEventHandlerRecord;
		CSL_IntcEventId SATA_GEM_EVENT_ID = (CSL_IntcEventId)CSL_INTC_SATAINT; 	// GEM INTC Interrupt Event # 24: See gem_int_index.h or Freon_ch_14_interrupt_????.pdf
		CSL_IntcEventHandler SATA_ISR_ADDR = (CSL_IntcEventHandler)sataIsr;

		CSL_intcGlobalNmiEnable();

		// Enable Global Interrupts
		intStat = CSL_intcGlobalEnable(&state);

		vectId = CSL_INTC_VECTID_4; // CPU interrupt number
		// Intc Module Initialization
		intcContext.eventhandlerRecord = EventHandler;
		intcContext.numEvtEntries = 10;  // used to allocate isr table entries
		CSL_intcInit(&intcContext);

		CSL_intcInterruptEnable(vectId);

		// Open a handle for the Event interrupt
		hIntc = CSL_intcOpen(&myIntcObj, SATA_GEM_EVENT_ID, &vectId , &intStat);
		myIntcEventHandlerRecord.handler = (CSL_IntcEventHandler)SATA_ISR_ADDR;
		myIntcEventHandlerRecord.arg = NULL;
		CSL_intcPlugEventHandler(hIntc, &myIntcEventHandlerRecord);
	#endif
}
#endif

// Used by both ARM and GEM Processors

void sataIsr(void) {
	intIsrCnt++;	// Count interrupt.
	intIsrFlag=1;

	//clearIntOrErrorDiag(ERRORFIELDS, sataRegs->P0SERR);	// Clear P0SERR Register
	// Ensure all pending Port Interrupts and The Single Global Interrupt are cleared.
	clearIntOrErrorDiag(INTFIELDS, sataRegs->P0IS); // Clear P0IS and IS Regs

}

char invokeSoftReset(void) {
/*
When Performing a Software Reset, HBA and Physical Communication is not Disturbed.

-  Clear P0CMD.ST to Zero.
-  Wait for P0CMD.CR goes to Zero.
-  Set P0CMD.ST so that the Command List DMA is running.
-  Prior to submitting the S/W Reset Command (writing to P0CI) make sure that:
     P0TFD.BSY and P0TFD.DRQ is cleared by the HBA.
    If P0TFD.BSY and/or P0TFD.DRQ will not clear need to use Command List
     Override.
-  Build two H2D FISes as noted below.

The First Command (Host to Device Register) FIS
=================================================
1a. The SRST bit needs to be set.
1b. The 'C' bit in DW0.BYTE1.bit7 of H2D is cleared.
1c. The CH[R] is set.
1d. The CH[C] is set.
                            C         R
|                   |                   |                   |                   |
| 15   14   13   12 | 11   10   09   08 | 07   06   05   04 | 03   02   01   00 |
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
|    |    |    |    |    |  1 |    |  1 |    |    |    |    |    |    |    |    |
+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+

The SECOND Command (Host to Device Register) FIS
=================================================
2a. The SRST bit needs to be cleared here (opposite to the first).
2b. The 'C' bit in DW0.BYTE1.bit7 of H2D is also Cleared here (same as first).
2c. The CH[R] is cleared here (opposite to first).
2d. The CH[C] is cleared here (opposite to first).

*/
	// Make sure P0CMD.ST bit is cleared.
	if ((sataRegs->P0CMD && AHCI_PxCMD_ST) != 0)
		sataRegs->P0CMD &= ~AHCI_PxCMD_ST;

	// Stay here until the Command DMA is halted.
	while((tempDebug2=sataRegs->P0CMD & AHCI_PxCMD_CR) != 0);

	//Ensure that BSY and DRQ are cleared prior to invoking S/W Reset.
	while(((sataRegs->P0TFD & (AHCI_PxTFD_STS_BSY | AHCI_PxTFD_STS_DRQ)) != 0))
		sataRegs->P0CMD |= AHCI_PxCMD_CLO;

	clearCmdList();		// Clear Cmd List allocated within Sys Mem
	clearCmdTables();	// Clear Cmd Tables allocated within Sys Mem
	clearRcvFis();		// Clear Receive FIS allocated within Sys Mem

// Build the first H2D FIS
	cmdSlot2Use=0;
	associateSysMem2Hba(cmdSlot2Use);	// Associate Sys Memory to CmdList and PRDs to Cmd Table

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//First Part of the SoftReset
//The first Register FIS has the SRST bit set to ‘1’ in the Control field of the Register
//FIS, the ‘C’ bit is set to ‘0’ in the Register FIS, and the command table has the
//CH[R] (reset) and CH[C] (clear BSY on R_OK) bits set to ‘1’. The CH[R] (reset) bit
//causes the HBA to perform a SYNC escape if necessary to put the device into an idle
//condition before sending the software reset. The CH[C] (clear BSY on R_OK) bit needs to
//be set for the first Register FIS to clear the BSY bit and proceed to issue the next
//Register FIS since the device does not send a response to the first Register FIS in
//a software reset sequence.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// If problem exist when setuping CmdList, stay here. If not continue
	if(setupCfisEntriesForSwReset(&CmdLists[cmdSlot2Use], 1, 1, 1)) // CH[R] (Reset), CH[C] (ROK), SRST
		for(;;); // Usage: setupCfisEntriesForDataRdWr(CmdListHeader *, CH[R] state, CH[C] state, DEV_CTL_REG.SRST State)

	// Write cfis Data
	buildCmdFis(&CmdTable[cmdSlot2Use]);

// Build the second H2D FIS
	cmdSlot2Use=1;
	associateSysMem2Hba(cmdSlot2Use);	// Associate Sys Memory to CmdList and PRDs to Cmd Table

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//Second Part of the SoftReset
//The second Register FIS has the SRST bit set to ‘0’ in the Control field of the Register FIS,
//the ‘C’ bit is set to ‘0’ in the Register FIS, and the command table has the CH[R] (reset)
//and CH[C] (clear BSY on R_OK) bits cleared to ‘0’.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// If problem exist when setuping CmdList, stay here. If not continue
	if(setupCfisEntriesForSwReset(&CmdLists[cmdSlot2Use], 0, 0, 0)) // CH[R] (Reset), CH[C] (ROK), SRST
		for(;;); // Usage: setupCfisEntriesForDataRdWr(CmdListHeader *, CH[R] state, CH[C] state, DEV_CTL_REG.SRST State)

	// Write cfis Data
	buildCmdFis(&CmdTable[cmdSlot2Use]);

// Enable Command List DMA
	// Make sure that a device is present and HBA has established communications.
	while ((sataRegs->P0SSTS & AHCI_PxSCTL_PxSSTS_DET) !=0x3);

	// Clear P0SERR.DIAG.X (RWC bit field) so that the P0TFD is updated by HBA.
	// Make sure it is cleared.
	sataRegs->P0SERR |= 0x04000000;

	// Make sure the Command List is not Running.

	if (sataRegs->P0CMD & AHCI_PxCMD_CR)
		return(1);
	//  Task file regs and look for Device ready status.
	while ((sataRegs->P0TFD & AHCI_PxTFD_STS_BSY_DRQ_ERR) != 0);

	// Make sure the the Receive FIS DMA is running.
	if ((sataRegs->P0CMD & (AHCI_PxCMD_FR | AHCI_PxCMD_FRE)) !=
	                         (AHCI_PxCMD_FR | AHCI_PxCMD_FRE))
		return(1);

	// Reading P0TFD to view the Status of the BYS and DRQ bit field at this stage.
//    7     6   5    4     3   2     1    0
//	+----+----+----+----+----+----+----+----+
//  |BSY | cs | cs | cs | DRQ| cs | cs |ERR |
//	+----+----+----+----+----+----+----+----+
//  | 0    1    0    1  |  0    0    0    0 |

	tempDebug0=sataRegs->P0TFD;

	tempDebug1 = sataRegs->P0CMD | AHCI_PxCMD_CLO;

	// Set Command List Override
	sataRegs->P0CMD = sataRegs->P0CMD | AHCI_PxCMD_CLO;

	tempDebug2 = sataRegs->P0CMD;

	// Wait here until CLO bit is cleared.
	while((sataRegs->P0CMD & AHCI_PxCMD_CLO) != 0);

	tempDebug3 = sataRegs->P0CMD;

	// Enable the Cmd List DMA Engine.
	sataRegs->P0CMD |= AHCI_PxCMD_ST;

	// Wait here a bit until the Command List DMA Engine has started to run
	while ((sataRegs->P0CMD & AHCI_PxCMD_CR) == 0)
		waitForXms(10);
// Enable Command List DMA End



	enableDisableInt(PORTint, ENABLE, 0xFFC000FF); // enableDisableInt(int type, intState, specificField)
														// int type=GLOBALint or PORTint
														// intState=DISABLE or ENABLE
// Submit both commands
	if(submitCmd (NON_QUEUED_CMD, 0))
		for(;;); // Usage: CommandType (QUEUED(NON_QUEUED)_CMD, Command Slot);
	                                   //  Stay here if Cmd List and Rx FIS DMAs are not enabled and running.
	if(submitCmd (NON_QUEUED_CMD, 1))
		for(;;); // Usage: CommandType (QUEUED(NON_QUEUED)_CMD, Command Slot);
	                                   //  Stay here if Cmd List and Rx FIS DMAs are not enabled and running.

	// No Interrupt will be Generated. See D2H FIS Received DW0.BYTE1.bit6(I). I will not be set.
//	while(sataRegs->IS == 0);	                         // Stay here until an interrupt is received.


	// If P0IS.DPS is set, A PRD with the ‘I’ bit set has transferred all of its data.
	// This bitfield might not bit set for Non-Queued DMA. Applicable for Queued DMA
	//while(getRegStatus((Uint32*)&sataRegs->P0IS, AHCI_P0IS_DPS) == 0); // getRegStatus(ptr2int,field Mask)

	while(getRegStatus((Uint32*)&sataRegs->P0CI, 3) != 0);	// Wait Until P0CI[ChHeader] to be cleared by HBA


	return(0);
}


char setupCfisEntriesForSwReset(CmdListHeader *CmdListNum, Uint8 chReset, Uint8 chRok, Uint8 h2dSrst) {
    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command Header *******
	// Other part of the Command List Structure, with the exception of Word 0 for the
	//  Command Slots[0] and [1] are already initialized when invoking sata_init_and_spin_up()
	//  function via associateMem2HBA() function.
	// Configure Word 0 of Command List
	CmdListNum->DW0.CmdLen=5;   // This is the length of H2D FIS. This might need changing
							    //   based on the Command issued to Device. Need to Check.
    CmdListNum->DW0.Atapi=0;    // Command is destined to HDD Like Device.
	CmdListNum->DW0.Prefetch=0; // Doesn't hurt prefetching so do it.
								// WARNING: Do Not Prefetch if using:
								//          => Command Queuing
								//          => Port Multiplier
	CmdListNum->DW0.Reset=chReset;    // This is set for the first H2D and cleared for the second H2D
	CmdListNum->DW0.Bist=0;     // This is for entering test mode and should be cleared for normal operation.
	CmdListNum->DW0.Rok=chRok;      // This is set for the first H2D and cleared for the second H2D
								//  Rok should be set for S/W Reset Command.
	CmdListNum->DW0.Pmp=0x0;    // Used only if an external Port Multiplier is attached and selects the Port of the Port Multiplier.

	CmdListNum->DW0.Write=0;    //  This is a Don't Care field here since there is no Data Transfer taking place so any value will work. Write=1/0=>Write/Read;

	CmdListNum->DW0.Prdtl=0;    // Need to update this when using DMA for Data transfer.

    // *************************************************************************************
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ **** Initializing the Command FIS (H2D FIS) ******

	// FIS Type is hard coded within buildCmdFis function and Reserved Locations
	//  are also cleared to Zeros, so no need of iniitalizing this bytes is necessary here.


	//myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CMD_UPDATE;	// Bit7 of Byte1 is set for Command Wr and Cleared for Control Wr
	myCmdFis.cfisByte1             = CMDFIS_BYTE1_C_IS_CTRL_UPDATE; // Bit7 of Byte1 is Cleared for Command Control Wr.

	myCmdFis.cfisCmd               = 0x00;
	myCmdFis.cfisFeature           = 0x00;
	myCmdFis.cfisDw1SecNumLbaLow   = 0x00;
	myCmdFis.cfisDw1CylLowLbaMid   = 0x00;
	myCmdFis.cfisDw1CylHighLbahigh = 0x00;
	myCmdFis.cfisDw1Dev            = 0x00;
	myCmdFis.cfisDw3SecCnt         = 0x00;
	if (h2dSrst)
		myCmdFis.cfisDw3Ctrl       = 0x04;	// SET Device Control Register SRST bit field
	else
		myCmdFis.cfisDw3Ctrl       = 0x00;	// CLEAR Device Control Register SRST bit field

	// The below require to be initialized at least once and can be ignored especially if
	//  not using 48-Bit Addressing. If use 48-Bit Addressing, then require constant
	//  maintenance prior to invoking a command.
	myCmdFis.cfisDw2SecNumLbaLowExp=0x00;
	myCmdFis.cfisDw2CylLowLbaMidExp=0x00;
	myCmdFis.cfisDw2CylHighLbahighExp=0x00;
	myCmdFis.cfisDw2FeatureExp=0x00;
	myCmdFis.cfisDw3SecCntExp=0x00;

	// Invalidate for future use.
	Dev28bitLbaAddress = 0xFFFFFFFF;

	return(0);
}

void detPwrMgmtSupport(void) {
	Uint8 pwrType;

	pwrType=((sataRegs->CAP & 0x00006000) >> 13);
	switch (pwrType) {
		case 0:
			pwrMgmtCap = supportNone;
			break;

		case 1:
			pwrMgmtCap = supportPmOnly;
			break;

		case 2:
			pwrMgmtCap = supportSmOnly;
			break;

		case 3:
			pwrMgmtCap = supportBothPm;
			break;

		default:
			break;
	}
}

void detHostPwrMgmtStateCfg(void) {
	if (sataRegs->CAP && 0x04000000) {	// if Host is capable of changing Pwr State
		if (sataRegs->P0CMD && AHCI_PxCMD_ASP)
			hostPwrMgmtCfg = hostCfgSlumberState;
		else
			hostPwrMgmtCfg = hostCfgPartialState;
	}
}

void cfgHBAlinkPwrTransState(hostPwrMgmtConfig hPmCfg) {
	if (hPmCfg == hostCfgSlumberState)
		sataRegs->P0CMD |= AHCI_PxCMD_ASP;
	else
		sataRegs->P0CMD &= ~AHCI_PxCMD_ASP;
}

Uint8 reqPwrTransState(hostPwrMgmtConfig hPmCfg) {

	if (!(sataRegs->P0CMD & (Uint32)(0xF << AHCI_PxCMD_ICC_SHIFT))) {
		switch (hPmCfg) {
			case hostCfgSlumberState:
				sataRegs->P0CMD |= (0x6 << AHCI_PxCMD_ICC_SHIFT);
				break;

			case hostCfgPartialState:
				sataRegs->P0CMD |= (0x2 << AHCI_PxCMD_ICC_SHIFT);
				break;

			default:
				break;
		}
	}
	else
		return(1);

	while((sataRegs->P0CMD & (Uint32)(0xF << AHCI_PxCMD_ICC_SHIFT)) != 0x00000000); // Wait here until Link goes to IDLE.
	return(0);
}

void enableDisableHBAlinkPwrMgmt(lowPwrState lps) {
	if (lps == ENABLE)
			sataRegs->P0CMD |= AHCI_PxCMD_ALPE;
	else
			sataRegs->P0CMD &= ~AHCI_PxCMD_ALPE;
}


hostPwrMgmtState detHBApwrTransSupport(void) {
	hostPwrMgmtState ptType;

	switch (((sataRegs->P0SCTL & 0x00000F00) >> 8)) {
		case 0:
			ptType = hostPTnoRestricition;
			break;

		case 1:
			ptType = hostPTtoPartialNotAllowed;
			break;

		case 2:
			ptType = hostPTtoSlumberNotAllowed;
			break;

		case 3:
			ptType = hostPTtoPartialSlumberNotAllowed;
			break;

		default:
			break;
	}
	return(ptType);
}

void cfgHbaPwrTrans(hostPwrMgmtState ptType) {
	switch (ptType) {
		case (hostPTnoRestricition):
			sataRegs->P0SCTL &= 0xFFFFFCFF;
			break;

		case (hostPTtoPartialNotAllowed):
			sataRegs->P0SCTL &= 0xFFFFFCFF;
			sataRegs->P0SCTL |= 0x00000100;
			break;

		case (hostPTtoSlumberNotAllowed):
			sataRegs->P0SCTL &= 0xFFFFFCFF;
			sataRegs->P0SCTL |= 0x00000200;
			break;

		case (hostPTtoPartialSlumberNotAllowed):
			sataRegs->P0SCTL |= 0x00000300;
			break;

		default:
			break;
	}
}


void parseDeviceCapabilityNdetermineLowPowerSupport(Uint16 *pIdDevPwrData) {
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// Parse receive data and Identify Low Power Transition Capability and Current Settings
	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	sataCapabilities=*(pIdDevPwrData+76);		// 0x0506
	sataFeaturesSupported=*(pIdDevPwrData+78);		// 0x0048
	sataFeaturesEnabled=*(pIdDevPwrData+79);		// 0x0040

	//Word 76 has Power and Speed Related Data. See Page 435 of 600 of Sata Rev 2.6 Spec.
		// If this field is 0000h or FFFFh the device does not claim compliance with the Serial ATA specification
		// |                   |                   |                   |                   |
		// | 15   14   13   12 | 11   10   09   08 | 07   06   05   04 | 03   02   01   00 |
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		// |    |    |    |    |    |    |PnSP| NCQ| RSV| RSV| RSV| RSV| RSV|GEN2|GEN1| 0  |
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		//   0    0    0     0    0    1    0    1    0    0    0   0     0   1     1   0
		// PnSP Supports Partial and Slumber Power State.

	/*
	Word 76:
	Bit 9 when set to one indicates that the Serial ATA device supports the Partial and Slumber
	interface power management states when initiated by the host.
	*/

	if ((sataCapabilities & 0x0200) != 0)
		support4PwrMgmtInitiatedByHost = _YES;
	else
		support4PwrMgmtInitiatedByHost = _NO;


	/*
	Word 76:
	Bit 8 when set to one indicates that the Serial ATA device supports Native Command Queuing
	*/
	if ((sataCapabilities & 0x0100) != 0)
		support4NativeCommandQueuing = _YES;
	else
		support4NativeCommandQueuing = _NO;


	//Word 77 Reserved

	//Word 78 has Serial ATA Features
		// If this field is 0000h or FFFFh the device does not claim compliance with the Serial ATA specification
		// |                   |                   |                   |                   |
		// | 15   14   13   12 | 11   10   09   08 | 07   06   05   04 | 03   02   01   00 |
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		// | RSV| RSV| RSV| RSV| RSV| RSV| RSV| RSV| RSV|Sing| RSV|InDt|Dpwr|DFIS|DFIS| RSV|
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		//   0    0    0     0    0    0    0    0    0    1    0   0     1    0    0   0
		//

	/*
	Word 78:
	Bit 3 indicates whether the device supports initiating power management requests to the host.
	When set to one the device supports initiating interface power management requests and when
	cleared to zero the device does not support initiating power management requests. A device may
	support reception of power management requests initiated by the host as described in the
	definition of bit 9 of Word 76 without supporting initiating such power management requests as
	indicated by this bit.
	*/

	if ((sataFeaturesSupported & 0x0008) != 0)
		support4PwrMgmtInitiatedByDevice = _YES;
	else
		support4PwrMgmtInitiatedByDevice = _NO;


	//Word 79 has Power and Speed Related Data. See Page 435 of 600 of Sata Rev 2.6 Spec.
		// If this field is 0000h or FFFFh the device does not claim compliance with the Serial ATA specification
		// |                   |                   |                   |                   |
		// | 15   14   13   12 | 11   10   09   08 | 07   06   05   04 | 03   02   01   00 |
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		// | RSV| RSV| RSV| RSV| RSV| RSV| RSV| RSV| RSV|Sing| RSV|InDt|Dpwr|DFIS|DFIS| RSV|
		// +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
		//   0    0    0     0    0    0    0    0    0    1    0   0     0    0    0   0
		//

	/*
	Word 79:
	Bit 3 indicates whether device support for initiating power management requests to the host is
	enabled. When set to one the device may initiate power management transition requests. When
	cleared to zero the device shall not initiate interface power management requests to the host.
	This field shall be zero by default.
	*/

	if ((sataFeaturesEnabled & 0x0008) != 0)
		support4PwrMgmtInitiatedByDeviceEnabled = _YES;
	else
		support4PwrMgmtInitiatedByDeviceEnabled = _NO;

}


void captureDeviceState() {
	if (((sataRegs->P0SSTS &= (AHCI_PxSCTL_PxSSTS_SPD))>>4) == 1)
		lineSpeedNegotiated = GEN1;
	else if (((sataRegs->P0SSTS &= (AHCI_PxSCTL_PxSSTS_SPD))>>4) == 2)
		lineSpeedNegotiated = GEN2;

	cmdListWrPrdCnt=CmdLists[WRITE_CMD_SLOT].DW1.PrdByteCnt;
	cmdListRdPrdCnt=CmdLists[READ_CMD_SLOT].DW1.PrdByteCnt;
	if (cmdListWrPrdCnt==cmdListRdPrdCnt)
		wrRdByteCntCompStatus=BYTE_COUNT_EQUAL;
	else
		wrRdByteCntCompStatus=BYTE_COUNT_NOT_EQUAL;
}

/*
void resumeSataCntrlClk(void) {
	    // setting  force mode to bypass clock acknowledge from module
	    CSL_FINS(pscRegs->MDCTL[LPSC_SATA-PSC1_OFFSET], PSC_MDCTL_FORCE, 0x1) ;

	    //Enable the LPSC
	    //Enable SATA Clk
		lpscEnable(0, LPSC_SATA);

}
*/

void exitHostPwrDwn(void) {
//		sataRegs->P0CMD &= !AHCI_PxCMD_ST;
		sataRegs->P0CMD |= (1<<AHCI_PxCMD_ICC_SHIFT);
//		sataRegs->P0CMD |= (1<<AHCI_PxCMD_SUD_SHIFT);
//		sataRegs->P0SCTL |= 1;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#if 1
void pllInit() {
   #ifdef _AVV_TEST 
   # else 
      #if defined(GIO_HANDSHAKE)
        // pllInit is not used for any TDL tests
        // Enable PLL mode
        CSL_PLLC_0_REGS->PLLCTL |= 0x00000001;   // PLLEN=1
        CSL_PLLC_1_REGS->PLLCTL |= 0x00000001;   // PLLEN=1
      #else
   unsigned short j;

   // Deassert PLL0 Powerdown(bit1) and PLL Disable (Bit4) and PLL Enable Mux (Bit5)
   CSL_PLLC_0_REGS->PLLCTL &= PLLC0_PD_DIS_CMD; // Default 0xFFFFFFCD;
   CSL_PLLC_1_REGS->PLLCTL &= PLLC1_PD_DIS_CMD; // Default 0xFFFFFFCD;
   // Deassert PLL Reset(bit3)
   CSL_PLLC_0_REGS->PLLCTL |= PLLC0_RST_CMD; // Default 0x00000008;
   CSL_PLLC_1_REGS->PLLCTL |= PLLC1_RST_CMD; // Default 0x00000008;
   
   // Programming PLL frequency
   CSL_PLLC_0_REGS->PLLM    = PLLC0_PLLM; // default 0x00000015 => 22x multiplier --> 27MHz*22 = ~600MHz
   CSL_PLLC_1_REGS->PLLM    = PLLC1_PLLM; // default 0x00000015 => 22x multiplier --> 27MHz*22 = ~600MHz

   CSL_PLLC_0_REGS->POSTDIV = PLLC0_POSTDIV; ; // default 0x00008001. Postdivide by 2 --> ~300Mhz
   CSL_PLLC_1_REGS->POSTDIV = PLLC1_POSTDIV; ; // default 0x00008001. Postdivide by 2 --> ~300Mhz

   // Set the GOSET bit
   CSL_PLLC_0_REGS->PLLCMD = PLLC0_CMD;   // Default 0x00000001
   CSL_PLLC_1_REGS->PLLCMD = PLLC1_CMD;   // Default 0x00000001
   
   // Wait for PLL to Lock
   for (j = 0; j < PLLLOCK_WAIT; j++);
   
   // Enable PLL0/1 mode
   CSL_PLLC_0_REGS->PLLCTL |= PLLC0_PLLEN;   // Default 0x00000001; PLLEN=1
   CSL_PLLC_1_REGS->PLLCTL |= PLLC1_PLLEN;   // Default 0x00000001; PLLEN=1

   CSL_PLLC_1_REGS->PLLDIV1 |= PLL1_PLLDIV1;     // Default 0x00008000. Enable sysclk1 for DDR clk

   #ifdef PLLASYNC3
   CSL_PLLC_1_REGS->PLLDIV2 |= PLL1_PLLDIV2;     // Default 0x00008001. Enable sysclk2 for AYNC3 clk
   // Unlock the pinmux registers with the Kick sequence
   CSL_BOOTCFG_0_REGS->KICK0R = 0x83e70b13;  // Kick0 register + data (unlock)
   CSL_BOOTCFG_0_REGS->KICK1R = 0x95a4f1e0;  // Kick1 register + data (unlock)

   // Enable ASYNC3 clock domain from PLLC2 sysclk2
   CSL_BOOTCFG_0_REGS->CFGCHIP3 = 0x000000010; // Select PLLC1 clock for ASYNC3 clock domain
   #endif

   #ifdef PLL0EXTCLK
   CSL_PLLC_1_REGS->PLLDIV3 |= PLL1_PLLDIV3;     // Default 0x00008002. Enable sysclk3 for AYNC3 clk
   CSL_PLLC_0_REGS->PLLCTL  |= 0x00000200;   // Select external clock source
   #endif

      #endif
   #endif
}

#endif

