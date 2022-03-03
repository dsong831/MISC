/**
 *******************************************************************************
 * @file        a31T21x_driver_common.h
 * @brief       Driver common
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
 * @addtogroup  driver
 * @{
 * @defgroup    driver_common Common
 * @{
 * @brief       Driver common function
 */


#ifndef _A31T21X_DRIVER_COMMON_H_
#define _A31T21X_DRIVER_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdio.h>
#include <string.h>


/**
 * Driver state
 */
enum driver_state
{
    DRIVER_DISABLE  = 0,
    DRIVER_ENABLE   = 1,
};

/**
 * Driver general error code
 */
enum driver_error
{
    DRIVER_ERROR_OK             = 0,        /**< operation succeeded */
    DRIVER_ERROR_UNKNOWN        = -1,       /**< unspecified error */
    DRIVER_ERROR_INVALID_ID     = -2,       /**< peripheral id is invalid */
    DRIVER_ERROR_BUSY           = -3,       /**< busy */
    DRIVER_ERROR_TIMEOUT        = -4,       /**< timeout occurred */
    DRIVER_ERROR_NOT_SUPPORTED  = -5,       /**< operation not supported */
    DRIVER_ERROR_PARAMETER      = -6,       /**< parameter error */
    DRIVER_ERROR_NO_RESOURCE    = -7,       /**< not enough resource */
    DRIVER_ERROR_INVALID_USE    = -8,       /**< not configured properly */
    DRIVER_ERROR_HW             = -9,       /**< hardware error */
    DRIVER_ERROR_SPECIFIC       = -10,      /**< driver specific errors */
};

/**
 * Driver IP specific error code
 */
enum driver_i2c_error
{
    DRIVER_ERROR_I2C_MASTERSHIP_LOST    = DRIVER_ERROR_SPECIFIC - 1,
    DRIVER_ERROR_I2C_NO_ACK             = DRIVER_ERROR_SPECIFIC - 2,
    DRIVER_ERROR_I2C_STOP               = DRIVER_ERROR_SPECIFIC - 3,
    DRIVER_ERROR_I2C_HW                 = DRIVER_ERROR_SPECIFIC - 4,
};

/**
 * Driver Priority, smaller number means higher priority
 */
enum driver_priority
{
    DRIVER_PRIORITY_0   = 0,                /**< priority 0 */
    DRIVER_PRIORITY_1   = 1,                /**< priority 1 */
    DRIVER_PRIORITY_2   = 2,                /**< priority 2 */
    DRIVER_PRIORITY_3   = 3,                /**< priority 3 */
    DRIVER_PRIORITY_4   = 4,                /**< priority 4 */
    DRIVER_PRIORITY_5   = 5,                /**< priority 5 */
    DRIVER_PRIORITY_6   = 6,                /**< priority 6 */
    DRIVER_PRIORITY_7   = 7,                /**< priority 7 */
    DRIVER_PRIORITY_8   = 8,                /**< priority 8 */
    DRIVER_PRIORITY_9   = 9,                /**< priority 0 */
    DRIVER_PRIORITY_10  = 10,               /**< priority 10 */
    DRIVER_PRIORITY_11  = 11,               /**< priority 11 */
    DRIVER_PRIORITY_12  = 12,               /**< priority 12 */
    DRIVER_PRIORITY_13  = 13,               /**< priority 13 */
    DRIVER_PRIORITY_14  = 14,               /**< priority 14 */
    DRIVER_PRIORITY_15  = 15,               /**< priority 15 */
    /* (1 << __NVIC_PRIO_BITS) - 1 */
};

#define DRIVER_PRIORITY_HIGHEST 0           /**< Priority highest */
#define DRIVER_PRIORITY_LOWEST  15          /**< Priority lowest */

/**
 * Assert function specific to the application
 */
extern void Assert(char *file, uint32_t line);

/**
 * Driver assert, application needs to declare a custom assert function
 */
#if (CONFIG_DEBUG == 1)
#define DRIVER_ASSERT(cond)     ((cond) ? ((void)0) : Assert(__FILE__, __LINE__))
#else
#define DRIVER_ASSERT(cond)     ((void)(cond))
#endif

/**
 * maximum value macro
 */
#if !defined(MAX)
#define MAX(a, b)               (((a) > (b)) ? (a) : (b))
#endif

/**
 * minimum value macro
 */
#if !defined(MIN)
#define MIN(a, b)               (((a) < (b)) ? (a) : (b))
#endif

/**
 * bit macro
 */
#if !defined(BIT)
#define BIT(n)                  (1 << (n))
#endif

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_DRIVER_COMMON_H_ */

/** @} */
/** @} */

