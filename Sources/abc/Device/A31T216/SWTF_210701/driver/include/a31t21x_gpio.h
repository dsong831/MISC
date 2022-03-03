/**
 *******************************************************************************
 * @file        a31t21x_gpio.h
 * @brief       GPIO driver
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
 * @defgroup    driver_gpio GPIO
 * @{
 * @brief       GPIO driver
 */

#ifndef _A31T21X_GPIO_H_
#define _A31T21X_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"



/**
 * GPIO access must be enabled before chaning any register values.
 */
static __INLINE void GPIO_Access_Enable(void)
{
		PORTEN->EN = 0x15;
		PORTEN->EN = 0x51;
}

/**
 * GPIO access must be disabled to protect the settings.
 */
static __INLINE void GPIO_Access_Disable()
{
		PORTEN->EN = 0x00;
}


/* Port n Alternative Function Selection */
#define Pn_MUX_AF0							(0)
#define Pn_MUX_AF1							(1)
#define Pn_MUX_AF2							(2)
#define Pn_MUX_AF3							(3)
#define Pn_MUX_AF4							(4)
#define Pn_MUX_AF5							(5)


/**
 * GPIO port number
 */
enum gpio_port
{
    PORTA       = 0,
    PORTB       = 1,
    PORTC       = 2,
    PORTD       = 3,
    PORTE       = 4,
    PORTF       = 5,
    PORT_MAX    = 6,
};

/**
 * GPIO pin number
 */
enum gpio_pin
{
    PIN0    = 0,
    PIN1    = 1,
    PIN2    = 2,
    PIN3    = 3,
    PIN4    = 4,
    PIN5    = 5,
    PIN6    = 6,
    PIN7    = 7,
    PIN8    = 8,
    PIN9    = 9,
    PIN10   = 10,
    PIN11   = 11,
    PIN12   = 12,
    PIN_MAX = 13,
};

/**
 * GPIO mode
 */
enum gpio_mode
{
    GPIO_MODE_INPUT     = 0,                /** intput */
    GPIO_MODE_OUTPUT    = 1,                /** output */
    GPIO_MODE_FUNC      = 2,                /** alternative function */
    GPIO_MODE_MAX       = 3,                /** Mode Max */
};

/**
 * GPIO output mode configuration
 */
enum gpio_output_type
{
    GPIO_OUTPUT_PUSH_PULL   = 0,            /**< push pull */
    GPIO_OUTPUT_OPEN_DRAIN  = 1,            /**< open drain */
};

/**
 * GPIO pull up down configuration
 */
enum gpio_pull
{
    GPIO_PULL_DISABLE   = 0,                /**< pull up pull down not used */
    GPIO_PULL_UP        = 1,                /**< pull up */
    GPIO_PULL_DOWN      = 2,                /**< pull down */
};

/**
 * GPIO interrupt triger control
 */
enum gpio_irq_trig
{
    GPIO_IRQ_DISABLE 		= 0x00,             /**< disable interrupt */
    GPIO_IRQ_TRIG_LEVEL = 0x01,             /**< level trigger */
    GPIO_IRQ_TRIG_EDGE  = 0x03,             /**< edge trigger */
};

/**
 * GPIO interrupt mode control
 */
enum gpio_irq_mode
{
		GPIO_IRQ_PROHIBIT						= 0x00,     /**< prohibit external interrupt */
    GPIO_IRQ_LOW_OR_FALLING     = 0x01,     /**< low level or falling edge */
    GPIO_IRQ_HIGH_OR_RISING     = 0x02,     /**< high level or rising edge */
    GPIO_IRQ_FALLING_AND_RISING = 0x03,     /**< falling/rising */
};

/**
 * GPIO debounce clock configuration
 */
enum gpio_dbc_clk_src
{
    GPIO_DBC_CLK_SRC_LSI	= 0x00,						/**< debounce clock from LSI */
    GPIO_DBC_CLK_SRC_LSE	= 0x03,						/**< debounce clock from LSE */
    GPIO_DBC_CLK_SRC_MCLK	= 0x04,						/**< debounce clock from MCLK */
    GPIO_DBC_CLK_SRC_HSI	= 0x05,						/**< debounce clock from HSI */
    GPIO_DBC_CLK_SRC_HSE	= 0x06,						/**< debounce clock from HSE */
    GPIO_DBC_CLK_SRC_PLL	= 0x07,						/**< debounce clock from PLL */
};


/**
 * number of irqs to support.
 * If an application requires more than this number of GPIO inputs
 * to be used as interrupt pins, then increase the number as much
 * as needed.
 */
#ifndef CONFIG_GPIO_NUM_IRQ
#define CONFIG_GPIO_NUM_IRQ    4
#endif

/**
 *******************************************************************************
 * @brief       GPIO interrupt callback function type
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*gpio_irq_handler_t)(void *context);

/**
 *******************************************************************************
 * @brief       Initialize GPIO.
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_Init(void);

/**
 *******************************************************************************
 * @brief       Set mode.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   mode mode to be used
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetMode(enum gpio_port port, enum gpio_pin pin, enum gpio_mode mode);

/**
 *******************************************************************************
 * @brief       Set function.
 *              GPIO must be configured as function first.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   func function to be used instead of input/output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetFunction(enum gpio_port port, enum gpio_pin pin, uint32_t func);

/**
 *******************************************************************************
 * @brief       Set output type
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   type output type
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetOutput(enum gpio_port port, enum gpio_pin pin, enum gpio_output_type type);

/**
 *******************************************************************************
 * @brief       Set pull up or down
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   pull pull up/down/none
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetPull(enum gpio_port port, enum gpio_pin pin, enum gpio_pull pull);

/**
 *******************************************************************************
 * @brief       Set GPIO Debouncing.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   enable 1 to enable, 0 to disable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetDebounce(enum gpio_port port, enum gpio_pin pin, uint8_t enable);

/**
 *******************************************************************************
 * @brief       Set GPIO Debouncing Clock
 * @param[in]   port port number
 * @param[in]   clk_src debounce clock source
 * @param[in]   clk_div debounce clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetDebounceClock(enum gpio_port port, enum gpio_dbc_clk_src clk_src,
                                  uint8_t clk_div);

/**
 *******************************************************************************
 * @brief       Enable interrupt.
 *              Interrupt are configured per port, interrupt priority can be
 *              set only once for each port.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   trig interrupt trigger
 * @param[in]   mode interrupt mode
 * @param[in]   irq_prio interrupt priority
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_EnableIRQ(enum gpio_port port, enum gpio_pin pin,
                           enum gpio_irq_trig trig,
                           enum gpio_irq_mode mode,
                           uint8_t irq_prio);

/**
 *******************************************************************************
 * @brief       Disable interrupt.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_DisableIRQ(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Set GPIO callback function to receive event.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @return      driver error code
 * @return      ::DRIVER_ERROR_NO_RESOURCE no more gpio irq can be set.
 *              See ::CONFIG_GPIO_NUM_IRQ
 ******************************************************************************/
int32_t HAL_GPIO_SetIRQHandler(enum gpio_port port, enum gpio_pin pin, gpio_irq_handler_t handler, void *context);

/**
 *******************************************************************************
 * @brief       Set (high) the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_SetPin(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Clear (low) the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ClearPin(enum gpio_port port, enum gpio_pin pin);

/**
 *******************************************************************************
 * @brief       Read the value of the specific port/pin.
 * @param[in]   port port number
 * @param[in]   pin pin number
 * @param[out]  value value of the pin
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ReadPin(enum gpio_port port, enum gpio_pin pin, uint8_t *value);

/**
 *******************************************************************************
 * @brief       Write the value for the whole GPIO port.
 *              It only sets the value for the pins configured as output.
 *              Other bits are not valid.
 * @param[in]   port port number
 * @param[in]   value port value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_WritePort(enum gpio_port port, uint8_t value);

/**
 *******************************************************************************
 * @brief       Read the value for the whole GPIO port.
 *              It only returns the value for the pins configured as input.
 *              Other bits are not valid.
 * @param[in]   port port number
 * @param[out]  value port value
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_GPIO_ReadPort(enum gpio_port port, uint8_t *value);

#ifdef __cplusplus
}
#endif

#endif /* _A31T21X_GPIO_H_ */

/** @} */
/** @} */

