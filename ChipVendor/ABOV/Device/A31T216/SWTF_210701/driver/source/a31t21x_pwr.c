/**
 *******************************************************************************
 * @file        a31t21x_pwr.c
 * @brief       pwr driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_pwr.h"


#define LVR_DISABLE_VALUE					0x55		// For LVRCR
#define	LVR_DISABLE_CONFIG_VALUE	0xAA		// For LVRCNFIG

struct pwr_ctrl_block
{
    pwr_irq_handler_t   handler;
    void *              context;
};

static struct pwr_ctrl_block g_pcb;

static void PWR_SYSTEM_Configure(uint8_t stop_sel, uint8_t aon_sel)
{
		uint32_t prev_vdccon;

		/* Configure Stop Mode */
		SCU->VDCCON = (0x5 << 20) | (0x1 << 8) | (0x7F << 0);						// VDC warm-up delay count value 4ms
		prev_vdccon = SCU->VDCCON;
		if (stop_sel == 1)
		{
				SCU->VDCCON = (0x5 << 20) | (prev_vdccon & (0xFFFDFFFF));		// STOP1 Mode
		}
		else
		{
				SCU->VDCCON = (0x5 << 20) | prev_vdccon | (1 << 17);				// STOP2 Mode
		}

		/* Disable LVR */
		HAL_LVR_Deinit();

		/* Configure AON Block */
		if (aon_sel)
		{
				SCU->SMR |= (aon_sel << 8);
		}
		else
		{
				SCU->SMR &= ~(7 << 8);			// Clear All AON Block
		}
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LVI_Init(struct lvi_cfg *cfg,
                     pwr_irq_handler_t handler,
                     void *context)
{
    uint32_t reg_val;
	
		DRIVER_ASSERT(cfg != NULL);

		/* Set LVI Parameter */
		reg_val = 0;
		reg_val |= cfg->level;
	  reg_val |= (1 << 5);				// <LVIINTEN>
	  reg_val |= (1 << 7);				// <LVIEN>

		SCULV->LVICR = reg_val;
	
    g_pcb.handler = handler;
    g_pcb.context = context;

		/* Set NVIC */
    NVIC_SetPriority(LVI_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(LVI_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LVI_Deinit(void)
{
		/* Disable LVI */
    SCULV->LVICR &= ~(1 << 7);	// <LVIEN>

		/* Clear NVIC */
    NVIC_DisableIRQ(LVI_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LVR_Init(struct lvr_cfg *cfg)
{
    uint32_t reg_val; 

		DRIVER_ASSERT(cfg != NULL);

		/* Enable LVR with LVRCNFIG Register */
		reg_val = 0;
		reg_val |= 0xF - cfg->level;
		SCULV->LVRCNFIG = (0x72A5UL << 16) | reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_LVR_Deinit(void)
{
    uint32_t reg_val; 

		/* Disable LVR with LVRCR Register */
    SCULV->LVRCR = (LVR_DISABLE_VALUE << 0);

		/* Disable LVR with LVRCNFIG Register */
		reg_val = (SCULV->LVRCNFIG & 0x0F);
		SCULV->LVRCNFIG = (0x72A5UL << 16) | (LVR_DISABLE_CONFIG_VALUE << 8) | reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
void HAL_PWR_EnterSleepMode(void)
{
    SCB->SCR = 0;
    __DSB();
    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

//////////////////////////////////////////////////////////////////////////////////////
void HAL_PWR_EnterDeepSleepMode(void)
{
		PWR_SYSTEM_Configure(2, 4);		// LSI ON when STOP2

    SCB->SCR = (1 << SCB_SCR_SLEEPDEEP_Pos);
    __DSB();		 	
    __WFI();
    __NOP();
    __NOP();
    __NOP();
    __NOP();
}

//////////////////////////////////////////////////////////////////////////////////////
void LVI_IRQHandler(void)
{
		/* Clear LVI Interrupt Flag bit */
    SCULV->LVICR |= (1 << 4);		// <LVIFLAG>

		/* Callback Function */
    g_pcb.handler(PWR_EVENT_LVI, g_pcb.context);
}

