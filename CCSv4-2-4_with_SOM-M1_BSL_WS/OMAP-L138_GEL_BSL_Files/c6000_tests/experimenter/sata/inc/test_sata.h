//-----------------------------------------------------------------------------
// \file    test_usb.h
// \brief   .
//
//-----------------------------------------------------------------------------

#include "types.h"
#include "evmomapl138_timer.h"

#define SPINUP_TEST_RETRY_DELAY		DELAY_HALF_SEC
#define SPINUP_TEST_RETRY_COUNT		20

#define DETECT_TEST_RETRY_DELAY		DELAY_HALF_SEC
#define DETECT_TEST_RETRY_COUNT		20

//-----------------------------------------------------------------------------
// Public Function Prototypes
//-----------------------------------------------------------------------------
uint32_t SATA_init(void);
uint32_t TEST_sata(void);
uint32_t sata_DoTest(void);

