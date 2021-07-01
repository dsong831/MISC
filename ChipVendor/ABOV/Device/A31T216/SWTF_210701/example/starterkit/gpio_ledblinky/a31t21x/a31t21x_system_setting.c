/**
 *******************************************************************************
 * @file        a31t21x_system_setting.c
 * @brief       Application specific system setting
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "main.h"

#include "a31t21x_scu.h"

void System_Setting_Config(void)
{
//////////////////////////////////////////////////////////////////////////////////////
		/* Configure Clock Parameter */ 																							////
    struct scu_clock_cfg cfg =																										////
    {																																							////
        .mclk_sel = SCU_CLOCK_HSE,			/* User specific value */									////
        .mclk_div = SCU_CLOCK_DIV_1,		/* User specific value */									////
        .mclk_hz = 8000000,							/* User specific value */									////
    };																																						////
//////////////////////////////////////////////////////////////////////////////////////

		/* If using pll clock, configure below pll parameter */
		if(cfg.mclk_sel == SCU_CLOCK_PLL)
		{
			struct scu_pll_clock_cfg p_cfg =
			{
					.pclk_src = SCU_PLL_CLOCK_SRC_HSE,
					.p_mode = SCU_PLL_MODE_VCO_EQUAL,
					.pre_div = 3,		// pre_div = (pclk_src / 2MHz) - 1
					.post_div1 = (uint8_t)((cfg.mclk_hz / 1000000) - 1),
					.post_div2 = 1,
					.out_div = 0,
			};
			HAL_SCU_SetPLLConfig(&p_cfg);
		}

		/* Update Main Clock */
    HAL_SCU_UpdateMainClock(&cfg);
}

