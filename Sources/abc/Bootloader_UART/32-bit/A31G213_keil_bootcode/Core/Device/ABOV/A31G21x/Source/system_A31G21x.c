/**************************************************************************//**
 * @file     system_A31G21x.c
 * @brief    CMSIS Cortex-M0+ Device Peripheral Access Layer Source File for
 *           Device A31G21x
 * @version  V1.0
 * @date     23. JAN. 2017
 *
 * @note
 *
 ******************************************************************************/
/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include "A31G21x.h"

/*----------------------------------------------------------------------------
  DEFINES
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock;     /*!< System Clock Frequency (Core Clock)  HLCK */
uint32_t SystemPeriClock;     /*!< System Clock Frequency (Peri Clock)  PCLK */
/*----------------------------------------------------------------------------
  Clock functions
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
  Clock functions
  This function is used to update SystemCoreClock and SystemPeriClock
  and should be executed whenever the clock is changed.
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)     /* Get Core Clock Frequency      */
{
	SystemCoreClock=32000000; //1Mhz
	SystemPeriClock=32000000; //1Mhz
}

/**
 * Initialize the system
 *
 * @param  none
 * @return none
 *
 * @brief  Setup the microcontroller system.
 *         Initialize the System.
 */
void SystemInit (void)
{
///* ToDo: add code to initialize the system
//         do not use global variables because this function is called before
//         reaching pre-main. RW section maybe overwritten afterwards.          */
//
	WDT->CR = 0 // disable WDT ;default ON so you must turn off
		|(0x5A69<<16)
		|(0x25<<10)
		|(0x1A<<4)
		;

	SystemCoreClock=500000; //500khz
	SystemPeriClock=500000; //500khz
//
// flash memory controller
	FMC->MR = 0x81;       // after changing 0x81 -> 0x28 in MR reg, flash access timing will be able to be set.
	FMC->MR = 0x28;       // enter flash access timing changing mode
	FMC->CFG = (0x7858<<16) | (3<<8);  //flash access cycles 	
	                            // flash access time cannot overflow ??Mhz.
	                            // ex) if MCLK=40Mhz, 
	                            //       40/1 = 40 (can't set no wait)
	                            //       40/2 = 20 (1 wait is ok)
	                            // so, 1 wait is possible.
	FMC->MR = 0;	      // exit flash access timing --> normal mode		
}



