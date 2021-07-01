/**
 *******************************************************************************
 * @file        a31t21x_i2c.h
 * @brief       I2C driver
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
 * @defgroup    driver_i2c I2C
 * @{
 * @brief       I2C driver
 */

#ifndef _A31T21X_I2C_H_
#define _A31T21X_I2C_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"

/**
 * I2C instance id
 */
enum i2c_id
{
    I2C_ID_0    = 0,                        /**< I2C 0 */
    I2C_ID_1    = 1,                        /**< I2C 1 */
    I2C_ID_MAX  = 2,                        /**< I2C max */
};

/**
 * I2C master clock frequency
 */
enum i2c_freq
{
    I2C_FREQ_400KHZ = 0,                    /**< frequency 400 KHz */
    I2C_FREQ_200KHZ = 1,                    /**< frequency 200 KHz */
    I2C_FREQ_100KHZ = 2,                    /**< frequency 100 KHz */
};

/**
 * I2C IO type
 */
enum i2c_io_type
{
    I2C_IO_TYPE_BLOCK       = 0,            /**< blocking */
    I2C_IO_TYPE_NON_BLOCK   = 1,            /**< non blocking */
};

/**
 * I2C event
 */
enum i2c_event
{
    I2C_EVENT_TRASMIT_DONE      = BIT(0),   /**< transmit is complete */
    I2C_EVENT_RECEIVE_DONE      = BIT(1),   /**< reception is complete */
    I2C_EVENT_MASTERSHIP_LOSS   = BIT(2),   /**< master detected another master */
    I2C_EVENT_NO_ACK            = BIT(3),   /**< no acknowledgement received */
    I2C_EVENT_HW_ERR            = BIT(4),   /**< hardware error */
};

/**
 *******************************************************************************
 * @brief       I2C interrupt callback function type
 * @param[in]   event event type (bit OR) ::i2c_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*i2c_irq_handler_t)(uint32_t event, void *context);

/**
 * I2C configuration
 */
struct i2c_cfg
{
    enum i2c_freq   freq;                   /**< i2c frequency */
    uint8_t         irq_prio;               /**< interrupt priority */
};

/**
 *******************************************************************************
 * @brief       Initialize I2C.
 * @param[in]   id instance id
 * @param[in]   cfg configuration
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_Init(enum i2c_id id, struct i2c_cfg *cfg,
                     i2c_irq_handler_t handler,
                     void *context);

/**
 *******************************************************************************
 * @brief       Deintialize I2C.
 * @param[in]   id instance id
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_Deinit(enum i2c_id id);

/**
 *******************************************************************************
 * @brief       I2C Master transmit data.
 *              Data should not be changed until trasmittion is complete.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is transmitted.
 * @param[in]   id instance id
 * @param[in]   slave_addr slave address
 * @param[in]   out data to transmit from
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_MasterTransmit(enum i2c_id id, uint8_t slave_addr, uint8_t *out, uint32_t len,
                               enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Master receive data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[in]   slave_addr slave address
 * @param[in]   in data to receive to
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_MasterReceive(enum i2c_id id, uint8_t slave_addr, uint8_t *in, uint32_t len,
                              enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Master burst transmit.
 *              This is a convenience function to interface with a slave device
 *              with 16 bit or more register indexing.
 * @param[in]   id instance id
 * @param[in]   slave_addr slave address
 * @param[in]   reg slave device register address
 * @param[in]   reg_len slave device register len
 * @param[in]   out data to transmit from
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_MasterBurstTransmit(enum i2c_id id, uint8_t slave_addr,
                                    uint8_t *reg, uint8_t reg_len,
                                    uint8_t *out, uint32_t len,
                                    enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Master burst receive.
 *              This is a convenience function to interface with a slave device
 *              with 16 bit or more register indexing.
 * @param[in]   id instance id
 * @param[in]   slave_addr slave address
 * @param[in]   reg slave device register address
 * @param[in]   reg_len slave device register len
 * @param[in]   in data to receive to
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_MasterBurstReceive(enum i2c_id id, uint8_t slave_addr,
                                   uint8_t *reg, uint8_t reg_len,
                                   uint8_t *in, uint32_t len,
                                   enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Slave transmit data.
 *              Data should not be changed until trasmittion is complete.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is transmitted.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_SlaveTransmit(enum i2c_id id, uint8_t *out, uint32_t len,
                              enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Slave receive data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the interrupt handler.
 *              If io_type is blocking, the function waits until the requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[in]   in data to receive to
 * @param[in]   len length of data
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_SlaveReceive(enum i2c_id id, uint8_t *in, uint32_t len,
                             enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       I2C Slave transmit/receive data.
 *              If io_type is nonblocking, the function returns immediately.
 *              Completion is signalled by the event handler.
 *              If io_type is blocking, the function waits until requested
 *              amount of the data is received.
 * @param[in]   id instance id
 * @param[in]   out data to transmit from
 * @param[in]   out_len length of data to transmit
 * @param[in]   in data to receive to
 * @param[in]   in_len length of data to receive
 * @param[in]   io_type operation type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_SlaveTransfer(enum i2c_id id, uint8_t *out, uint32_t out_len,
                              uint8_t *in, uint32_t in_len, enum i2c_io_type io_type);

/**
 *******************************************************************************
 * @brief       Set I2C slave address.
 * @param[in]   id instance id
 * @param[in]   own_addr own address used slave mode
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_I2C_SetOwnAddress(enum i2c_id id, uint8_t own_addr);

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_I2C_H_ */

/** @} */
/** @} */
