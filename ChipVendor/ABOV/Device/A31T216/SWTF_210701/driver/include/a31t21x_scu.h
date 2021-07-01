/**
 *******************************************************************************
 * @file        a31t21x_scu.h
 * @brief       SCU driver
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
 * @defgroup    driver_scu SCU
 * @{
 * @brief       SCU driver
 */

#ifndef _A31T21X_SCU_H_
#define _A31T21X_SCU_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "a31t21x.h"
#include "a31t21x_driver_common.h"



/**
 * SCU clock selection
 */
enum scu_clock
{
    SCU_CLOCK_LSI			= 0,					/**< Internal low speed clcok(500Khz) */
    SCU_CLOCK_LSE			= 1,					/**< External low speed clcok */
    SCU_CLOCK_HSI			= 2,					/**< Internal high speed clock(32Mhz) */
    SCU_CLOCK_HSE			= 6,					/**< Extern high speed clock */
    SCU_CLOCK_PLL			= 7,					/**< PLL clock from HSE 8MHz */
};

/**
 * SCU clock divider
 */
enum scu_clock_div
{
    SCU_CLOCK_DIV_NONE			= 0,    /**< Do not divide clock */
    SCU_CLOCK_DIV_1					= 8,    /**< Divide the clock by 1 */
    SCU_CLOCK_DIV_2					= 9,    /**< Divide the clock by 2 */
    SCU_CLOCK_DIV_4					= 10,   /**< Divide the clock by 4 */
    SCU_CLOCK_DIV_8					= 11,   /**< Divide the clock by 8 */
    SCU_CLOCK_DIV_16				= 12,   /**< Divide the clock by 16 */
    SCU_CLOCK_DIV_32				= 13,   /**< Divide the clock by 32 */
};

/**
 * SCU PLL clock source
 */
enum scu_pll_clock_src
{
    SCU_PLL_CLOCK_SRC_HSI   = 0,    /**< PLL clock source internal high speed clock(32Mhz) */
    SCU_PLL_CLOCK_SRC_HSE   = 1,    /**< PLL clock source external high speed clock */
};

/**
 * SCU PLL mode
 */
enum scu_pll_mode
{
    SCU_PLL_MODE_VCO_EQUAL      = 0,    /**< The VCO frequency is the same as FOUT */
    SCU_PLL_MODE_VCO_DOUBLER    = 1,    /**< The VCO frequency is twice FOUT */
};

/**
 * SCU clock monitoring
 */
enum scu_clock_mon
{
    SCU_MON_CLOCK_MCLK  = 6,						/**< Mornitoring main clock */
    SCU_MON_CLOCK_LSE   = 10,					/**< Mornigoring external low speed clock */
    SCU_MON_CLOCK_HSE   = 2,						/**< Mornigoring external high speed clock */
};

/**
 * SCU clock failure event
 */
enum scu_clock_fail_event
{
    SCU_CLOCK_FAIL_EVENT_MCLK = (1 << 5),     /**< Main clock fail event */
    SCU_CLOCK_FAIL_EVENT_LSE  = (1 << 9),     /**< Extern low speed clock fail event */
    SCU_CLOCK_FAIL_EVENT_HSE  = (1 << 1),     /**< Extern high speed clock fail event */
};

/**
 * SCU reset source
 */
enum scu_reset_source
{
    SCU_RESET_SOURCE_PIN    = (1 << 6),     /**< Reset by pin */
    SCU_RESET_SOURCE_CPU    = (1 << 5),     /**< Reset by cpu */
    SCU_RESET_SOURCE_SW     = (1 << 4),     /**< Reset by s/w */
    SCU_RESET_SOURCE_WDT    = (1 << 3),     /**< Reset by watchdog */
    SCU_RESET_SOURCE_CLKF   = (1 << 2),     /**< Reset by clock failure */
    SCU_RESET_SOURCE_HSE    = (1 << 1),     /**< Reset by HSE */
    SCU_RESET_SOURCE_LVD    = (1 << 0),     /**< Reset by low voltage */
};

/**
 * SCU reset reason
 */
enum scu_reset_reason
{
    SCU_RESET_REASON_POR    = (1 << 7),     /**< Reset by POR */
    SCU_RESET_REASON_PIN    = (1 << 6),     /**< Reset by pin */
    SCU_RESET_REASON_CPU    = (1 << 5),     /**< Reset by cpu */
    SCU_RESET_REASON_SW     = (1 << 4),     /**< Reset by s/w */
    SCU_RESET_REASON_WDT    = (1 << 3),     /**< Reset by watchdog */
    SCU_RESET_REASON_CLKF   = (1 << 2),     /**< Reset by clock failure */
    SCU_RESET_REASON_HSE    = (1 << 1),     /**< Reset by HSE */
    SCU_RESET_REASON_LVD    = (1 << 0),     /**< Reset by low voltage */
};

/**
 * SCU wakeup source
 */
enum scu_wakeup_source
{
    SCU_WAKEUP_SOURCE_GPIOF		= (1 << 13),    /**< Wakeup by GPIO F */
    SCU_WAKEUP_SOURCE_GPIOE		= (1 << 12),    /**< Wakeup by GPIO E */
    SCU_WAKEUP_SOURCE_GPIOD		= (1 << 11),    /**< Wakeup by GPIO D */
    SCU_WAKEUP_SOURCE_GPIOC		= (1 << 10),    /**< Wakeup by GPIO C */
    SCU_WAKEUP_SOURCE_GPIOB		= (1 << 9),     /**< Wakeup by GPIO B */
    SCU_WAKEUP_SOURCE_GPIOA		= (1 << 8),     /**< Wakeup by GPIO A */
    SCU_WAKEUP_SOURCE_USART11	= (1 << 4),			/**< Wakeup by USART 11 */
    SCU_WAKEUP_SOURCE_USART10	= (1 << 3), 	  /**< Wakeup by USART 10 */
    SCU_WAKEUP_SOURCE_WT			= (1 << 2),     /**< Wakeup by WT */
    SCU_WAKEUP_SOURCE_WDT			= (1 << 1),     /**< Wakeup by WDT */
    SCU_WAKEUP_SOURCE_LV			= (1 << 0),     /**< Wakeup by LVI */
};

/**
 * SCU wakeup reason
 */
enum scu_wakeup_reason
{
    SCU_WAKEUP_REASON_GPIOF		= (1 << 13),    /**< Wakeup by GPIO F */
    SCU_WAKEUP_REASON_GPIOE		= (1 << 12),    /**< Wakeup by GPIO E */
    SCU_WAKEUP_REASON_GPIOD		= (1 << 11),    /**< Wakeup by GPIO D */
    SCU_WAKEUP_REASON_GPIOC		= (1 << 10),    /**< Wakeup by GPIO C */
    SCU_WAKEUP_REASON_GPIOB		= (1 << 9),     /**< Wakeup by GPIO B */
    SCU_WAKEUP_REASON_GPIOA		= (1 << 8),     /**< Wakeup by GPIO A */
    SCU_WAKEUP_REASON_USART11	= (1 << 4),			/**< Wakeup by USART 11 */
    SCU_WAKEUP_REASON_USART10	= (1 << 3), 	  /**< Wakeup by USART 10 */
    SCU_WAKEUP_REASON_WT			= (1 << 2),     /**< Wakeup by WT */
    SCU_WAKEUP_REASON_WDT			= (1 << 1),     /**< Wakeup by WDT */
    SCU_WAKEUP_REASON_LV			= (1 << 0),     /**< Wakeup by LVI */
};

/**
 *******************************************************************************
 * @brief       SCU clock failure interrupt callback function type
 * @param[in]   event event type ::scu_clock_fail_event
 * @param[in]   context context provided during initialization
 ******************************************************************************/
typedef void (*scu_clk_fail_handler_t)(uint32_t event, void *context);

/**
 * SCU clock configuration
 */
struct scu_clock_cfg
{
    enum scu_clock          mclk_sel;       /**<system main clock */
    enum scu_clock_div			mclk_div;				/**<divide the clock and use the clock */
    uint32_t                mclk_hz;        /**<frequncy of main clock */
};

/**
 * SCU PLL clock configuration
 */
struct scu_pll_clock_cfg
{
    uint8_t                         out_div;            /**<output divide value */
    uint8_t                         post_div1;          /**<post divide 1 value */
    uint8_t                         post_div2;          /**<post divide 2 value */
    uint8_t                         pre_div;            /**<previous divide value */
    enum scu_pll_mode               p_mode;             /**<pll mode */
    enum scu_pll_clock_src          pclk_src;           /**<pll clock source */
};


/**
 *******************************************************************************
 * @brief       Update system main clock.
                If the main clock is selected as PLL, must be called
 * @param[in]   cfg configration for the main clcok
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_UpdateMainClock(struct scu_clock_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Configure PLL
 * @param[in]   cfg configuration for the PLL clock
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_SetPLLConfig(struct scu_pll_clock_cfg *cfg);

/**
 *******************************************************************************
 * @brief       Enable system clock
 * @param[in]   clk type of clock to enable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClock(enum scu_clock clk);

/**
 *******************************************************************************
 * @brief       Disable system clock
 * @param[in]   clk type of clock to enable
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClock(enum scu_clock clk);

/**
 *******************************************************************************
 * @brief       Enable clock monitoring
 * @param[in]   clk clock to monitor
 * @param[in]   handler callback function to receive event
 * @param[in]   context caller's context
 * @param[in]   irq_prio interrupt priority
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClockMonitor(enum scu_clock_mon clk,
                                   scu_clk_fail_handler_t handler,
                                   void *context,
                                   uint8_t irq_prio);
/**
 *******************************************************************************
 * @brief       Disable clock monitoring
 * @param[in]   clk clock to monitor
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClockMonitor(enum scu_clock_mon clk);

/**
 *******************************************************************************
 * @brief       Enable clock output
 * @param[in]   clk_div clock divider
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableClockOut(uint8_t clk_div);

/**
 *******************************************************************************
 * @brief       Disable clock output
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableClockOut(void);

/**
 *******************************************************************************
 * @brief       Enable wakeup source
 * @param[in]   src sources (bit ORed value of ::scu_wakeup_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableWakeupSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Disable wakeup source
 * @param[in]   src sources (bit ORed value of ::scu_wakeup_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableWakeupSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Enable reset source
 * @param[in]   src sources (bit ORed value of ::scu_reset_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_EnableResetSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       Disable reset source
 * @param[in]   src sources (bit ORed value of ::scu_reset_source)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_DisableResetSource(uint32_t src);

/**
 *******************************************************************************
 * @brief       This function returns the reset reason.
 *              Once the reason is obtained, it is cleared within this function
 *              so that next reset will have only the latest reset reason only.
 * @param[in]   reason reset reason (::scu_reset_reason)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_GetResetReason(uint32_t *reason);

/**
 *******************************************************************************
 * @brief       This function returns the wakeup reason.
 *              Once the reason is obtained, wakeup source is disabled within this function
 *              so that the next wakeup has to be enabled again from the application
 * @param[in]   reason wakeup reason (::scu_wakeup_reason)
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_GetWakeupReason(uint32_t *reason);

/**
 *******************************************************************************
 * @brief       Set Timer1n Clock
 * @param[in]   t1nclk               T1NCLK_MCCR1, T1NCLK_PCLK
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_Timer1n_ClockConfig(uint32_t t1nclk);

/**
 *******************************************************************************
 * @brief       Set Timer20 Clock
 * @param[in]   t20clk               TIMER20CLK_MCCR2, TIMER20CLK_PCLK
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_Timer20_ClockConfig(uint32_t t20clk);

/**
 *******************************************************************************
 * @brief       Set Timer30 Clock
 * @param[in]   t30clk               T30CLK_MCCR2, T30CLK_PCLK
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_Timer30_ClockConfig(uint32_t t30clk);

/**
 *******************************************************************************
 * @brief       Set LED Clock
 * @param[in]   ledclk               LEDCLK_MCCR5, LEDCLK_PCLK
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_LED_ClockConfig(uint32_t ledclk);

/**
 *******************************************************************************
 * @brief       Set WT Clock
 * @param[in]   wtclk               WTCLK_MCCR3, WTCLK_SOSC, WTCLK_WDTRC
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_WT_ClockConfig(uint32_t wtclk);

/**
 *******************************************************************************
 * @brief       Set WDT Clock
 * @param[in]   wdtclk               WDTCLK_WDTRC, WDTCLK_MCCR3
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_WDT_ClockConfig(uint32_t wdtclk);

/**
 *******************************************************************************
 * @brief       Set LCD Clock
 * @param[in]   lcdclk               LCDCLK_MCCR5, LCDCLK_SOSC, LCDCLK_WDTRC
 * @return      driver error code
 ******************************************************************************/
int32_t HAL_SCU_LCD_ClockConfig(uint32_t lcdclk);


#endif /* _A31T21X_SCU_H_ */

/** @} */
/** @} */

