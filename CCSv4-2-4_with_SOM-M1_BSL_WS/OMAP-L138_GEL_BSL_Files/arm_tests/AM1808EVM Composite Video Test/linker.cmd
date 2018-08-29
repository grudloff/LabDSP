/*****************************************************************************
* linker command file for AM-1808 test code.
*
* © Copyright 2010, Logic Product Development, Inc. All Rights Reserved.
******************************************************************************/

-stack           0x00000800
-heap            0x00000800

MEMORY
{
   shared_ram:      ORIGIN = 0x80000000  LENGTH = 0x00020000
   external_ram:    ORIGIN = 0xC0000000  LENGTH = 0x08000000
   arm_local_ram:   ORIGIN = 0xFFFF0000  LENGTH = 0x00002000
}

SECTIONS
{
   .text       > shared_ram
   .const      > shared_ram
   .bss        > shared_ram
   .far        > shared_ram
   .switch     > shared_ram
   .stack      > shared_ram
   .data       > shared_ram
   .cinit      > shared_ram
   .sysmem     > shared_ram
   .cio        > shared_ram
}

