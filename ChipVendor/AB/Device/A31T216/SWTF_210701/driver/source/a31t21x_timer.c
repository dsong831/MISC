/**
 *******************************************************************************
 * @file        a31t21x_timer.c
 * @brief       TIMER driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_timer.h"


#define TIMER_REG_OFFSET    0x100

static __INLINE TIMER_Type *TIMER_REG(enum timer_id id)
{
		/* TIMER10/11/12/13 */
		if (id < 4)
		{
				if (id != TIMER_ID_13)
				{
						return (TIMER_Type *)(TIMER10_BASE + (TIMER_REG_OFFSET * id));
				}
				else
				{
						return (TIMER_Type *)(TIMER10_BASE + (0x600));	// TIMER13
				}
		}
		/* TIMER20 */
		else
		{
				return (TIMER_Type *)(TIMER20_BASE + (TIMER_REG_OFFSET * (id - 4)));
		}
}

static __INLINE IRQn_Type TIMER_IRQ_NUM(enum timer_id id)
{
		/* TIMER10/11/12/13 */
		if (id < 4)
		{
				if (id != TIMER_ID_13)
				{
						return (IRQn_Type)((uint8_t)TIMER10_IRQn + id);
				}
				else
				{
						return (IRQn_Type)((uint8_t)TIMER13_IRQn);		// TIMER13
				}
		}
		/* TIMER20 */
		else
		{
				return (IRQn_Type)((uint8_t)TIMER20_IRQn + (id - 4));
		}
}

struct timer_ctrl_block
{
    uint8_t								mode;
    uint8_t           	  pwm_event;
    timer_irq_handler_t		handler;
    void *								context;
};

static struct timer_ctrl_block g_tcb[TIMER_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
static int32_t TIMER_Configure(struct timer_ctrl_block *tcb,
                               TIMER_Type *timer,
                               struct timer_cfg *cfg)
{
    uint32_t cfg_val = 0;

		/* Configure Timer Operation Mode */
    switch (cfg->mode)
    {
        case TIMER_MODE_PERIODIC:
						cfg_val |= (TIMER_MODE_PERIODIC << 12);
						tcb->mode = cfg->mode;
						break;
        case TIMER_MODE_CAPTURE:
						cfg_val |= (TIMER_MODE_CAPTURE << 12);
						tcb->mode = cfg->mode;
						break;
        case TIMER_MODE_ONESHOT:
						cfg_val |= (TIMER_MODE_ONESHOT << 12);
						tcb->mode = cfg->mode;
						break;
        case TIMER_MODE_PWM:
						cfg_val |= (TIMER_MODE_PWM << 12);
            tcb->mode = cfg->mode;
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Configure Timer Clock Selection */
    switch (cfg->clk_sel)
    {
        case TIMER_CLK_MCCLK:
        case TIMER_CLK_PCLK:
						cfg_val &= ~(1 << 14);
						break;
        case TIMER_CLK_TNIO:
						cfg_val |= (1 << 14);
            break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Check range of prescaler */
    if (cfg->prescaler > 4095)
    {
        return DRIVER_ERROR_PARAMETER;
    }

    timer->CR = cfg_val;
    timer->PREDR = (cfg->prescaler - 1) & 0xFFF;
		timer->CR |= (1 << 0);	// Clear Counter

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_Init(enum timer_id id, struct timer_cfg *cfg,
                       timer_irq_handler_t handler,
                       void *context)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;
    int32_t ret;

    DRIVER_ASSERT(id < TIMER_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

    memset(tcb, 0, sizeof(struct timer_ctrl_block));

    tcb->handler = handler;
    tcb->context = context;

		/* Timer Block Enable */
		if (id == TIMER_ID_10)
		{
				SCU->PER1 &= ~(1<<16);
				SCU->PCER1 &= ~(1<<16);
				SCU->PER1 |= (1<<16);
				SCU->PCER1 |= (1<<16);
		}
		else if (id == TIMER_ID_11)
		{
				SCU->PER1 &= ~(1<<17);
				SCU->PCER1 &= ~(1<<17);
				SCU->PER1 |= (1<<17);
				SCU->PCER1 |= (1<<17);
		}
		else if (id == TIMER_ID_12)
		{
				SCU->PER1 &= ~(1<<18);
				SCU->PCER1 &= ~(1<<18);
				SCU->PER1 |= (1<<18);
				SCU->PCER1 |= (1<<18);
		}
		else if (id == TIMER_ID_13)
		{
				SCU->PER1 &= ~(1<<19);
				SCU->PCER1 &= ~(1<<19);
				SCU->PER1 |= (1<<19);
				SCU->PCER1 |= (1<<19);
		}
		else if (id == TIMER_ID_20)
		{
				SCU->PER1 &= ~(1<<25);
				SCU->PCER1 &= ~(1<<25);
				SCU->PER1 |= (1<<25);
				SCU->PCER1 |= (1<<25);
		}
		else if (id == TIMER_ID_21)
		{
				SCU->PER1 &= ~(1<<26);
				SCU->PCER1 &= ~(1<<26);
				SCU->PER1 |= (1<<26);
				SCU->PCER1 |= (1<<26);
		}

		/* Configure Timer Parameter */
    ret = TIMER_Configure(tcb, timer, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_TIMER_Deinit(id);
        return ret;
    }

		/* Set NVIC */
    NVIC_ClearPendingIRQ(TIMER_IRQ_NUM(id));
    NVIC_SetPriority(TIMER_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(TIMER_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_Deinit(enum timer_id id)
{
    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Timer1n Block Disable */
		if (id == TIMER_ID_10)
		{
				SCU->PER1 &= ~(1<<16);
				SCU->PCER1 &= ~(1<<16);
		}
		else if (id == TIMER_ID_11)
		{
				SCU->PER1 &= ~(1<<17);
				SCU->PCER1 &= ~(1<<17);
		}
		else if (id == TIMER_ID_12)
		{
				SCU->PER1 &= ~(1<<18);
				SCU->PCER1 &= ~(1<<18);
		}
		else if (id == TIMER_ID_13)
		{
				SCU->PER1 &= ~(1<<19);
				SCU->PCER1 &= ~(1<<19);
		}
		else if (id == TIMER_ID_20)
		{
				SCU->PER1 &= ~(1<<25);
				SCU->PCER1 &= ~(1<<25);
		}
		else if (id == TIMER_ID_21)
		{
				SCU->PER1 &= ~(1<<26);
				SCU->PCER1 &= ~(1<<26);
		}

		/* Clear NVIC */
    NVIC_DisableIRQ(TIMER_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_GetStatus(enum timer_id id, uint8_t *status)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

		/* Get Interrupt Flags */
    *status = timer->CR & 0x0c;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_Start(enum timer_id id, uint32_t count)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

    DRIVER_ASSERT(id < TIMER_ID_MAX);
    DRIVER_ASSERT(tcb->mode == TIMER_MODE_PERIODIC || tcb->mode == TIMER_MODE_ONESHOT);

		/* Set A-Match Parameter */
    timer->ADR = count - 1;

		/* Counter clear and start */
    timer->CR |= (1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_StartPWM(enum timer_id id, enum timer_pwm_start_level level,
                           uint32_t duty, uint32_t period,
                           uint8_t event)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

    DRIVER_ASSERT(id < TIMER_ID_MAX);
    DRIVER_ASSERT(tcb->mode == TIMER_MODE_PWM);

		/* Check PWM Period Match(A-Match) Event */
    if (event & TIMER_PWM_EVENT_PERIOD)
    {
        timer->CR |= (1 << 5);	// Enable Timer Match Interrupt
    }
    tcb->pwm_event = event;

		/* Set PWM Period & Duty */
    timer->ADR = period;
    timer->BDR = duty;

		/* Set PWM Start Level */
    if (level == TIMER_PWM_START_LOW)
    {
        timer->CR |= (1 << 8);
    }
    else
    {
        timer->CR &= ~(1 << 8);
    }
		
		/* Counter clear and start */
    timer->CR |= (1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_StartCapture(enum timer_id id, enum timer_capture_clear clear)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

    DRIVER_ASSERT(id < TIMER_ID_MAX);
    DRIVER_ASSERT(tcb->mode == TIMER_MODE_CAPTURE);

		/* Set Capture Polarity */
    timer->CR &= ~(3 << 6);
    timer->CR |= (clear << 6);

		/* Counter clear and start */
		timer->ADR = 0xFFFFFFFF;
    timer->CR |= (1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_Stop(enum timer_id id)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

		/* Disable timer operation */
    timer->CR &= ~(1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_GetCount(enum timer_id id, uint32_t *count)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

    *count = timer->CNT;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_ClearCount(enum timer_id id)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

    timer->CR = (1 << 0);		// Clear Timer Counter

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_GetCaptureCount(enum timer_id id, uint32_t *count, enum timer_capture_reg reg)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

		/* Get Capture Data */
    if (reg == TIMER_CAPTURE_REG_CAPDR)
    {
        *count = timer->CAPDR;
    }
    else
    {
				// Test Signal For LSE and WDTRC. Check the TIMER20_CR<CAPSEL>.
        *count = TIMER20->CAPDR;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_SetClock(enum timer_id id, enum timer_clk_sel clk_sel, uint16_t prescaler,
                           enum timer_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		uint32_t reg_val;
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

		/* Check range of prescaler */
    if (prescaler > 4095)
    {
        return DRIVER_ERROR_PARAMETER;
    }

		/* Set External Clock Source */
		// TIMER10/11/12/13
		if (id < 4)
		{
				if (clk_sel == TIMER_CLK_MCCLK)
				{
						// Select MCCR Source
						reg_val = SCU->PPCLKSR;
						reg_val &= ~(1 << 22);
						SCU->PPCLKSR = reg_val;

						// Set MCCR Clock
						SCU->MCCR1 &= ~(0x7FF << 16);
						SCU->MCCR1 |= (mcclk_sel << 24);
						SCU->MCCR1 |= (mcclk_div << 16);
				}
				else
				{
						// Select PCLK Source
						reg_val = SCU->PPCLKSR;
						reg_val |= (1 << 22);
						SCU->PPCLKSR = reg_val;
				}
		}
		// TIMER20/21
		else
		{
				if (clk_sel == TIMER_CLK_MCCLK)
				{
						// Select MCCR Source
						reg_val = SCU->PPCLKSR;
						reg_val &= ~(1 << 20);
						SCU->PPCLKSR = reg_val;

						// Set MCCR Clock
						SCU->MCCR2 &= ~(0x7FF << 0);
						SCU->MCCR2 |= (mcclk_sel << 8);
						SCU->MCCR2 |= (mcclk_div << 0);
				}
				else
				{
						// Select PCLK Source
						reg_val = SCU->PPCLKSR;
						reg_val |= (1 << 20);
						SCU->PPCLKSR = reg_val;
				}
		}

		/* Set Prescaler */
    timer->PREDR = (prescaler - 1) & 0xFFF;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER_SetIRQ(enum timer_id id, enum timer_irq_type type,
                         uint8_t enable)
{
    TIMER_Type *timer;

    DRIVER_ASSERT(id < TIMER_ID_MAX);

		/* Get Register Name */
    timer = TIMER_REG(id);

		/* Set Timer Interrupt */
    switch (type)
    {
				case TIMER_IRQ_COUNT_MATCH:
				case TIMER_IRQ_PWM_PERIOD:
						if (enable)
						{
								timer->CR |= (1 << 5);
						}
						else
						{
								timer->CR &= ~(1 << 5);
						}
						break;
				case TIMER_IRQ_CAPTURE_CAPDR:
						if (enable)
						{
								timer->CR |= (1 << 4);
						}
						else
						{
								timer->CR &= ~(1 << 4);
						}
						break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void TIMER_IRQHandler(enum timer_id id)
{
    struct timer_ctrl_block *tcb;
    TIMER_Type *timer;
    volatile uint32_t event = 0;
    volatile uint8_t status;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER_REG(id);

		/* Get Timer Interrupt Flags */
    status = (timer->CR & 0x0c);
    timer->CR |= 0x0c;	// Clear Flags

    if (tcb->handler)
    {
        if (status & (1 << 2))
        {
            if (tcb->mode == TIMER_MODE_CAPTURE)
            {
                event |= TIMER_EVENT_CAPTURE_CAPDR;
            }
        }
				else
				{
            if (tcb->mode == TIMER_MODE_PERIODIC || tcb->mode == TIMER_MODE_ONESHOT)
            {
                event |= TIMER_EVENT_COUNT_MATCH;
            }
            else if (tcb->mode == TIMER_MODE_PWM && (tcb->pwm_event & TIMER_PWM_EVENT_PERIOD))
            {
                event |= TIMER_EVNET_PWM_PERIOD;
            }
				}

				/* Callback function */
        if (event)
        {
            tcb->handler(event, tcb->context);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void TIMER10_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_10);
}

void TIMER11_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_11);
}

void TIMER12_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_12);
}

void TIMER13_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_13);
}

void TIMER20_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_20);
}

void TIMER21_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER_ID_21);
}

