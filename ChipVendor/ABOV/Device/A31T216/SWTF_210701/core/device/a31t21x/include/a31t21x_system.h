/**
 *******************************************************************************
 * @file        a31t21x_system.h
 * @brief       System configuration
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

/**
 * @addtogroup device
 * @{
 * @defgroup device_system System
 * @{
 * @brief System configuration
 */


#ifndef _A31T21X_SYSTEM_H_
#define _A31T21X_SYSTEM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Global Chipset ID macro
 */
#ifndef A31T21X
#define A31T21X
#endif

extern uint32_t SystemCoreClock;        /**< System Clock Frequency */
extern uint32_t SystemPeriClock;        /**< System Peripheral clock Frequency */
extern uint32_t SystemDelayCount;       /**< System delay count used for the delay */

/**
 *******************************************************************************
 * @brief       Setup the microcontroller system.
 *              Initialize the System.
 *              This function is normally called from the startup before
 *              program jumps to the application's main function.
 * @return      none
 ******************************************************************************/
void SystemInit(void);

/**
 *******************************************************************************
 * @brief       Delay using loop
 *              This function is based on _NOP instruction and does not provide
 *              accurate delay.
 * @param[in]   ms delay in millisecond
 * @return      none
 ******************************************************************************/
void SystemDelay(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_SYSTEM_H_ */

/** @} */
/** @} */

