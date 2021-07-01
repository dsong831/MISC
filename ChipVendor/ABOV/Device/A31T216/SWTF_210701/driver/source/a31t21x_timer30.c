/**
 *******************************************************************************
 * @file        a31t21x_timer30.c
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

#include "a31t21x_timer30.h"


#define TIMER_REG_OFFSET    0x100

static __INLINE TIMER3n_Type *TIMER3n_REG(enum timer30_id id)
{
		return (TIMER3n_Type *)(TIMER30_BASE + (TIMER_REG_OFFSET * id));
}

static __INLINE IRQn_Type TIMER3n_IRQ_NUM(enum timer30_id id)
{
		return (IRQn_Type)((uint8_t)TIMER30_IRQn + id);
}

struct timer30_ctrl_block
{
    uint8_t									mode;
    uint8_t									output_mode;
    uint8_t         	  	  pwm_event;
    timer30_irq_handler_t		handler;
    void *									context;
};

static struct timer30_ctrl_block g_tcb[TIMER30_ID_MAX];

//////////////////////////////////////////////////////////////////////////////////////
static int32_t TIMER30_Configure(struct timer30_ctrl_block *tcb,
                               TIMER3n_Type *timer,
                               struct timer30_cfg *cfg)
{
    uint32_t cfg_val = 0;

		/* Configure Timer Operation Mode */
    switch (cfg->mode)
    {
        case TIMER30_MODE_INTERVAL:
						cfg_val |= (TIMER30_MODE_INTERVAL << 12);
						tcb->mode = cfg->mode;
						break;
        case TIMER30_MODE_CAPTURE:
						cfg_val |= (TIMER30_MODE_CAPTURE << 12);
						tcb->mode = cfg->mode;
						break;
        case TIMER30_MODE_BACKTOBACK:
						cfg_val |= (TIMER30_MODE_BACKTOBACK << 12);
						tcb->mode = cfg->mode;
						break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Configure Timer Output Mode */
    switch (cfg->output_mode)
    {
        case TIMER30_OUTPUT_MODE_6_CH:
						cfg_val &= ~(1 << 10);
						tcb->output_mode = cfg->output_mode;
						break;
        case TIMER30_OUTPUT_MODE_FORCE_A:
						cfg_val |= (1 << 10);
						tcb->output_mode = cfg->output_mode;
						break;
        default:
            return DRIVER_ERROR_PARAMETER;
    }

		/* Configure Timer Clock Selection */
    switch (cfg->clk_sel)
    {
        case TIMER30_CLK_MCCLK:
        case TIMER30_CLK_PCLK:
						cfg_val &= ~(1 << 14);
						break;
        case TIMER30_CLK_TNIO:
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

		/* Enable PWM output */
		timer->OUTCR = (0xe06cUL << 16) | (0x3f << 8);

    timer->CR = cfg_val;
    timer->PREDR = (cfg->prescaler - 1) & 0xFFF;
		timer->CR |= (1 << 0);	// Clear Counter

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_Init(enum timer30_id id, struct timer30_cfg *cfg,
                       timer30_irq_handler_t handler,
                       void *context)
{
    struct timer30_ctrl_block *tcb;
    TIMER3n_Type *timer;
    int32_t ret;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER3n_REG(id);

    memset(tcb, 0, sizeof(struct timer30_ctrl_block));

    tcb->handler = handler;
    tcb->context = context;

		/* Timer Block Enable */
		if (id == TIMER30_ID_30)
		{
				SCU->PER1 &= ~(1<<24);
				SCU->PCER1 &= ~(1<<24);
				SCU->PER1 |= (1<<24);
				SCU->PCER1 |= (1<<24);
		}

		/* Configure Timer Parameter */
    ret = TIMER30_Configure(tcb, timer, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_TIMER30_Deinit(id);
        return ret;
    }

		/* Set NVIC */
    NVIC_ClearPendingIRQ(TIMER3n_IRQ_NUM(id));
    NVIC_SetPriority(TIMER3n_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(TIMER3n_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_Deinit(enum timer30_id id)
{
    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Timer1n Block Disable */
		if (id == TIMER30_ID_30)
		{
				SCU->PER1 &= ~(1<<24);
				SCU->PCER1 &= ~(1<<24);
		}

		/* Clear NVIC */
    NVIC_DisableIRQ(TIMER3n_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_GetStatus(enum timer30_id id, uint8_t *status)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Get Interrupt Flags */
    *status = timer->CR & 0x0c;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_StartPWM(enum timer30_id id, enum timer30_pwm_start_level level,
                           uint16_t a_duty, uint16_t b_duty, uint16_t c_duty, uint16_t period,
                           uint8_t event)
{
    struct timer30_ctrl_block *tcb;
    TIMER3n_Type *timer;
		uint32_t reg_val;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER3n_REG(id);

    DRIVER_ASSERT(id < TIMER30_ID_MAX);
    DRIVER_ASSERT(tcb->mode == TIMER30_MODE_INTERVAL || tcb->mode == TIMER30_MODE_BACKTOBACK);

		/* Check PWM Event */
    if (event == TIMER30_PWM_EVENT_PERIOD)
    {
        timer->INTCR |= (1 << 3);	// Enable Period Interrupt
    }
    else if (event == TIMER30_PWM_EVENT_A_DUTY)
    {
        timer->INTCR |= (1 << 2);	// Enable A-Match Interrupt
    }
    else if (event == TIMER30_PWM_EVENT_B_DUTY)
    {
        timer->INTCR |= (1 << 1);	// Enable B-Match Interrupt
    }
    else if (event == TIMER30_PWM_EVENT_C_DUTY)
    {
        timer->INTCR |= (1 << 0);	// Enable C-Match Interrupt
    }
    tcb->pwm_event = event;

		/* Set PWM Period & Duty */
    timer->PDR = period;
    timer->ADR = a_duty;
    timer->BDR = b_duty;
    timer->CDR = c_duty;

		/* Set PWM Start Level */
    if (level == TIMER30_PWM_START_LOW)
    {
				reg_val = timer->OUTCR;
        reg_val |= (1 << 15);		// PWM30xB Low level start
				reg_val &= ~(1 << 14);	// PWM30xA Low level start
				timer->OUTCR = (0xe06cUL << 16) | (reg_val);
    }
    else
    {
				reg_val = timer->OUTCR;
        reg_val &= ~(1 << 15);	// PWM30xB High level start
				reg_val |= (1 << 14);		// PWM30xA High level start
				timer->OUTCR = (0xe06cUL << 16) | (reg_val);
    }
		
		/* Counter clear and start */
    timer->CR |= (1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_StartCapture(enum timer30_id id, enum timer30_capture_clear clear)
{
    struct timer30_ctrl_block *tcb;
    TIMER3n_Type *timer;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER3n_REG(id);

    DRIVER_ASSERT(id < TIMER30_ID_MAX);
    DRIVER_ASSERT(tcb->mode == TIMER30_MODE_CAPTURE);

		/* Set Capture Polarity */
		timer->CR &= ~(3 << 6);
    timer->CR |= (clear << 6);

		/* Counter clear and start */
		timer->PDR = 0xFFFF;
    timer->CR |= (1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_Stop(enum timer30_id id)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Disable timer operation */
    timer->CR &= ~(1 << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_GetCount(enum timer30_id id, uint16_t *count)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

    *count = timer->CNT;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_ClearCount(enum timer30_id id)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

    timer->CR = (1 << 0);		// Clear Timer Counter

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_GetCaptureCount(enum timer30_id id, uint16_t *count, enum timer30_capture_reg reg)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Get Capture Data */
    if (reg == TIMER30_CAPTURE_REG_CAPDR)
    {
        *count = timer->CAPDR;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_SetAdcTrigger(enum timer30_id id, enum timer30_adc_trigger_sig sig, uint16_t counter, uint8_t enable)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Set ADC Trigger Signal */
    if (enable)
    {
        timer->ADTCR |= (1 << sig);
				timer->ADTDR = counter;
    }
		else
		{
        timer->ADTCR &= ~(1 << sig);
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_SetDelayTime(enum timer30_id id, enum timer30_delay_time_position delay_position, uint16_t delay_data, uint8_t enable)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Set Delay Time */
    if (enable)
    {
				timer->CR &= ~(3 << 8);
				timer->CR |= (delay_position << 8);	// Set delay position

				timer->CR |= (1 << 9);							// Enable delay time
				timer->DLY = delay_data;
    }
		else
		{
				timer->CR &= ~(1 << 9);							// Disable delay time
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_SetHIZ(enum timer30_id id, enum timer30_hi_z_edge_sel edge, uint8_t enable)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Set High-Impedance Control */
    if (enable)
    {
				timer->HIZCR &= ~(1 << 2);
				timer->HIZCR |= (edge << 2);	// High-Impedance Edge Selection

				timer->HIZCR |= (1 << 7);			// Enable to control hi-impedance
    }
		else
		{
				timer->HIZCR &= ~(1 << 7);		// Disable to control hi-impedance
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_SetClock(enum timer30_id id, enum timer30_clk_sel clk_sel, uint16_t prescaler,
                           enum timer30_mcclk_sel mcclk_sel, uint8_t mcclk_div)
{
		uint32_t reg_val;
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Check range of prescaler */
    if (prescaler > 4095)
    {
        return DRIVER_ERROR_PARAMETER;
    }

		/* Set External Clock Source */
		if (clk_sel == TIMER30_CLK_MCCLK)
		{
				// Select MCCR Source
				reg_val = SCU->PPCLKSR;
				reg_val &= ~(1 << 17);
				SCU->PPCLKSR = reg_val;

				// Set MCCR Clock
				SCU->MCCR2 &= ~(0x7FF << 16);
				SCU->MCCR2 |= (mcclk_sel << 24);
				SCU->MCCR2 |= (mcclk_div << 16);
		}
		else
		{
				// Select PCLK Source
				reg_val = SCU->PPCLKSR;
				reg_val |= (1 << 17);
				SCU->PPCLKSR = reg_val;
		}

		/* Set Prescaler */
    timer->PREDR = (prescaler - 1) & 0xFFF;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_TIMER30_SetIRQ(enum timer30_id id, enum timer30_irq_type type,
                         uint8_t enable)
{
    TIMER3n_Type *timer;

    DRIVER_ASSERT(id < TIMER30_ID_MAX);

		/* Get Register Name */
    timer = TIMER3n_REG(id);

		/* Set Timer Interrupt */
		if (enable)
		{
				timer->INTCR |= (1 << type);	// Enable Interrupt
		}
		else
		{
				timer->INTCR &= ~(1 << type);	// Disable Interrupt
		}

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void TIMER_IRQHandler(enum timer30_id id)
{
    struct timer30_ctrl_block *tcb;
    TIMER3n_Type *timer;
    volatile uint32_t event = 0;
    volatile uint8_t status;

		/* Get Register Name */
    tcb = &g_tcb[id];
    timer = TIMER3n_REG(id);

		/* Get Timer Interrupt Flags */
		status = timer->INTFLAG;
		timer->INTFLAG = status;	// Clear Flags

    if (tcb->handler)
    {
				if ((status & (1 << TIMER30_IRQ_C_MATCH)) && (timer->INTCR & (1 << TIMER30_IRQ_C_MATCH)))
				{
						event |= TIMER30_EVENT_C_MATCH;
				}
				if ((status & (1 << TIMER30_IRQ_B_MATCH)) && (timer->INTCR & (1 << TIMER30_IRQ_B_MATCH)))
				{
						event |= TIMER30_EVENT_B_MATCH;
				}
				if ((status & (1 << TIMER30_IRQ_A_MATCH)) && (timer->INTCR & (1 << TIMER30_IRQ_A_MATCH)))
				{
						event |= TIMER30_EVENT_A_MATCH;
				}
				if ((status & (1 << TIMER30_IRQ_PERIOD_MATCH)) && (timer->INTCR & (1 << TIMER30_IRQ_PERIOD_MATCH)))
				{
						event |= TIMER30_EVENT_PERIOD_MATCH;
				}
				if ((status & (1 << TIMER30_IRQ_BOTTOM_MATCH)) && (timer->INTCR & (1 << TIMER30_IRQ_BOTTOM_MATCH)))
				{
						event |= TIMER30_EVENT_BOTTOM_MATCH;
				}
				if ((status & (1 << TIMER30_IRQ_CAPTURE_CAPDR)) && (timer->INTCR & (1 << TIMER30_IRQ_CAPTURE_CAPDR)))
				{
						event |= TIMER30_EVENT_CAPTURE_CAPDR;
				}
				if ((status & (1 << TIMER30_IRQ_HI_Z)) && (timer->INTCR & (1 << TIMER30_IRQ_HI_Z)))
				{
						event |= TIMER30_EVENT_HI_Z;
				}

				/* Callback function */
        if (event)
        {
            tcb->handler(event, tcb->context);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
void TIMER30_IRQHandler(void)
{
    TIMER_IRQHandler(TIMER30_ID_30);
}

