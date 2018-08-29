/*   ==========================================================================
 *   Copyright (c) Texas Instruments Inc , 2007
 *
 *   Use of this software is controlled by the terms and conditions found
 *   in the license agreement under which this software has been supplied
 *   provided
 *   ==========================================================================
*/
#ifndef _DEVICE_H
#define _DEVICE_H

/*****************************************************************************/
 /** \file device.h
 *
 * \brief This file contains the Chip Description for DEVICE (ARM and DSP side)
 *
 *****************************************************************************/

#include <cslr.h>
#include <tistdtypes.h>

#define CSL_IDEF_INLINE static
/*****************************************************************************\
* Include files for all the modules in the device
\*****************************************************************************/

#include "cslr_aemif_001.h"
#include "cslr_bootcfg_001.h"
#include "cslr_cpgmacssr_001.h"
#include "cslr_dmaxintc_001.h"
#include "cslr_dmaxpdsp_001.h"
#include "cslr_ecap_001.h"
#include "cslr_edmacc_001.h"
#include "cslr_edmatc_001.h"
#include "cslr_ehrpwm_001.h"
#include "cslr_emac_001.h"
#include "cslr_emif3a_001.h"
#include "cslr_gpio_001.h"
#include "cslr_i2c_001.h"
#include "cslr_intc_001.h"
#include "cslr_iopu0_001.h"
#include "cslr_iopu1_001.h"
#include "cslr_iopu2_001.h"
#include "cslr_iopu3_001.h"
#include "cslr_iopu4_001.h"
#include "cslr_iopu5_001.h"
#include "cslr_keymgr_001.h"
#include "cslr_lcdc_001.h"
#include "cslr_mcasp_001.h"
#include "cslr_mcbsp_001.h"
#include "cslr_mdio_001.h"
#include "cslr_mmcsd_001.h"
#include "cslr_mpu0_001.h"
#include "cslr_mpu1_001.h"
#include "cslr_mpu2_001.h"
#include "cslr_pbist_001.h"
#include "cslr_pbistc_001.h"
#include "cslr_pllc_001.h"
#include "cslr_psc_001.h"
#include "cslr_rtc_001.h"
#include "cslr_sata_001.h"
#include "cslr_sc_001.h"
#include "cslr_spi_001.h"
#include "cslr_syscfg_001.h"
#include "cslr_tmr_001.h"
#include "cslr_uart_001.h"
#include "cslr_uhpi_001.h"
#include "cslr_upp_001.h"
#include "cslr_usb_ohci_001.h"
#include "cslr_usb_otg_001.h"
#include "cslr_vpif_001.h"

#include "cslr_debuginfo_001.h"

/*****************************************************************************\
* Peripheral Instance counts
\*****************************************************************************/
#define CSL_AEMIF_PER_CNT                1
#define CSL_CPGMACSSR_PER_CNT            1
#define CSL_DMAXINTC_PER_CNT             1
#define CSL_DMAXPDSP_PER_CNT             2
#define CSL_ECAP_PER_CNT                 3
#define CSL_EDMA_CC_CNT                  2
#define CSL_EDMA_TC_CNT                  3 
#define CSL_EMAC_PER_CNT                 1
#define CSL_EMIF3A_PER_CNT               1
#define CSL_EPWM_PER_CNT                 2
#define CSL_GPIO_PER_CNT                 1
#define CSL_HRPWM_PER_CNT                2
#define CSL_I2C_PER_CNT                  2
#define CSL_INTC_PER_CNT                 1
#define CSL_IOPU_PER_CNT                 6
#define CSL_KEYMGR_PER_CNT               1
#define CSL_LCDC_PER_CNT                 1
#define CSL_MCASP_PER_CNT                1
#define CSL_MCBSP_PER_CNT                2
#define CSL_MDIO_PER_CNT                 1
#define CSL_MMCSD_PER_CNT                2
#define CSL_MPU_PER_CNT                  3
#define CSL_PBISTC_PER_CNT               1
#define CSL_PBIST_PER_CNT                1
#define CSL_PLLC_PER_CNT                 2
#define CSL_PSC_PER_CNT                  2
#define CSL_RTC_PER_CNT                  1
#define CSL_SATA_PER_CNT                 1
#define CSL_SC_PER_CNT                   1
#define CSL_SPI_PER_CNT                  2
#define CSL_TMR_PER_CNT                  4
#define CSL_UART_PER_CNT                 3
#define CSL_UHPI_PER_CNT                 1
#define CSL_UPP_PER_CNT                  1
#define CSL_USB_PER_CNT                  2
#define CSL_VPIF_PER_CNT                 1


/*****************************************************************************\
* Peripheral Overlay Structures
\*****************************************************************************/
typedef volatile CSL_AdataRegs           *CSL_AdataRegsOvly;
typedef volatile CSL_AemifRegs           *CSL_AemifRegsOvly;
typedef volatile CSL_AfifoRegs           *CSL_AfifoRegsOvly;
typedef volatile CSL_BdataRegs           *CSL_BdataRegsOvly;
typedef volatile CSL_BfifoRegs           *CSL_BfifoRegsOvly;
typedef volatile CSL_BootcfgRegs         *CSL_BootcfgRegsOvly;
typedef volatile CSL_CpgmacssrRegs       *CSL_CpgmacssrRegsOvly;
typedef volatile CSL_DmaxintcRegs        *CSL_DmaxintcRegsOvly;
typedef volatile CSL_DmaxpdspRegs        *CSL_DmaxpdspRegsOvly;
typedef volatile CSL_EcapRegs            *CSL_EcapRegsOvly;
typedef volatile CSL_EdmaccRegs          *CSL_EdmaccRegsOvly;
typedef volatile CSL_EdmaccShadowRegs    *CSL_EdmaccShadowRegsOvly;
typedef volatile CSL_EdmatcRegs          *CSL_EdmatcRegsOvly;
typedef volatile CSL_EhrpRegs            *CSL_EhrpRegsOvly;
typedef volatile CSL_EmacRegs            *CSL_EmacRegsOvly;
typedef volatile CSL_Emif3aRegs          *CSL_Emif3aRegsOvly;
typedef volatile CSL_EpwmRegs            *CSL_EpwmRegsOvly;
typedef volatile CSL_GpioRegs            *CSL_GpioRegsOvly;
typedef volatile CSL_I2cRegs             *CSL_I2cRegsOvly;
typedef volatile CSL_IntcRegs            *CSL_IntcRegsOvly;
typedef volatile CSL_Iopu0Regs           *CSL_Iopu0RegsOvly; 
typedef volatile CSL_Iopu1Regs           *CSL_Iopu1RegsOvly; 
typedef volatile CSL_Iopu2Regs           *CSL_Iopu2RegsOvly; 
typedef volatile CSL_Iopu3Regs           *CSL_Iopu3RegsOvly; 
typedef volatile CSL_Iopu4Regs           *CSL_Iopu4RegsOvly; 
typedef volatile CSL_Iopu5Regs           *CSL_Iopu5RegsOvly; 
typedef volatile CSL_KeymgrRegs          *CSL_KeymgrRegsOvly;
typedef volatile CSL_LcdcRegs            *CSL_LcdcRegsOvly;
typedef volatile CSL_McaspRegs           *CSL_McaspRegsOvly;
typedef volatile CSL_McbspRegs           *CSL_McbspRegsOvly;
typedef volatile CSL_MdioRegs            *CSL_MdioRegsOvly;
typedef volatile CSL_MmcsdRegs           *CSL_MmcsdRegsOvly;
typedef volatile CSL_Mpu0Regs            *CSL_Mpu0RegsOvly;
typedef volatile CSL_Mpu1Regs            *CSL_Mpu1RegsOvly;
typedef volatile CSL_Mpu2Regs            *CSL_Mpu2RegsOvly;
typedef volatile CSL_OhciRegs            *CSL_OhciRegsOvly;
typedef volatile CSL_PbistCRegs          *CSL_PbistCRegsOvly;
typedef volatile CSL_PbistRegs           *CSL_PbistRegsOvly;
typedef volatile CSL_PllcRegs            *CSL_PllcRegsOvly;
typedef volatile CSL_PscRegs             *CSL_PscRegsOvly;
typedef volatile CSL_RtcRegs             *CSL_RtcRegsOvly;
typedef volatile CSL_SataRegs            *CSL_SataRegsOvly;
typedef volatile CSL_ScRegs              *CSL_ScRegsOvly;
typedef volatile CSL_SpiRegs             *CSL_SpiRegsOvly;
/*typedef volatile CSL_SyscfgRegs          *CSL_SyscfgRegsOvly;*/
typedef volatile CSL_TmrRegs             *CSL_TmrRegsOvly;
typedef volatile CSL_UartRegs            *CSL_UartRegsOvly;
typedef volatile CSL_UdftRegs            *CSL_UdftRegsOvly;
typedef volatile CSL_UhpiRegs            *CSL_UhpiRegsOvly;
typedef volatile CSL_UppRegs             *CSL_UppRegsOvly;
typedef volatile CSL_Usb20Regs           *CSL_Usb20RegsOvly;
typedef volatile CSL_VpifRegs            *CSL_VpifRegsOvly;

/*****************************************************************************\
* Peripheral Base Address
\*****************************************************************************/

#define CSL_PLLC_0_REGS                  ((CSL_PllcRegsOvly)   0x01C11000u)
#define CSL_PLLC_1_REGS                  ((CSL_PllcRegsOvly)   0x01E1A000u)

//GEM Interrupt Controller
#if defined(_TMS320C6X)
#define CSL_INTC_REGS                    ((CSL_IntcRegsOvly)   0x1800000u)
#endif

//ARM Interrupt Controller 
#if defined(__TMS470__) 
#define CSL_INTC_REGS                    ((CSL_IntcRegsOvly)   0xFFFEE000u)
#endif 

#define CSL_ADATA_0_REGS                 ((CSL_AdataRegsOvly)  0x01D02000u)
#define CSL_AEMIF_0_REGS                 ((CSL_AemifRegsOvly)  0x68000000u)
#define CSL_AFIFO_0_REGS                 ((CSL_AfifoRegsOvly)  0x01D01000u)
#define CSL_BDATA_0_REGS                 ((CSL_BdataRegsOvly)  0x01F10000u)
#define CSL_BDATA_1_REGS                 ((CSL_BdataRegsOvly)  0x01F11000u)
#define CSL_BFIFO_0_REGS                 ((CSL_BfifoRegsOvly)  0x01D10800u)
#define CSL_BFIFO_1_REGS                 ((CSL_BfifoRegsOvly)  0x01D11800u)
#define CSL_BOOTCFG_0_REGS               ((CSL_BootcfgRegsOvly)0x01C14000u)
#define CSL_CPGMACSSR_0_REGS             ((CSL_CpgmacssrRegsOvly) 0x01E22000u)
#define CSL_DMAXINTC_0_REGS              ((CSL_DmaxintcRegsOvly) 0x01C34000u)
#define CSL_DMAXPDSP0_IRAM_BASEADDR             (0x01C38000u)
#define CSL_DMAXPDSP1_IRAM_BASEADDR             (0x01C3C000u)
#define CSL_DMAXPDSP_0_REGS              ((CSL_DmaxpdspRegsOvly) 0x01C37000u)
#define CSL_DMAXPDSP_1_REGS              ((CSL_DmaxpdspRegsOvly) 0x01C37800u)
#define CSL_DMAX_DRAM0_BASEADDR                 (0x01C30000u)
#define CSL_DMAX_DRAM1_BASEADDR                 (0x01C32000u)
#define CSL_ECAP_0_REGS                  ((CSL_EcapRegsOvly)   0x01F06000u)
#define CSL_ECAP_1_REGS                  ((CSL_EcapRegsOvly)   0x01F07000u)
#define CSL_ECAP_2_REGS                  ((CSL_EcapRegsOvly)   0x01F08000u)
#define CSL_EDMACC_0_REGS                ((CSL_EdmaccRegsOvly) 0x01C00000u)
#define CSL_EDMACC_1_REGS                ((CSL_EdmaccRegsOvly) 0x01E30000u)
#define CSL_EDMATC_0_REGS                ((CSL_EdmatcRegsOvly) 0x01C08000u)
#define CSL_EDMATC_1_REGS                ((CSL_EdmatcRegsOvly) 0x01C08400u)
#define CSL_EDMATC_2_REGS                ((CSL_EdmatcRegsOvly) 0x01E38000u)
#define CSL_EHRP_0_REGS                  ((CSL_EhrpRegsOvly)   0x01F01000u)
#define CSL_EHRP_1_REGS                  ((CSL_EhrpRegsOvly)   0x01F03000u)
#define CSL_EMAC_0_REGS                  ((CSL_EmacRegsOvly)   0x01E23000u)
#define CSL_EMIF3A_0_REGS                ((CSL_Emif3aRegsOvly) 0xB0000000u)
#define CSL_EPWM_0_REGS                  ((CSL_EpwmRegsOvly)   0x01F00000u)
#define CSL_EPWM_1_REGS                  ((CSL_EpwmRegsOvly)   0x01F02000u)
#define CSL_GPIO_REGS                    ((CSL_GpioRegsOvly)   0x01E26000u)
#define CSL_I2C_0_REGS                   ((CSL_I2cRegsOvly)    0x01C22000u)
#define CSL_I2C_1_REGS                   ((CSL_I2cRegsOvly)    0x01E28000u)
#define CSL_IOPU_0_REGS                  ((CSL_Iopu0RegsOvly)  0x01C16000u)
#define CSL_IOPU_1_REGS                  ((CSL_Iopu1RegsOvly)  0x01C24000u)
#define CSL_IOPU_2_REGS                  ((CSL_Iopu2RegsOvly)  0x01C17000u)
#define CSL_IOPU_3_REGS                  ((CSL_Iopu3RegsOvly)  0x01E29000u) 
#define CSL_IOPU_4_REGS                  ((CSL_Iopu4RegsOvly)  0x01D0E000u) 
#define CSL_IOPU_5_REGS                  ((CSL_Iopu5RegsOvly)  0x01F0B000u) 
#define CSL_KEYMGR_0_REGS                ((CSL_KeymgrRegsOvly) 0x01C12000u)
#define CSL_LCDC_0_REGS                  ((CSL_LcdcRegsOvly)   0x01E13000u)
#define CSL_MCASP_0_REGS                 ((CSL_McaspRegsOvly)  0x01D00000u)
#define CSL_MCBSP_0_REGS                 ((CSL_McbspRegsOvly)  0x01D10000u)
#define CSL_MCBSP_1_REGS                 ((CSL_McbspRegsOvly)  0x01D11000u)
#define CSL_MDIO_0_REGS                  ((CSL_MdioRegsOvly)   0x01E24000u)
#define CSL_MMCSD_0_REGS                 ((CSL_MmcsdRegsOvly)  0x01C40000u)
#define CSL_MMCSD_1_REGS                 ((CSL_MmcsdRegsOvly)  0x01E1B000u)
#define CSL_MPU_0_REGS                   ((CSL_Mpu0RegsOvly)   0x01C43000u)
#define CSL_MPU_1_REGS                   ((CSL_Mpu1RegsOvly)   0x01E14000u)
#define CSL_MPU_2_REGS                   ((CSL_Mpu2RegsOvly)   0x01E15000u)
#define CSL_PBISTC_0_REGS                ((CSL_PbistCRegsOvly) 0x01E2B000u)
#define CSL_PBIST_0_REGS                 ((CSL_PbistRegsOvly)  0x01E2A000u)
#define CSL_PBIST_1_REGS                 ((CSL_PbistRegsOvly)  0x01BD0000u)
#define CSL_PSC_0_REGS                   ((CSL_PscRegsOvly)    0x01C10000u)
#define CSL_PSC_1_REGS                   ((CSL_PscRegsOvly)    0x01E27000u)
#define CSL_RTC_0_REGS                   ((CSL_RtcRegsOvly)    0x01C23000u)
#define CSL_SATA_0_REGS                  ((CSL_SataRegsOvly)   0x01E18000u)
#define CSL_SC_0_REGS                    ((CSL_ScRegsOvly)     0x01C13000u) 
#define CSL_SPI_0_REGS                   ((CSL_SpiRegsOvly)    0x01C41000u)
#define CSL_SPI_1_REGS                   ((CSL_SpiRegsOvly)    0x01F0E000u)
#define CSL_SYSCFG_0_REGS                ((CSL_SyscfgRegsOvly) 0x01E2C000u)
#define CSL_TMR_0_REGS                   ((CSL_TmrRegsOvly)    0x01C20000u)
#define CSL_TMR_1_REGS                   ((CSL_TmrRegsOvly)    0x01C21000u)
#define CSL_TMR_2_REGS                   ((CSL_TmrRegsOvly)    0x01F0C000u)
#define CSL_TMR_3_REGS                   ((CSL_TmrRegsOvly)    0x01F0D000u)
#define CSL_UART_0_REGS                  ((CSL_UartRegsOvly)   0x01C42000u)
#define CSL_UART_1_REGS                  ((CSL_UartRegsOvly)   0x01D0C000u)
#define CSL_UART_2_REGS                  ((CSL_UartRegsOvly)   0x01D0D000u)
#define CSL_UDFT_0_REGS                  ((CSL_UdftRegsOvly)   0x01E11000u)
#define CSL_UHPI_0_REGS                  ((CSL_UhpiRegsOvly)   0x01E10000u)
#define CSL_UPP_0_REGS                   ((CSL_UppRegsOvly)    0x01E16000u)
#define CSL_USB11_0_REGS                 ((CSL_OhciRegsOvly)   0x01E25000u)
#define CSL_USB20_0_REGS                 ((CSL_Usb20RegsOvly)  0x01E00000u)
#define CSL_VPIF_0_REGS                  ((CSL_VpifRegsOvly)   0x01E17000u)

/*
#define CSL_USBOTG_REGS                  ((CSL_OtgRegsOvly)  0x01E00000u)
*/


/** \brief  UART Module Instances
*
*/
typedef enum {
  CSL_UART_ANY   = -1, /**< <b>: Any instance of UART module</b> */
  CSL_UART_0     =  0, /**< <b>: UART Instance 0</b> */
  CSL_UART_1     =  1, /**< <b>: UART Instance 1</b> */
  CSL_UART_2     =  2  /**< <b>: UART Instance 2</b> */
} CSL_UartNum;

/** \brief I2C Module Instances
*
*/
typedef enum {
  CSL_I2C_ANY    = -1, /**< <b>: Any instance of I2C module</b> */
  CSL_I2C_0      =  0, /**< <b>: I2C Instance 0</b> */
  CSL_I2C_1      =  1  /**< <b>: I2C Instance 1</b> */
} CSL_I2cNum;

/** \brief PLLC Module Instances
*
*/
typedef enum {
  CSL_PLLC_ANY   = -1, /**< <b>: Any instance of PLLC module</b> */
  CSL_PLLC_0     =  0, /**< <b>: PLLC Instance 0</b> */
  CSL_PLLC_1     =  1  /**< <b>: PLLC Instance 1</b> */
} CSL_PllcNum;


/** \brief INTC Module Instances
*
*/
typedef enum {
  CSL_INTC_ANY   = -1, /**< <b>: Any instance of INTC module</b> */
  CSL_INTC_0     =  0  /**< <b>: INTC Instance 0</b> */
} CSL_IntcNum;

/** \brief  SPI Module Instances
*
*/
typedef enum {
  CSL_SPI_ANY    = -1, /**< <b>: Any instance of SPI module</b> */
  CSL_SPI_0      =  0, /**< <b>: SPI Instance 0</b> */
  CSL_SPI_1      =  1  /**< <b>: SPI Instance 1</b> */

} CSL_SpiNum;

/** \brief GPIO Module Instances
*
*/
typedef enum {
  CSL_GPIO_ANY   = -1, /**< <b>: Any instance of GPIO module</b> */
  CSL_GPIO       =  0  /**< <b>: GPIO Instance 0</b> */
} CSL_GpioNum;

/** \brief EMIF Module Instances
*
*/
typedef enum {
  CSL_AEMIF_ANY  = -1, /**< <b>: Any instance of AEMIF module</b> */
  CSL_AEMIF_0    =  0  /**< <b>: AEMIF Instance 0</b> */
} CSL_AemifNum;

/** \brief  RTC Module Instances
*
*/
typedef enum {
  CSL_RTC_ANY    = -1, /**< <b>: Any instance of RTC module</b> */
  CSL_RTC_0      =  0  /**< <b>: RTC Instance 0</b> */
} CSL_RtcNum;

/** \brief  MCASP Module Instances
*
*/
typedef enum {
  CSL_MCASP_ANY  = -1, /**< <b>: Any instance of MCASP module</b> */
  CSL_MCASP_0    =  0  /**< <b>: MCASP Instance 0</b> */
} CSL_McaspNum;

/** \brief  MCBSP Module Instances
*
*/
typedef enum {
  CSL_MCBSP_ANY  = -1, /**< <b>: Any instance of MCBSP module</b> */
  CSL_MCBSP_0    =  0, /**< <b>: MCBSP Instance 0</b> */
  CSL_MCBSP_1    =  1  /**< <b>: MCBSP Instance 1</b> */
} CSL_McbspNum;

/** \brief  MMC/SD Module Instances
*
*/
typedef enum {
  CSL_MMCSD_ANY  = -1, /**< <b>: Any instance of MMCSD module</b> */
  CSL_MMCSD_0    =  0, /**< <b>: MMCSD Instance 0</b> */
  CSL_MMCSD_1    =  1  /**< <b>: MMCSD Instance 1</b> */
} CSL_MmcsdNum;

/** \brief LCDC Module Instances
*
*/
typedef enum {
  CSL_LCDC_ANY   = -1, /**< <b>: Any instance of LCD module</b> */
  CSL_LCDC_0     =  0  /**< <b>: LCD Instance 0</b> */
} CSL_LcdcNum;

/** \brief DDR Module Instances
*
*/
typedef enum {
  CSL_EMIF_ANY   = -1, /**< <b>: Any instance of EMIF3C module</b> */
  CSL_EMIF_0     =  0  /**< <b>: EMIF3C Instance 0</b> */
} CSL_EmifNum;

/** \brief Key manager Module Instances
*
*/
typedef enum {
  CSL_KEYMGR_ANY = -1, /**< <b>: Any instance of KM module</b> */
  CSL_KEYMGR_0   =  0  /**< <b>: KM Instance 0</b> */
} CSL_KeymgrNum;

/** \brief bootcfg Module Instances
*
*/
typedef enum {
  CSL_BOOTCFG_ANY= -1, /**< <b>: Any instance of BOOTCONFIG module</b> */
  CSL_BOOTCFG_0  =  0  /**< <b>: BOOTCONFIG Instance 0</b> */
} CSL_BootcfgNum;

/** \brief syscfg Module Instances
*
*/
typedef enum {
  CSL_SYSCFG_ANY = -1, /**< <b>: Any instance of SYSCFG module</b> */
  CSL_SYSCFG_0   =  0  /**< <b>: SYSCFG Instance 0</b> */
} CSL_SyscfgNum;

/** \brief TMR Module Instances
*
*/
typedef enum {
  CSL_TMR_ANY    = -1, /**< <b>: Any instance of TMR module</b> */
  CSL_TMR_0      =  0, /**< <b>: TMR Instance 0</b> */
  CSL_TMR_1      =  1, /**< <b>: TMR Instance 1</b> */
  CSL_TMR_2      =  2, /**< <b>: TMR Instance 2</b> */
  CSL_TMR_3      =  3  /**< <b>: TMR Instance 3</b> */
} CSL_TmrNum;

/** \brief UHPI Module Instances
*
*/
typedef enum {
  CSL_UHPI_ANY   = -1, /**< <b>: Any instance of UHPI module</b> */
  CSL_UHPI_0     =  0  /**< <b>: UHPI Instance 0</b> */
} CSL_UhpiNum;

/** \brief USB Module Instances
*
*/
typedef enum {
  CSL_USB_ANY    = -1, /**< <b>: Any instance of USB module</b> */
  CSL_USB_0      =  0, /**< <b>: USB Instance 0</b> */
  CSL_USB_1      =  1  /**< <b>: USB Instance 1</b> */
} CSL_UsbNum;

/** \brief HRPWM Module Instances
*
*/
typedef enum {
  CSL_EHR_ANY    = -1, /**< <b>: Any instance of HRPWMr module</b> */
  CSL_EHRP_0     =  0, /**< <b>: HRPWM Instance 0</b> */
  CSL_EHRP_1     =  1  /**< <b>: HRPWM Instance 1</b> */
} CSL_EhrpNum;

/** \brief EPWM Module Instances
*
*/
typedef enum {
  CSL_EPWM_ANY   = -1, /**< <b>: Any instance of EPWM module</b> */
  CSL_EPWM_0     =  0, /**< <b>: EPWM Instance 0</b> */
  CSL_EPWM_1     =  1  /**< <b>: EPWM Instance 1</b> */
} CSL_EpwmNum;

/** \brief PSC Module Instances
*
*/
typedef enum {
  CSL_PSC_ANY    = -1, /**< <b>: Any instance of PSC module</b> */
  CSL_PSC_0      =  0, /**< <b>: PSC Instance 0</b> */
  CSL_PSC_1      =  1
} CSL_PscNum;

/** \brief EMAC Module Instances
*
*/
typedef enum {
  CSL_EMAC_ANY   = -1, /**< <b>: Any instance of EMAC module</b> */
  CSL_EMAC_0     =  0  /**< <b>: EMAC Instance 0</b> */
} CSL_EmacNum;

/** \brief EWRAP Module Instances
*
*/
typedef enum {
  CSL_CPGMACSSR_ANY    = -1, /**< <b>: Any instance of EWRAP module</b> */
  CSL_CPGMACSSR_0      =  0  /**< <b>: EWRAP Instance 0</b> */
} CSL_CpgmacssrNum;

/** \brief MDIO Module Instances
*
*/
typedef enum {
  CSL_MDIO_ANY   = -1, /**< <b>: Any instance of MDIO module</b> */
  CSL_MDIO_0     =  0  /**< <b>: MDIO Instance 0</b> */
} CSL_MdioNum;

/** \brief Security Controller (SC) Module Instances
*
*/
typedef enum {
  CSL_SC_ANY     = -1, /**< <b>: Any instance of SC module</b> */
  CSL_SC_0       =  0  /**< <b>: SC Instance 0</b> */
} CSL_ScNum;

/** \brief ECAP Module Instances
*
*/
typedef enum {
  CSL_ECAP_ANY   = -1, /**< <b>: Any instance of ECAP module</b> */
  CSL_ECAP_0     =  0, /**< <b>: ECAP Instance 0</b> */
  CSL_ECAP_1     =  1, /**< <b>: ECAP Instance 1</b> */
  CSL_ECAP_2     =  2  /**< <b>: ECAP Instance 2</b> */
} CSL_EcapNum;

/** \brief DMAX Module Instances
*
*/
typedef enum {
  CSL_DMAXPDSP_ANY= -1, /**< <b>: Any instance of DMAX module</b> */
  CSL_DAMXPDSP_0  =  0, /**< <b>: DMAX Instance 0</b> */
  CSL_DAMXPDSP_1  =  1  /**< <b>: DMAX Instance 1</b> */
} CSL_DmaxNum;

/** \brief IOPU Module Instances
*
*/
typedef enum {
  CSL_IOPU_ANY   = -1, /**< <b>: Any instance of IOPU module</b> */
  CSL_IOPU_0     =  0, /**< <b>: IOPU Instance 0</b> */
  CSL_IOPU_1     =  1, /**< <b>: IOPU Instance 1</b> */
  CSL_IOPU_2     =  2, /**< <b>: IOPU Instance 2</b> */
  CSL_IOPU_3     =  3, /**< <b>: IOPU Instance 3</b> */
  CSL_IOPU_4     =  4, /**< <b>: IOPU Instance 4</b> */
  CSL_IOPU_5     =  5  /**< <b>: IOPU Instance 5</b> */
} CSL_IopuNum;

/** \brief MPU Module Instances
*
*/
typedef enum {
  CSL_MPU_ANY    = -1, /**< <b>: Any instance of MPU module</b> */
  CSL_MPU_0      =  0, /**< <b>: MPU Instance 0</b> */
  CSL_MPU_1      =  1, /**< <b>: MPU Instance 1</b> */
  CSL_MPU_2      =  2  /**< <b>: MPU Instance 2</b> */
} CSL_MpuNum;

/** \brief EDMACC Module Instances
*
*/
typedef enum {
  CSL_EDMACC_ANY = -1, /**< <b>: Any instance of EDMACC module</b> */
  CSL_EDMACC_0   =  0, /**< <b>: EDMACC Instance 0</b> */
  CSL_EDMACC_1   =  1  /**< <b>: EDMACC Instance 1</b> */
} CSL_EdmaccNum;

/** \brief EDMATC Module Instances
*
*/
typedef enum {
  CSL_EDMATC_ANY = -1, /**< <b>: Any instance of EDMATC module</b> */
  CSL_EDMATC_0   =  0, /**< <b>: EDMATC Instance 0</b> */
  CSL_EDMATC_1   =  1, /**< <b>: EDMATC Instance 1</b> */
  CSL_EDMATC_2   =  2  /**< <b>: EDMATC Instance 2</b> */
} CSL_EdmatcNum;

/** \brief RPI Module Instances
*
*/
typedef enum {
  CSL_UPP_ANY    = -1, /**< <b>: Any instance of UPP module</b> */
  CSL_UPP_0      =  0  /**< <b>: UPP Instance 0</b> */
} CSL_UppNum;

/** \brief VPIF Module Instances
*
*/
typedef enum {
  CSL_VPIF_ANY   = -1, /**< <b>: Any instance of VPIF module</b> */
  CSL_VPIF_0     =  0  /**< <b>: VPIF Instance 0</b> */
} CSL_VpifNum;

/** \brief SATA Module Instances
*
*/
typedef enum {
  CSL_SATA_ANY   = -1, /**< <b>: Any instance of SATA module</b> */
  CSL_SATA_0     =  0  /**< <b>: SATA Instance 0</b> */
} CSL_SataNum;

/*These lookups to get module instances*/
extern void * _CSL_edmacclookup[];
extern void * _CSL_edmatclookup[];
extern void * _CSL_psclookup[];
extern void * _CSL_pllclookup[];
extern void * _CSL_keymgrlookup[];
extern void * _CSL_sclookup[];
extern void * _CSL_iopulookup[];
extern void * _CSL_tmrlookup[];
extern void * _CSL_i2clookup[];
extern void * _CSL_dmaxpdsplookup[];
extern void * _CSL_mmcsdlookup[];
extern void * _CSL_spilookup[];
extern void * _CSL_uartlookup[];
extern void * _CSL_mpulookup[];
extern void * _CSL_mcasplookup[];
extern void * _CSL_mcbsplookup[];
extern void * _CSL_usb11lookup[];
extern void * _CSL_usb20lookup[];
extern void * _CSL_uhpilookup[];
extern void * _CSL_lcdclookup[];
extern void * _CSL_intclookup[];
extern void * _CSL_cppilookup[];
extern void * _CSL_emaclookup[];
extern void * _CSL_cpgmacssrlookup[];
extern void * _CSL_mdiolookup[];
extern void * _CSL_gpiolookup[];
extern void * _CSL_ehrplookup[];
extern void * _CSL_epwmlookup[];
extern void * _CSL_ecaplookup[];
extern void * _CSL_aemiflookup[];
extern void * _CSL_emiflookup[];
extern void * _CSL_rtclookup[];
extern void * _CSL_bootcfglookup[];
extern void * _CSL_syscfglookup[];
extern void * _CSL_upplookup[];
extern void * _CSL_vpiflookup[];
extern void * _CSL_satalookup[];

#endif

/* Rev.No.   Date/Time               ECN No.          Modifier      */
/* -------   ---------               -------          --------      */
/* 1         2  May  2007 13:32:22   0                Vishwanath    */
/* 2         17 July 2008 14:53:22   0                C. Lam        */
/********************************************************************/
                                                                                 
