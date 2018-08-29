//***************************************************************************
//  Copyright(c) 2000-2008, Texas Instruments Incorporated. All Rights Reserved
//
//  FILE:   stat_utilities.h Test program for SATA Module
//
//  DESCRIPTION:
//      This file provides functions used by the SATA code to perform common
//      tasks.
//
//  Note: Requires the sata_utilities.c and sata_ahci.h
//
//
//  HISTORY:
//      25July08 Zegeye Alemu - Original version written
//
//***************************************************************************
#ifndef _SATA_UTILITIES_AHCI_H
#define	_SATA_UTILITIES_AHCI_H

#include "speed_select.h"

//#define DESIRED_SPEED			(GEN1)       // GOASFASTASDEVICE, GEN1, GEN2
#define DEVICE_LBA_ADDRESS      (0x00000002) // Dev28bitLbaAddress = 28-Bit LBA Address
#define WAIT_500_MILLISECONDS   (50)		 // This should be set to 500 once the ONE_MS_VALUE is programmed correctly.
#define WAIT_1_MILLISECOND      (1)
#define ONE_MS_VALUE			(1)			 // Number of CPU Cycles needed to generate a millisecond wait time.
//* Note: Use this rule to program P0DMACR Register:
//*       Burst Size <= Transaction Size
//*  The followings is the combination of allowed options.
//*  Note: transaction value =3 and burst value = 0 or 4 gives the max throughput.
//*
//*       transaction       burst
//*           3          4,3,2,1,0
//*           2          3,2,1,0
//*           1          2,1,0
//*           0          1,0

#define DMA_BURST_LENGTH        (0x4)        // [0x0 - 0x4] Burst=2^(n-1) i.e., 0x4=> 2^(4-1)=8
#define DMA_TRANACTION_LENGTH   (0x3)        // [0x0 - 0x3] TransLength=2^n i.e., 0xA=> 2^3=8
// CCC_CTL REGs Fields
#define CCC_TIMEOUT_MS_VALUE    (0x1234)     // 16-bit Field
#define CCC_NUM_OF_CMDS         (0xCD)       // 8-bit Field
// 1MS Timer Reg Field
#define ONE_MS_TIMER_RESOLUTION (100000)     // [0 - 0x3FFFFF]


typedef enum {
	_NO=0,
	_YES=1,
	_NOT_DETERMINED=2
}sataCapability;

typedef enum {
	GOASFASTASDEVICE=0,
	GEN1=1,
	GEN2=2,
	COMMNOTESTABLISHED=0xFF
}interfaceSpeed;

typedef enum {
	GLOBALint=0x11,
	PORTint=0x22	// PortInt=LocalInt
}intType;

typedef enum {
	ENABLE=0x33,
	DISABLE=0x55
}intState;

typedef enum {
	INTFIELDS=0x77,
	ERRORFIELDS=0x88
}intOrErrorDiag;

typedef enum {
	SETINT=0x44,
	CLEARINT=0x66
}intStatus;

typedef enum {
	QUEUED_CMD=0xAB,
	NON_QUEUED_CMD=0xDC
}cmdType;

typedef enum {
	DATA_DIR_RD=0xBA,
	DATA_DIR_WR=0xCD
}dataXferDir;

typedef enum {
	PIO_PROTOCOL,
	DMA_PROTOCOL
}xferProtocol;


typedef enum {
	USE_POLLING=8,
	USE_INT_HANDLER
} interruptHandlingMethod;


typedef enum {
	supportPmOnly=0xBB,
	supportSmOnly=0xCC,
	supportBothPm=0xDD,
	supportNone  =0xEE,
	notDetermined=0xFF
}pwrMgmtCapability;


typedef enum {
	BYTE_COUNT_EQUAL=0x1A,
	BYTE_COUNT_NOT_EQUAL=0xA1
}compareResult;

/*
typedef enum {
	ENABLE=0x33,
	DISABLE=0x55
}lowPwrState;
*/
typedef intState lowPwrState;


typedef enum {
	hostPTnoRestricition             = 0xCF,
	hostPTtoPartialNotAllowed        = 0xDF,
	hostPTtoSlumberNotAllowed        = 0xEF,
	hostPTtoPartialSlumberNotAllowed = 0xFF,
	HostPTisNotDetermined            = 0x00
}hostPwrMgmtState;


typedef enum {
	hostCfgSlumberState = 0xCF,
	hostCfgPartialState = 0xDF,
	hostCfgDisPwrDwn    = 0xEF,
	notAccessed         = 0xFF
}hostPwrMgmtConfig;

typedef struct {
	Uint32  capSMPS:1;      // bit[0] // AM-1808 has bonded out an input pin for this purpose. So, set it to 1.
	Uint32  capSSS:1;       // bit[1] // Need to set this to 1 regardless to the support of SSS or not since we are supporting a single HBA port. If using a PM then need to enable this bit one at a time per PM port basis.
	Uint32  piPi:2;         // bits[3:2] // For AM-1808 this should always be set to 1 since bit 0 corresponds to a sigle HBA port support. When using PM, use Command Based switching to access multiple ports.
	Uint32  p0cmdCpd:1;     // bit[4] // We have bonded out the two signals (one input and one output) for Detecting Device that are Bus Powered
	Uint32  p0cmdEsp:1;     // bit[5] // This is most likely needs to be set to Zero since it is mutually exclusive with MPSP.
	Uint32  p0cmdMpsp:1;    // bit[6] // We have bonded out a pin (input) to detect a change on a switch or line
	Uint32  p0cmdHpcp:1;    // bit[7] // Since we support CPD and MPSP by default we will have to support Hot Plugable Capability.
	Uint32  rsv:24;         // bits[24:8] Reserved and populate if need to add more control
}FirmwareCtrlFeatures;

//Sector Count   Sector Count
//LBA Low        Sector Number
//LBA Mid        Cylinder Low
//LBA High       Cylinder High


// The following structures are subsets of command FIS that I have interest of and are not here to define the actual Command FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint8 cfisType;
	Uint8 cfisByte1;
	Uint8 cfisCmd;
	Uint8 cfisFeature;
	Uint8 cfisDw1SecNumLbaLow;
	Uint8 cfisDw1CylLowLbaMid;
	Uint8 cfisDw1CylHighLbahigh;
	Uint8 cfisDw1Dev;
	Uint8 cfisDw2SecNumLbaLowExp;
	Uint8 cfisDw2CylLowLbaMidExp;
	Uint8 cfisDw2CylHighLbahighExp;
	Uint8 cfisDw2FeatureExp;
	Uint8 cfisDw3SecCnt;
	Uint8 cfisDw3SecCntExp;
	Uint8 cfisDw3Ctrl;
}cmdFis;

// The following structures are subsets of DMA Setup FIS that I have interest of and are not here to define the actual FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint8 dsfisType;
	Uint8 dsfisByte1;
	Uint32 dsfisDw1DmaBuffLow;
	Uint32 dsfisDw2DmaBuffHigh;
	Uint32 dsfisDw4DmaBuffOffset;
	Uint32 dsfisDw5DmaXferCnt;
}dsFis;

// The following structures are subsets of PIO Setup FIS that I have interest of and are not here to define the actual FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint8 psfisType;
	Uint8 psfisByte1;
	Uint8 psfisStatus;
	Uint8 psfisError;
	Uint8 psfisDw1SecNumLbaLow;
	Uint8 psfisDw1CylLowLbaMid;
	Uint8 psfisDw1CylHighLbahigh;
	Uint8 psfisDw1Dev;
	Uint8 psfisDw3SecCnt;
	Uint8 psfisDw3Estatus;
	Uint16 psfisDw4XferCnt;
}piosFis;

// The following structures are subsets of Device to Host Register FIS that I have interest of and are not here to define the actual FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint8 regfisType;
	Uint8 regfisByte1;
	Uint8 regfisStatus;
	Uint8 regfisError;
	Uint8 regfisDw1SecNumLbaLow;
	Uint8 regfisDw1CylLowLbaMid;
	Uint8 regfisDw1CylHighLbahigh;
	Uint8 regfisDw1Dev;
	Uint8 regfisDw3SecCnt;
}regFis;

// The following structures are subsets of Set Device Bits FIS that I have interest of and are not here to define the actual FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint8 sdbfisType;
	Uint8 sdbfisByte1;
	Uint8 sdbfisStatus;
	Uint8 sdbfisError;
}sdbFis;

// The following structures are subsets of Unknown FIS that I have interest of and are not here to define the actual FIS. See sata_ahci.h for FIS defintion.
typedef struct {
	Uint32 ufisWord[16];
}uFis;

// CCC
#define AHCI_CCCCTL_EN_SHIFT         (0)
#define AHCI_CCCCTL_INT_SHIFT        (3)
#define AHCI_CCCCTL_CC_SHIFT         (8)
#define AHCI_CCCCTL_TV_SHIFT         (16)


#define AHCI_CCCCTL_EN               (0x1    << AHCI_CCCCTL_EN_SHIFT)
#define AHCI_CCCCTL_INT              (0x1f   << AHCI_CCCCTL_INT_SHIFT)
#define AHCI_CCCCTL_CC               (0xff   << AHCI_CCCCTL_CC_SHIFT)
#define AHCI_CCCCTL_TV               (0xffff << AHCI_CCCCTL_TV_SHIFT)


// P0CMD
#define	AHCI_PxCMD_ST_SHIFT	   (0)
#define	AHCI_PxCMD_SUD_SHIFT   (1)
#define	AHCI_PxCMD_POD_SHIFT   (2)
#define	AHCI_PxCMD_CLO_SHIFT   (3)
#define	AHCI_PxCMD_FRE_SHIFT   (4)
#define	AHCI_PxCMD_CCS_SHIFT   (8)
#define	AHCI_PxCMD_MPSS_SHIFT  (13)
#define	AHCI_PxCMD_FR_SHIFT    (14)
#define	AHCI_PxCMD_CR_SHIFT    (15)
#define	AHCI_PxCMD_CPS_SHIFT   (16)
#define	AHCI_PxCMD_PMA_SHIFT   (17)
#define	AHCI_PxCMD_HPCP_SHIFT  (18)
#define	AHCI_PxCMD_MPSP_SHIFT  (19)
#define	AHCI_PxCMD_CPD_SHIFT   (20)
#define	AHCI_PxCMD_ESP_SHIFT   (21)
#define	AHCI_PxCMD_ATAPI_SHIFT (24)
#define	AHCI_PxCMD_DLAE_SHIFT  (25)
#define	AHCI_PxCMD_ALPE_SHIFT  (26)
#define	AHCI_PxCMD_ASP_SHIFT   (27)
#define	AHCI_PxCMD_ICC_SHIFT   (28)

/* port command and status bits */
#define	AHCI_PxCMD_ST	  (0x1 << AHCI_PxCMD_ST_SHIFT)   /* Start */
#define	AHCI_PxCMD_SUD	  (0x1 << AHCI_PxCMD_SUD_SHIFT)  /* Spin-up device */
#define	AHCI_PxCMD_POD	  (0x1 << AHCI_PxCMD_POD_SHIFT)  /* Power on device */
#define	AHCI_PxCMD_CLO	  (0x1 << AHCI_PxCMD_CLO_SHIFT)  /* Command list override */
#define	AHCI_PxCMD_FRE	  (0x1 << AHCI_PxCMD_FRE_SHIFT)  /* FIS receive enable */
#define	AHCI_PxCMD_CCS	  (0x1f << AHCI_PxCMD_CCS_SHIFT) /* Current command slot */
			/* Mechanical presence switch state */
#define	AHCI_PxCMD_MPSS	  (0x1 << AHCI_PxCMD_MPSS_SHIFT)
#define	AHCI_PxCMD_FR	  (0x1 << AHCI_PxCMD_FR_SHIFT)   /* FIS receiving running */
#define	AHCI_PxCMD_CR	  (0x1 << AHCI_PxCMD_CR_SHIFT)   /* Command list running */
#define	AHCI_PxCMD_CPS	  (0x1 << AHCI_PxCMD_CPS_SHIFT)  /* Cold presence state */
#define	AHCI_PxCMD_PMA	  (0x1 << AHCI_PxCMD_PMA_SHIFT)  /* Port multiplier attached */
#define	AHCI_PxCMD_HPCP	  (0x1 << AHCI_PxCMD_HPCP_SHIFT) /* Hot plug capable port */
			/* Mechanical presence switch attached to port */
#define	AHCI_PxCMD_MPSP	  (0x1 << AHCI_PxCMD_MPSP_SHIFT)
#define	AHCI_PxCMD_CPD	  (0x1 << AHCI_PxCMD_CPD_SHIFT)  /* Cold presence detection */
#define	AHCI_PxCMD_ESP	  (0x1 << AHCI_PxCMD_ESP_SHIFT)  /* External SATA port */
#define	AHCI_PxCMD_ATAPI  (0x1 << AHCI_PxCMD_ATAPI_SHIFT)/* Device is ATAPI */
#define	AHCI_PxCMD_DLAE	  (0x1 << AHCI_PxCMD_DLAE_SHIFT) /* Drive LED on ATAPI enable */
			/* Aggressive link power magament enable */
#define	AHCI_PxCMD_ALPE	  (0x1 << AHCI_PxCMD_ALPE_SHIFT)
#define	AHCI_PxCMD_ASP	  (0x1 << AHCI_PxCMD_ASP_SHIFT)  /* Aggressive slumber/partial */
			/* Interface communication control */
#define	AHCI_PxCMD_ICC	  (0xf << AHCI_PxCMD_ICC_SHIFT)

// P0SCTL and P0SSTS
#define AHCI_PxSCTL_PxSSTS_DET_SHIFT   (0)
#define AHCI_PxSCTL_PxSSTS_SPD_SHIFT   (4)
#define AHCI_PxSCTL_PxSSTS_IPM_SHIFT   (8)

#define AHCI_PxSCTL_PxSSTS_DET	       (0xf << AHCI_PxSCTL_PxSSTS_DET_SHIFT)
#define AHCI_PxSCTL_PxSSTS_SPD	       (0xf << AHCI_PxSCTL_PxSSTS_SPD_SHIFT)
#define AHCI_PxSCTL_PxSSTS_IPM	       (0xf << AHCI_PxSCTL_PxSSTS_IPM_SHIFT)

/*
     1   0    0    0    0     0    0    0    0    1    0    0    0    0    0    0

  |                   |                   |                   |                   |
    31   30   29   28   27   26   25   24   23   22   21   20   19   18   17   16
  +----+----+----+----+----+----+----+----+----+----+-----------------------------+
  |CPDS|TFES|HBFS|HBDS|IFS |INFS|RSV |OFS |IPMS|PRCS|          RESERVED           |
  +----+----+----+----+----+----+----+----+----+----+-----------------------------+


     0   0    0    0    0     0    0    0    1    1    0    0    0    0    0    0
  |                   |                   |                   |                   |
   15                                       07   06   05   04   03   02   01   00
  +---------------------------------------+----+----+----+----+----+----+----+----+
  |             RESERVED                  |DMPS|PCS |DPS |UFS |SDBS|DSS |PSS |DHRS|
  +---------------------------------------+----+----+----+----+----+----+----+----+

*/

// P0IS
#define AHCI_P0IS_DHRS_SHIFT     (0)
#define AHCI_P0IS_PSS_SHIFT      (1)
#define AHCI_P0IS_DSS_SHIFT      (2)
#define AHCI_P0IS_SDBS_SHIFT     (3)
#define AHCI_P0IS_UFS_SHIFT      (4)
#define AHCI_P0IS_DPS_SHIFT      (5)
#define AHCI_P0IS_PCS_SHIFT      (6)
#define AHCI_P0IS_DMPS_SHIFT     (7)
#define AHCI_P0IS_PRCS_SHIFT     (22)
#define AHCI_P0IS_IPMS_SHIFT     (23)
#define AHCI_P0IS_OFS_SHIFT      (24)
#define AHCI_P0IS_INFS_SHIFT     (26)
#define AHCI_P0IS_IFS_SHIFT      (27)
#define AHCI_P0IS_HBDS_SHIFT     (28)
#define AHCI_P0IS_HBFS_SHIFT     (29)
#define AHCI_P0IS_TFES_SHIFT     (30)
#define AHCI_P0IS_CPDS_SHIFT     (31)


#define AHCI_P0IS_DHRS           (0x1 << AHCI_P0IS_DHRS_SHIFT)
#define AHCI_P0IS_PSS            (0x1 << AHCI_P0IS_PSS_SHIFT)
#define AHCI_P0IS_DSS            (0x1 << AHCI_P0IS_DSS_SHIFT)
#define AHCI_P0IS_SDBS           (0x1 << AHCI_P0IS_SDBS_SHIFT)
#define AHCI_P0IS_UFS            (0x1 << AHCI_P0IS_UFS_SHIFT)
#define AHCI_P0IS_DPS            (0x1 << AHCI_P0IS_DPS_SHIFT)
#define AHCI_P0IS_PCS            (0x1 << AHCI_P0IS_PCS_SHIFT)
#define AHCI_P0IS_DMPS           (0x1 << AHCI_P0IS_DMPS_SHIFT)
#define AHCI_P0IS_PRCS           (0x1 << AHCI_P0IS_PRCS_SHIFT)
#define AHCI_P0IS_IPMS           (0x1 << AHCI_P0IS_IPMS_SHIFT)
#define AHCI_P0IS_OFS            (0x1 << AHCI_P0IS_OFS_SHIFT)
#define AHCI_P0IS_INFS           (0x1 << AHCI_P0IS_INFS_SHIFT)
#define AHCI_P0IS_IFS            (0x1 << AHCI_P0IS_IFS_SHIFT)
#define AHCI_P0IS_HBDS           (0x1 << AHCI_P0IS_HBDS_SHIFT)
#define AHCI_P0IS_HBFS           (0x1 << AHCI_P0IS_HBFS_SHIFT)
#define AHCI_P0IS_TFES           (0x1 << AHCI_P0IS_TFES_SHIFT)
#define AHCI_P0IS_CPDS           (0x1 << AHCI_P0IS_CPDS_SHIFT)

#define AHCI_P0IS_ALL_INTERRUPTS (0xFDC000FF)

// PxTFD
//   7     6     5     4     3     2     1    0
//+-----+-----------------+-----+----------+------+
//| BSY |       CS        | DRQ |    CS    | ERR  |
//+-----+-----------------+-----+----------+------+
// BSY=BUSY
// CS=Command Specific
// DRQ=Data Request
// ERR=Error
#define AHCI_PxTFD_STS_SHIFT             (0)
#define AHCI_PxTFD_STS_BSY_DRQ_ERR_SHIFT (0)
#define AHCI_PxTFD_STS_ERR_SHIFT         (0)
#define AHCI_PxTFD_STS_DRQ_SHIFT         (3)
#define AHCI_PxTFD_STS_BSY_SHIFT         (7)
#define AHCI_PxTFD_ERR_SHIFT             (8)

#define AHCI_PxTFD_STS                   (0xff<< AHCI_PxTFD_STS_SHIFT)
#define AHCI_PxTFD_STS_BSY_DRQ_ERR       (0x88<< AHCI_PxTFD_STS_BSY_DRQ_ERR_SHIFT)
#define AHCI_PxTFD_STS_ERR	             (0x1 << AHCI_PxTFD_STS_ERR_SHIFT)
#define AHCI_PxTFD_STS_DRQ	             (0x1 << AHCI_PxTFD_STS_DRQ_SHIFT)
#define AHCI_PxTFD_STS_BSY	             (0x1 << AHCI_PxTFD_STS_BSY_SHIFT)
#define AHCI_PxTFD_ERR                   (0xff<< AHCI_PxTFD_ERR_SHIFT)



// GHC
#define AHCI_GHC_HR_SHIFT   (0)
#define AHCI_GHC_IE_SHIFT   (1)
#define AHCI_GHC_AE_SHIFT   (31)

#define AHCI_GHC_HR         (0x1 << AHCI_GHC_HR_SHIFT) // HBA Reset/Release
#define AHCI_GHC_IE	        (0x1 << AHCI_GHC_IE_SHIFT) // Global Int Enable/Disable
#define AHCI_GHC_AE	        (0x1 << AHCI_GHC_AE_SHIFT) // AHCI/Legacy


// IE
#define AHCI_P0IE_DHRE_SHIFT (0)
#define AHCI_P0IE_PSE_SHIFT  (1)
#define AHCI_P0IE_DSE_SHIFT  (2)
#define AHCI_P0IE_SDBE_SHIFT (3)
#define AHCI_P0IE_UFE_SHIFT  (4)
#define AHCI_P0IE_DPE_SHIFT  (5)
#define AHCI_P0IE_PCE_SHIFT  (6)
#define AHCI_P0IE_DMPE_SHIFT (7)
#define AHCI_P0IE_PRCE_SHIFT (22)
#define AHCI_P0IE_IPME_SHIFT (23)
#define AHCI_P0IE_OFE_SHIFT  (24)
#define AHCI_P0IE_INFE_SHIFT (26)
#define AHCI_P0IE_IFE_SHIFT  (27)
#define AHCI_P0IE_HBDE_SHIFT (28)
#define AHCI_P0IE_HBFE_SHIFT (29)
#define AHCI_P0IE_TFEE_SHIFT (30)
#define AHCI_P0IE_CPDE_SHIFT (31)

#define AHCI_P0IE_DHRE       (0x1 << AHCI_P0IE_DHRE_SHIFT)
#define AHCI_P0IE_PSE        (0x1 << AHCI_P0IE_PSE_SHIFT)
#define AHCI_P0IE_DSE        (0x1 << AHCI_P0IE_DSE_SHIFT)
#define AHCI_P0IE_SDBE       (0x1 << AHCI_P0IE_SDBE_SHIFT)
#define AHCI_P0IE_UFE        (0x1 << AHCI_P0IE_UFE_SHIFT)
#define AHCI_P0IE_DPE        (0x1 << AHCI_P0IE_DPE_SHIFT)
#define AHCI_P0IE_PCE        (0x1 << AHCI_P0IE_PCE_SHIFT)
#define AHCI_P0IE_DMPE       (0x1 << AHCI_P0IE_DMPE_SHIFT)
#define AHCI_P0IE_PRCE       (0x1 << AHCI_P0IE_PRCE_SHIFT)
#define AHCI_P0IE_IPME       (0x1 << AHCI_P0IE_IPME_SHIFT)
#define AHCI_P0IE_OFE        (0x1 << AHCI_P0IE_OFE_SHIFT)
#define AHCI_P0IE_INFE       (0x1 << AHCI_P0IE_INFE_SHIFT)
#define AHCI_P0IE_IFE        (0x1 << AHCI_P0IE_IFE_SHIFT)
#define AHCI_P0IE_HBDE       (0x1 << AHCI_P0IE_HBDE_SHIFT)
#define AHCI_P0IE_HBFE       (0x1 << AHCI_P0IE_HBFE_SHIFT)
#define AHCI_P0IE_TFEE       (0x1 << AHCI_P0IE_TFEE_SHIFT)
#define AHCI_P0IE_CPDE       (0x1 << AHCI_P0IE_CPDE_SHIFT)
#define AHCI_P0IE_ALL_FIELDS (0xFDC000FF)

// PHY
#define AHCI_P0PHYCR_MPY_SHIFT       (0)
#define AHCI_P0PHYCR_LB_SHIFT        (4)
#define AHCI_P0PHYCR_LOS_SHIFT       (6)
#define AHCI_P0PHYCR_RXINVPOLE_SHIFT (7)
#define AHCI_P0PHYCR_RXTERM_SHIFT    (8)
#define AHCI_P0PHYCR_RXCDR_SHIFT     (10)
#define AHCI_P0PHYCR_RXEQ_SHIFT      (13)
#define AHCI_P0PHYCR_TXINVPOLE_SHIFT (17)
#define AHCI_P0PHYCR_TXCM_SHIFT      (18)
#define AHCI_P0PHYCR_TXSWING_SHIFT   (19)
#define AHCI_P0PHYCR_TXDE_SHIFT      (22)
#define AHCI_P0PHYCR_OVERRIDE_SHIFT  (30)
#define AHCI_P0PHYCR_ENPLL_SHIFT     (31)

#define AHCI_P0PHYCR_MPY		     (0xf << AHCI_P0PHYCR_MPY_SHIFT)
#define AHCI_P0PHYCR_LB			     (0x3 << AHCI_P0PHYCR_LB_SHIFT)
#define AHCI_P0PHYCR_LOS		     (0x1 << AHCI_P0PHYCR_LOS_SHIFT)
#define AHCI_P0PHYCR_RXINVPOLE	     (0x1 << AHCI_P0PHYCR_RXINVPOLE_SHIFT)
#define AHCI_P0PHYCR_RXTERM		     (0x3 << AHCI_P0PHYCR_RXTERM_SHIFT)
#define AHCI_P0PHYCR_RXCDR		     (0x7 << AHCI_P0PHYCR_RXCDR_SHIFT)
#define AHCI_P0PHYCR_RXEQ		     (0xf << AHCI_P0PHYCR_RXEQ_SHIFT)
#define AHCI_P0PHYCR_TXINVPOLE	     (0x1 << AHCI_P0PHYCR_TXINVPOLE_SHIFT)
#define AHCI_P0PHYCR_TXCM		     (0x1 << AHCI_P0PHYCR_TXCM_SHIFT)
#define AHCI_P0PHYCR_TXSWING	     (0x7 << AHCI_P0PHYCR_TXSWING_SHIFT)
#define AHCI_P0PHYCR_TXDE		     (0xf << AHCI_P0PHYCR_TXDE_SHIFT)
#define AHCI_P0PHYCR_OVERRIDE	     (0x1 << AHCI_P0PHYCR_OVERRIDE_SHIFT)
#define AHCI_P0PHYCR_ENPLL		     (0x1 << AHCI_P0PHYCR_ENPLL_SHIFT)

// DMA
#define AHCI_P0DMACR_TXTS_SHIFT      (0)
#define AHCI_P0DMACR_RXTS_SHIFT      (4)
#define AHCI_P0DMACR_TXABL_SHIFT     (8)
#define AHCI_P0DMACR_RXABL_SHIFT     (12)


#define AHCI_P0DMACR_TXTS		     (0xf << AHCI_P0DMACR_TXTS_SHIFT)
#define AHCI_P0DMACR_RXTS 		     (0xf << AHCI_P0DMACR_RXTS_SHIFT)
#define AHCI_P0DMACR_TXABL 		     (0xf << AHCI_P0DMACR_TXABL_SHIFT)
#define AHCI_P0DMACR_RXABL 		     (0xf << AHCI_P0DMACR_RXABL_SHIFT)

// ATA Commands
// Note: READ_DMA_QUEUED and WRITE_DMA_QUEUED Legacy Commands do not
//        work with AHCI Controller. Need to use strictly
//        FPDMA_READ and FPDMA_WRITE Command for Native Command Queuing.
#define ATA_CMD_READ_SECTOR          (0x20) // PIO Reaad Sector(s) Command
#define ATA_CMD_WRITE_SECTOR         (0x30) // PIO Write Sector(s) Command
#define ATA_CMD_READ_DMA_EXT         (0x25) // Read DMA Ext: Used when using 48-Bit Addressing
#define ATA_CMD_WRITE_DMA_EXT        (0x35) // Write DMA Ext: Used when using 48-Bit Addressing
#define ATA_CMD_READ_DMA             (0xC8) // Read DMA: Used when using 28-Bit Addressing
#define ATA_CMD_WRITE_DMA            (0xCA) // Write DMA: Used when using 28-Bit Addressing
#define ATA_CMD_READ_DMA_QUEUED      (0xC7) // Legacy Queued Read Command (Not to be used by AHCI)
#define ATA_CMD_WRITE_DMA_QUEUED     (0xCC) // Legacy Queued Write Command (Not to be used by AHCI)
#define ATA_CMD_FPDMA_READ           (0x60) // Queued Read Command: First Party DMA Read Cmd
#define ATA_CMD_FPDMA_WRITE          (0x61) // Queued Write Command: First Party DMA Write Cmd
#define ATA_CMD_IDENTIFY_DEVICE      (0xEC) // Receive Device Parameter info using PIO Read Protocol.
#define ATA_CMD_SET_FEATURES		 (0xEF) // Set Feature Command (Used to Disable IORDY)
											//  Invoking this Cmd to ensure proper operation of
											//  SATA Core when invoking Non-Data Command.


// SUB Command of SET FEATURE to be written onot Features Reg.
#define ATA_SUB_CMD_XFER_MODE        (0x03)	// ATA Feature - Sub Command
#define SATA_FEATURE_ENABLE          (0x10) // SATA Feature - Sub Command
#define SATA_FEATURE_DISABLE         (0x90) // SATA Feature - Sbu Command

// Sector Count Register Contain Features to be Enabled/Disabled
#define ATA_SUB_CMD_XFER_MODE_VALUE  (0x01) // ATA Sub Command Specific
#define SATA_DEV_INITIATED_PWR_CAP   (0x03) // SATA Sub Command Specific


#define DEVICE_REG_USE_LBA_ADDRESSING (0x1 << 6) // This is Device Head Register not Control
#define CMDFIS_BYTE1_C_IS_CMD_UPDATE  (0x1 << 7)
#define CMDFIS_BYTE1_C_IS_CTRL_UPDATE (0x0 << 7)

// ATA Good Status Signature after Power Up or Port Reset
#define AHCI_P0SIG_SIG_ATA_DEV_GOOD_STAT     (0x00000101) // LBAhigh:LBAmid:LBAlow:SECcnt
#define AHCI_P0SIG_SIG_PACKET_DEV_GOOD_STAT  (0xEB140101) // LBAhigh:LBAmid:LBAlow:SECcnt
// Device


//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void sata_init_and_spin_up(void);
char placeHbaInIdle(void);
void waitForXms(Uint32);
void associateMem2HBA(void);
void enableRcvFis(void);
char spinUpDeviceAndWaitForInitToComplete(void);
void setSataSpeed(Uint8);
void enableDisableInt(Uint8,Uint8,Uint32);
void clearIntOrErrorDiag(Uint8,Uint32);
void clearError(Uint32);
void initMemory(Uint32 *,Uint32,Uint32,Uint32);
void clearCmdTableAndDmaBuff(void);
void clearRcvFis(void);
void invokeHBAReset(void);
void invokePortReset(void);
void buildCmdFis(CommandTable *);
char setupCfisEntriesForDataRdWr(CmdListHeader *,dataXferDir,xferProtocol);
void modifyCfisEntries4NCQ(CmdListHeader *,dataXferDir);
void getCmdFis(CommandTable *);
void getDmaSetupFis(ReceiveFis *);
void getRegDev2HostFis(ReceiveFis *);
void getSetDevBitsFis(ReceiveFis *);
void getUnkownFis(ReceiveFis *);
void initTimer1ms(Uint32);
void initCCCint(Uint16,Uint16);
void eanbleCCCint(void);
void disableCCCint(void);
void cfgDmaSetting(void);
char startCmdListProcessing(void);
char submitCmd(Uint8,Uint8);
void submitBatchNCQcmds(Uint32);
Uint32 getRegStatus(Uint32 *,Uint32);
char sataDoTest(void);
char chceckSysMemorySize(void);
char compMemContent(Uint32 *,Uint32 *,Uint32);
char setupCfisEntriesForSwReset(CmdListHeader *,Uint8,Uint8,Uint8);
char invokeSoftReset(void);


void testWriteDsFis(ReceiveFis *);
void testWritePsFis(ReceiveFis *);
void testWriteRegFis(ReceiveFis *);
void testWriteSdbFis(ReceiveFis *);
void testWriteUnknownFis(ReceiveFis *);
char setupCfisEntriesForSetFeature(CmdListHeader *);
void invokeIdentifyDevCmd(Uint16);
void invokeSetFeatureCmd(Uint16);

void clearCmdFis(void);
void clearCmdTable(Uint16);
void clearAllPrdMem(void);
void initBaseAddresses(void);
void performFirmwareInit(void);
void initPhyAndDmaPars(void);
void associateSysMem2Hba(Uint16);
void sata_intc_setup(void);
void sataIsr(void);
void disableSataClk(hostPwrMgmtConfig);
void enableDisableHBAlinkPwrMgmt(lowPwrState);
Uint8 reqPwrTransState(hostPwrMgmtConfig);
void cfgHBAlinkPwrTransState(hostPwrMgmtConfig);
void detHostPwrMgmtStateCfg(void);
void detPwrMgmtSupport(void);
void clearCmdList(void);
void clearIndividualCmdList(CmdListHeader *);
void clearCmdTables(void);
void clearIndividualCmdTable(Uint16);
void clearDmaBuffers(void);
char setupCfisEntriesForSetFeatureAll(CmdListHeader *,Uint8,Uint8);
void parseDeviceCapabilityNdetermineLowPowerSupport(Uint16 *);
void captureDeviceState(void);

void pllInit();

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


//extern interfaceSpeed intSpeed;

//extern CSL_PscRegsOvly pscRegs;
extern FirmwareCtrlFeatures swCtrlFeatures;
extern cmdFis myCmdFis;
extern dsFis myDsFis;
extern piosFis myPsFis;
extern regFis myRegFis;
extern sdbFis mySdbFis;
extern uFis myUnknownFis;
extern Uint32 Dev28bitLbaAddress;
extern Uint8 PioCmd;
extern Uint16 intIsrCnt, intIsrFlag;
extern interruptHandlingMethod intHandlingMethod;
extern pwrMgmtCapability pwrMgmtCap;
extern hostPwrMgmtConfig hostPwrMgmtCfg;
extern Uint32 p0sctlHighp0sstsLow, cmdListWrPrdCnt, cmdListRdPrdCnt;
extern Uint16 *pIdDevPwrData;
extern Uint16 *pIdDevData, IdDevData, sataCapabilities, sataFeaturesSupported, sataFeaturesEnabled;
extern sataCapability support4PwrMgmtInitiatedByDeviceAfterEnable, support4PwrMgmtInitiatedByDeviceEnabled;
extern sataCapability support4PwrMgmtInitiatedByDevice, support4PwrMgmtInitiatedByHost, support4NativeCommandQueuing;
extern hostPwrMgmtState hbaPwrState;
extern compareResult wrRdByteCntCompStatus;
extern interfaceSpeed lineSpeedNegotiated;
void resumeSataCntrlClk(void);
void exitHostPwrDwn(void);


extern unsigned char progStatus;
//extern CSL_SataRegs* sataRegs;


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PID;
    volatile Uint8 RSVD0[204];
    volatile Uint32 SHIFTDIV;
    volatile Uint32 CS0;
    volatile Uint32 DFTCNTR;
    volatile Uint32 DFTCNTRCTRL;
    volatile Uint32 FUSERR;
    volatile Uint32 RSTYPE;
    volatile Uint32 RSTCTRL;
    volatile Uint32 RSTCFG;
    volatile Uint32 RSISO;
    volatile Uint8 RSVD1[12];
    volatile Uint32 PLLCTL;
    volatile Uint32 OCSEL;
    volatile Uint32 SECCTL;
    volatile Uint8 RSVD2[4];
    volatile Uint32 PLLM;
    volatile Uint32 PREDIV;
    volatile Uint32 PLLDIV1;
    volatile Uint32 PLLDIV2;
    volatile Uint32 PLLDIV3;
    volatile Uint32 OSCDIV1;
    volatile Uint32 POSTDIV;
    volatile Uint32 BPDIV;
    volatile Uint32 WAKEUP;
    volatile Uint8 RSVD3[4];
    volatile Uint32 PLLCMD;
    volatile Uint32 PLLSTAT;
    volatile Uint32 ALNCTL;
    volatile Uint32 DCHANGE;
    volatile Uint32 CKEN;
    volatile Uint32 CKSTAT;
    volatile Uint32 SYSTAT;
    volatile Uint8 RSVD4[12];
    volatile Uint32 PLLDIV4;
    volatile Uint32 PLLDIV5;
    volatile Uint32 PLLDIV6;
    volatile Uint32 PLLDIV7;
    volatile Uint32 PLLDIV8;
    volatile Uint32 PLLDIV9;
    volatile Uint32 PLLDIV10;
    volatile Uint32 PLLDIV11;
    volatile Uint32 PLLDIV12;
    volatile Uint32 PLLDIV13;
    volatile Uint32 PLLDIV14;
    volatile Uint8 RSVD5[4];
    volatile Uint32 PLLDIV15;
    volatile Uint8 RSVD6[88];
    volatile Uint32 PLLHDIVEN;
    volatile Uint32 EMUCNT0;
    volatile Uint32 EMUCNT1;
} CSL_PllcRegs;

/**************************************************************************\
* Register Overlay Structure
\**************************************************************************/
typedef struct  {
    volatile Uint32 PID;
    volatile Uint8 RSVD0[12];
    volatile Uint32 GBLCTL;
    volatile Uint32 GBLSTAT;
    volatile Uint32 INTEVAL;
    volatile Uint32 IPWKCNT;
    volatile Uint8 RSVD1[32];
    volatile Uint32 MERRPR[2];
    volatile Uint8 RSVD2[8];
    volatile Uint32 MERRCR[2];
    volatile Uint8 RSVD3[8];
    volatile Uint32 PERRPR;
    volatile Uint8 RSVD4[4];
    volatile Uint32 PERRCR;
    volatile Uint8 RSVD5[4];
    volatile Uint32 EPCPR;
    volatile Uint8 RSVD6[4];
    volatile Uint32 EPCCR;
    volatile Uint8 RSVD7[132];
    volatile Uint32 RAILSTAT;
    volatile Uint32 RAILCTL;
    volatile Uint32 RAILSEL;
    volatile Uint8 RSVD8[20];
    volatile Uint32 PTCMD;
    volatile Uint8 RSVD9[4];
    volatile Uint32 PTSTAT;
    volatile Uint8 RSVD10[212];
    volatile Uint32 PDSTAT[32];
    volatile Uint8 RSVD11[128];
    volatile Uint32 PDCTL[32];
    volatile Uint8 RSVD12[128];
    volatile Uint32 PDCFG[32];
    volatile Uint8 RSVD13[384];
    volatile Uint32 MDCFG[32];
    volatile Uint8 RSVD14[384];
    volatile Uint32 MDSTAT[32];
    volatile Uint8 RSVD15[384];
    volatile Uint32 MDCTL[32];
} CSL_PscRegs;


typedef volatile CSL_PllcRegs				*CSL_PllcRegsOvly;
typedef volatile CSL_PscRegs				*CSL_PscRegsOvly;

#define CSL_PLLC_0_REGS						((CSL_PllcRegsOvly)   0x01C11000u)
#define CSL_PLLC_1_REGS						((CSL_PllcRegsOvly)   0x01E1A000u)

#define CSL_PSC_0_REGS						((CSL_PscRegsOvly)    0x01C10000u)
#define CSL_PSC_1_REGS						((CSL_PscRegsOvly)    0x01E27000u)


#define CSL_PSC_MDCTL_FORCE_MASK			(0x80000000u)
#define CSL_PSC_MDCTL_FORCE_SHIFT			(0x0000001Fu)
#define CSL_PSC_MDCTL_FORCE_RESETVAL		(0x00000000u)

//#define LPSC_SATA         PSC1_OFFSET+8        /*SATA LPSC*/
//#define PSC1_OFFSET  100

#define SATACLK_PWRDN  0x01E2C018

#define PLLC0_PD_DIS_CMD					0xFFFFFFCD
#define PLLC1_PD_DIS_CMD					0xFFFFFFCD
#define PLLC0_RST_CMD						0x00000008
#define PLLC1_RST_CMD						0x00000008
#define PLLC0_PLLM							0x00000015
#define PLLC1_PLLM							0x00000015
#define PLLC0_POSTDIV						0x00008001
#define PLLC1_POSTDIV						0x00008001
#define PLLC0_CMD							0x00000001
#define PLLC1_CMD							0x00000001
#define PLLLOCK_WAIT						0xFFFF	//jemiah
#define PLLC0_PLLEN							0x00000001
#define PLLC1_PLLEN							0x00000001
#define PLL1_PLLDIV1						0x00008000


#endif
