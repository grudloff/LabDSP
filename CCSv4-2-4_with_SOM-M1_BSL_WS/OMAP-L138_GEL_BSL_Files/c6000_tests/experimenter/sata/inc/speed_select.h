//***************************************************************************
//  Copyright(c) 2000-2008, Texas Instruments Incorporated. All Rights Reserved
//
//  FILE:   speed_select.h Test program for SATA Module
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
#ifndef _SATA_SPEED_SELECT_H
#define	_SATA_SPEED_SELECT_H

/////////////////////////////////////////////////////////////////////////
// Used to Configure PHY and Speed
// PHY Configuration for Silicon is Different than for QT. SILICON PHY
//  requires more careful programming based on Board Design and need
//  to use values that is good for Internal, Intermediate, or Exterme
//  Envirnoment. The _SILICON_ #define differentiates between Internal
//  and QT timing. For Slicon keep the #define. For QT, Comment it.
/////////////////////////////////////////////////////////////////////////
// Comment and UnComment the below based on your Target.
#define _SILICON_	// You need to comment this #define _SILICON_ when running test on QT.


/////////////////////////////////////////////////////////////////////////
// NO SPEED NEGOTIATION RESTRICTION implies for the Host to go as fast
//  as Device can go, i.e, operate at a max speed with GEN2 being the
//  fastest the SYNOPSYS controller can handle.
// QT SpeedBridge support on GEN1 speed. Most HDDs support both GEN1 and
//  GEN2 speed and for QT and GEN2 speed operation on Silicon, no need
//  of specifying the speed. However, for GEN1 speed settings, need
//  to force the speed.
/////////////////////////////////////////////////////////////////////////
#if _NO_SPEED_NEGOTIATION_RESTRICTION_
		#define DESIRED_SPEED	GOASFASTASDEVICE	// Go As fast as the Device Can Go.
#elif _GEN1_
		#define DESIRED_SPEED	GEN1	// Go As fast as GEN1
#elif _GEN2_
		#define DESIRED_SPEED	GEN2	// Go As fast as GEN2
#else
		#define DESIRED_SPEED	GOASFASTASDEVICE	// Go As fast as the Device Can Go.
#endif

/////////////////////////////////////////////////////////////////////////
// QT SpeedBridge does not support Power Down mode. So, the below applies
//  only to Silicon. Note also that Power Down mode invoked by Device
//  is found to be not working on Freon. So, the Power Down Entry
//  mechanism used is only Host invoked. The below #define selects
//  whether Partial or Slumber Power Down mode is invoked when needed.
/////////////////////////////////////////////////////////////////////////
#ifndef _SLUMBER_
	#define desiredPwrDwnMode hostCfgPartialState
#else
	#define desiredPwrDwnMode hostCfgSlumberState
#endif

#endif
