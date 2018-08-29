//***************************************************************************
//  Copyright(c) 2000-2008, Texas Instruments Incorporated. All Rights Reserved
//
//  FILE:   stat_ahci.h Test program for SATA Module
//
//  DESCRIPTION:
//      This file provides functions used by the SATA code to perform common
//      tasks.
//
//  Note: Requires the sata_utilities.c/h
//
//
//  HISTORY:
//      25July08 Zegeye Alemu - Original version written
//
//***************************************************************************
#ifndef _SATA_AHCI_H
#define	_SATA_AHCI_H
/* unsigned quantities */
typedef uint32_t   	Uint32;
typedef uint16_t 	Uint16;
typedef uint8_t		Uint8;


#define NUMOFPORTS       (1)	// Freon Supports A Single HBA Port. However it can support up to
						    	 //  16 additional Ports with the use of an external Port Multiplier.
						    	 //  So keep this value to 1.
#define LISTLENGTH       (4)	// Max Command Header Per Port is 32

#define WRITE_CMD_SLOT   (0)	// Value used here should be <= LISTLENGTH-1
#define READ_CMD_SLOT    (1)	// Value used here should be <= LISTLENGTH-1
	// WARNING. PRDLENGTH can not be greater than 8 for this program.
	//  See Note captured by the area when memory has been reserved for
	//  within sata_utilities.c for Command Table "CmdTable" for
	//  more information.

//#define _MAX_DATA_TRANSFER_	// Define this in project file when needed.

#ifndef _MAX_DATA_TRANSFER_ // 512 Bytes Data Size within 2 PRD Descriptors.
	#define PRDLENGTH     (2)   	// Max PRD Length is 65535 per port.
	#define DATABUFFERLEN (256) 	// DMA Data Buffer Length
#else // Max Data Size Transfer 8K Bytes within 2 PRD Descriptors
	#define PRDLENGTH     (2)   	// Max PRD Length is 65535 per port.
	#define DATABUFFERLEN (4096) 	// DMA Data Buffer Length
#endif

#if ((PRDLENGTH > 8) | (WRITE_CMD_SLOT > LISTLENGTH-1) | (READ_CMD_SLT > LISTLENGTH-1))
#error PRDLENGTH ENTRY ERROR - PROGRAM HARD CODED FOR MAX VALUE OF 8 - CMD SLOT ENTRY ERROR
#endif
/////////////////////////////////////////////////////////////////////////////////////////////
// Maximum of 32 commands slots per port exist where each command occupies 8 DWs (64 Bytes).
// The structure 'CmdListHeader' defines a single command header definition.
// The start of the first Command List &CmdListHeader[0] needs to be programmed onto P0CLB.
//
// Command List Base Address should be 1K Byte Aligned.

typedef struct {
	Uint32 CmdLen:5;   //bits[4:0]
	Uint32 Atapi:1;    //bit[5]
	Uint32 Write:1;    //bit[6]
	Uint32 Prefetch:1; //bit[7]
	Uint32 Reset:1;    //bit[8]
	Uint32 Bist:1;     //bit[9]
	Uint32 Rok:1;      //bit[10]
	Uint32 Rsv:1;      //bit[11]
	Uint32 Pmp:4;      //bits[15:12]
	Uint32 Prdtl:16;   //bits[31:16]
}CmdListHeaderW0;


typedef struct {
	Uint32 PrdByteCnt; //bits[31:0]
}CmdListHeaderW1;


typedef struct {
//	Uint32 CmdTableAddLowRsv:7; //bit[6:0]
//	Uint32 CmdTableAddLow:25;   //bits[31:7]
	Uint32 CmdTableAddLow;   //bits[31:7]
}CmdListHeaderW2;

typedef struct {
	Uint32 CmdTableAddHigh;     //bits[31:0]
}CmdListHeaderW3;


typedef struct {
	CmdListHeaderW0 DW0;
	CmdListHeaderW1 DW1;
	CmdListHeaderW2 DW2;
	CmdListHeaderW3 DW3;
	Uint32          DW4;
	Uint32          DW5;
	Uint32          DW6;
	Uint32          DW7;
} CmdListHeader;


//CmdListHeader CmdList[LISTLENGTH] = {0};
//CmdListHeader * pCmdList=CmdList;


/////////////////////////////////////////////////////////////////////////////////////////////
// Command Table requires alignment of 128 Bytes. CTBA should be programmed with this
//  restriction.

// Command Table Data Structure
// Members: Command FIS
//          ATAPI Command
//          PRD Table

// Host to Device FIS Definition also named as Command FIS in AHCI Specification.
//
//-----------Command FIS-------------------------

typedef struct {
	Uint32 B0FisType:8;//bits[7:0]
	Uint32 BYTE1:8;       //bits[15:8]
	Uint32 B2Cmd:8;    //bits[23:16]
	Uint32 B3Feature:8;//bits[31:24]
}CmdFisWord0;

typedef struct {
	Uint32 B0LbaLow:8; //bits[7:0]
	Uint32 B1LbaMid:8; //bits[15:8]
	Uint32 B2LbaHigh:8;//bits[23:16]
	Uint32 B3Device:8; //bits[31:24]
}CmdFisWord1;

typedef struct {
	Uint32 B0LbaLowExp:8; //bits[7:0]
	Uint32 B1LbaMidExp:8; //bits[15:8]
	Uint32 B2LbaHighExp:8;//bits[23:16]
	Uint32 B3FeatureExp:8; //bits[31:24]
}CmdFisWord2;

typedef struct {
	Uint32 B0SecCnt:8;    //bits[7:0]
	Uint32 B1SecCntExp:8; //bits[15:8]
	Uint32 B2Rsv:8;       //bits[23:16]
	Uint32 B3Control:8;   //bits[31:24]
}CmdFisWord3;


typedef struct {
	Uint32 DWResv; //bits[31:0]
}CmdFisWord4;

typedef struct {
	CmdFisWord0 DW0;
	CmdFisWord1 DW1;
	CmdFisWord2 DW2;
	CmdFisWord3 DW3;
	CmdFisWord4 DW4;
	Uint32      DW5;
	Uint32      DW6;
	Uint32      DW7;
	Uint32      DW8;
	Uint32      DW9;
	Uint32      DW10;
	Uint32      DW11;
	Uint32      DW12;
	Uint32      DW13;
	Uint32      DW14;
	Uint32      DW15;
}CommandFIS;

//CommandFIS CmdFis;
//CommandFIS *pCmdFis=&CmdFis;

//-----------Command FIS end ATAPI Command ------

// ATAPI Command Data Structure
typedef struct {
	Uint32 ATAPI[4];
}Atapi;

//-----------ATAPI Command end PRDT -------------
// Physical Region Descriptor Table Data Structure
typedef struct {
	Uint32 DbaLow;   //bits[31:0]
}DbaAddressLow;

typedef struct {
	Uint32 DbaHigh; //bits[31:0]
}DbaAddressHigh;

typedef struct {
	Uint32 DW2Reserved; //bits[31:0]
}PrdtRsv;

typedef struct {
	Uint32 DataBC:22;  //bits[21:0]
}DataByteCnt;


typedef struct {
	DbaAddressLow  DW0;
	DbaAddressHigh DW1;
	PrdtRsv        DW2;
	DataByteCnt    DW3;
}PRDT;
//-----------PRDT end ---------------------------

//-----------Command Table Data Structure -------
typedef struct {
	CommandFIS cfis;
	Atapi      atapi;
	Uint32     Rsv[12];
	PRDT       prdTable[8];	// Have forced this size to 8 in order to meet the minimum
							//  required size for Command Table.
}CommandTable;

//-----------Command Table Data Structure end ---


/////////////////////////////////////////////////////////////////////////////////////////////
// Receive FIS requires the Receive FIS to be 256 byte aligned. P0FB should be programmed
//  with this restriction.
//
// RECEIVE FIS Data Structure
// Members: DMA Setup FIS (DSFIS)
//          PIO Setup FIS (PSFIS)
//          D2H Register FIS (RFIS)
//          Set Device Bits FIS (SDBFIS)
//          Unknown FIS (UFIS)

//-----------DMA Setup FIS-----------------------

typedef struct {
	Uint32 B0FisType:8;//bits[7:0]
	Uint32 BYTE1:8;       //bits[15:8]
	Uint32 B2Rsv:8;    //bits[23:16]
	Uint32 B3Rsv:8;    //bits[31:24]
}DsfisW0;



typedef struct {
	DsfisW0 DW0;				// 4 x 4B	= 16B
	Uint32 DW1DmaBuffLow;		// 4B
	Uint32 DW2DmaBuffHigh;		// 4B
	Uint32 DW3Rsv;				// 4B
	Uint32 DW4DmaBuffOffset;	// 4B
	Uint32 DW5DmaXfrCnt;		// 4B
	Uint32 DW6Rsv;				// 4B
}DMASetupFis;					// = 40B

//-----------DMA Setup FIS end PIO Setup FIS ----

typedef struct {
	Uint32 B0FisType:8;//bits[7:0]
	Uint32 BYTE1:8;       //bits[15:8]
	Uint32 B2Status:8;    //bits[23:16]
	Uint32 B3Errror:8;//bits[31:24]
}PioSetupDW0;	// 16B

typedef struct {
	Uint32 B0LbaLow:8; //bits[7:0]
	Uint32 B1LbaMid:8; //bits[15:8]
	Uint32 B2LbaHigh:8;//bits[23:16]
	Uint32 B3Device:8; //bits[31:24]
}PioSetupDW1;

typedef struct {
	Uint32 B0LbaLowExp:8; //bits[7:0]
	Uint32 B1LbaMidExp:8; //bits[15:8]
	Uint32 B2LbaHighExp:8;//bits[23:16]
	Uint32 B3Rsv:8;       //bits[31:24]
}PioSetupDW2;


typedef struct {
	Uint32 B0SecCnt:8;    //bits[7:0]
	Uint32 B1SecCntExp:8; //bits[15:8]
	Uint32 B2Rsv:8;       //bits[23:16]
	Uint32 B3Estatus:8;   //bits[31:24]
}PioSetupDW3;

typedef struct {
	Uint32 HW0XferCnt:16; //bits[15:0]
	Uint32 HW1Rsv:16;     //bits[31:16]
}PioSetupDW4;


typedef struct {
	PioSetupDW0 DW0;	// 16B
	PioSetupDW1 DW1;	// 16B
	PioSetupDW2 DW2;	// 16B
	PioSetupDW3 DW3;	// 16B
	PioSetupDW4 DW4;	// 16B
}PIOSetupFis;			// 80B

//-----------PIO Setup FIS end D2H Reg FIS-------


typedef struct {
	Uint32 B0FisType:8;//bits[7:0]
	Uint32 BYTE1:8;       //bits[15:8]
	Uint32 B2Status:8;    //bits[23:16]
	Uint32 B3Errror:8;//bits[31:24]
}D2HRegDW0;

typedef struct {
	Uint32 B0LbaLow:8; //bits[7:0]
	Uint32 B1LbaMid:8; //bits[15:8]
	Uint32 B2LbaHigh:8;//bits[23:16]
	Uint32 B3Device:8; //bits[31:24]
}D2HRegDW1;

typedef struct {
	Uint32 B0LbaLowExp:8; //bits[7:0]
	Uint32 B1LbaMidExp:8; //bits[15:8]
	Uint32 B2LbaHighExp:8;//bits[23:16]
	Uint32 B3Rsv:8;       //bits[31:24]
}D2HRegDW2;


typedef struct {
	Uint32 B0SecCnt:8;    //bits[7:0]
	Uint32 B1SecCntExp:8; //bits[15:8]
	Uint32 HW1Rsv:16;     //bits[31:16]
}D2HRegDW3;

typedef struct {
	Uint32 W0Rsv;         //bits[31:0]
}D2HRegDW4;


typedef struct {
	D2HRegDW0 DW0;
	D2HRegDW1 DW1;
	D2HRegDW2 DW2;
	D2HRegDW3 DW3;
	D2HRegDW4 DW4;
}D2HRegFis;

//-----------D2H Reg FIS end Set Device Bits FIS-
// The Set Device Bit FIS definition does not contain the 2nd Word required
//  for Native Command Queueing. This second word is the SACTVE register and
//  the AHCI takes care of updating SACTIVE register at its location.

typedef struct {
	Uint32 B0FisType:8;//bits[7:0]
	Uint32 BYTE1:8;       //bits[15:8]
	Uint32 B2Status:8; //bits[23:16]
	Uint32 B3Errror:8; //bits[31:24]
}SetDevBitsDW0;

typedef struct {
	Uint32 SActive;    //bits[31:0]
}SetDevBitsDW1;

typedef struct {
	SetDevBitsDW0 DW0;
	SetDevBitsDW1 DW1;
}SetDevBitsFis;

//-----------Set Device Bits FIS end Unkonwn FIS-

typedef struct {
	Uint32 UserDefined; //bits[31:0]
}UnknownDWx;


typedef struct {
	UnknownDWx DW[16]; // 16 Words (Max 64 Bytes allowed)
}UnknownFis;

//-----------Unkonw FIS end----------------------

//-----------Receive Register FIS Structure------

typedef struct {
	DMASetupFis   DSFIS;		// 40B
	Uint32        Rsv1;			// 4B
	PIOSetupFis   PSFIS;		// 80B
	Uint32        Rsv2[3];
	D2HRegFis     RFIS;
	Uint32        Rsv3;
	SetDevBitsFis SDBFIS;
	UnknownFis    UFIS;
}ReceiveFis;

//-----------Receive Register FIS Structure End--

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@



extern unsigned char prdTableDataBuff[LISTLENGTH][PRDLENGTH][DATABUFFERLEN];

extern CmdListHeader CmdLists[];
extern CommandTable CmdTable[];
extern ReceiveFis RcvFis;
extern Uint16 cmdSlot2Use;

#endif /* _SATA_AHCI_H */
