/**
 *******************************************************************************
 * @file        a31t21x_adc.c
 * @brief       ADC driver
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "a31t21x_adc.h"


struct adc_ctrl_block
{
    adc_irq_handler_t   handler;
    void *              context;
    uint8_t             busy;
		enum adc_trigger_id trigger;
};

static struct adc_ctrl_block g_acb;

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_Init(struct adc_cfg *cfg, adc_irq_handler_t handler, void *context)
{
    struct adc_ctrl_block *acb;

    acb = &g_acb;
		acb->trigger = cfg->trigger;
    acb->handler = handler;
    acb->context = context;

		/* Enable ADC Block */
    SCU->PER2 &= ~(1 << 20);
    SCU->PCER2 &= ~(1 << 20);
    SCU->PER2 |= (1 << 20);
    SCU->PCER2 |= (1 << 20);

		/* Configure ADC Parameter */
		ADC->CR = (1 << 15);						// <ADCEN>
		ADC->CR |= (cfg->trigger << 11)	// <TRIG>
				| (cfg->reference << 10)		// <REFSEL>
				| (1 << 4);									// Clear Flag

		ADC->PREDR = (cfg->prescaler & 0x0F);

		/* Set NVIC */
    NVIC_ClearPendingIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn, cfg->irq_prio);
    NVIC_EnableIRQ(ADC_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_DeInit(void)
{
		/* Disable ADC */
		ADC->CR = 0;

		/* Disable ADC Block */
    SCU->PER2 &= ~(1 << 20);
    SCU->PCER2 &= ~(1 << 20);

		/* Clear NVIC */
    NVIC_DisableIRQ(ADC_IRQn);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_GetData(uint16_t *data)
{
    *data = (ADC->DR & 0x0FFF);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_Start(uint16_t *data, enum adc_io_type io_type)
{
		struct adc_ctrl_block *acb;
		uint32_t timeout;
	
    acb = &g_acb;

    if (acb->busy == 1)
    {
        return DRIVER_ERROR_BUSY;
    }

    acb->busy = 1;

		/* BLOCK MODE */
    if (io_type == ADC_IO_TYPE_BLOCK)
    {
        uint32_t status;

				ADC->CR &= ~(1 << 5);									// ADC interrupt disable

				if (acb->trigger == ADC_TRIGGER_ADST)
				{
						ADC->CR |= (1 << 8);							// Conversion start
				}

				timeout = ADC_READY_TIMEOUT;
        while (1)	
        {
            timeout--;

						HAL_ADC_GetStatus(&status);
						if (status & (1 << 4))						// <ADCIFLAG>
						{
								HAL_ADC_ClearStatus(status);
								break;
						}
						if (timeout == 0)
						{
								return DRIVER_ERROR_TIMEOUT;
						}
        }

        *data = ADC->DR;
				acb->busy = 0;
    }

		/* NON-BLOCK MODE */
    else
    {
				ADC->CR |= (1 << 5);			// ADC interrupt enable

				if (acb->trigger == ADC_TRIGGER_ADST)
				{
						ADC->CR |= (1 << 8);	// Conversion start
				}
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_Stop(void)
{
    ADC->CR &= ~(1UL << 15);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_SelectChannel(uint8_t channel)
{
		ADC->CR &= ~(0xf << 0);
		ADC->CR |= (channel << 0);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_GetStatus(uint32_t *status)
{
    *status = (ADC->CR & (1 << 4));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_ClearStatus(uint32_t status)
{
    ADC->CR |= status;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_ADC_SetIRQ(uint8_t enable)
{
    if (enable)
    {
        ADC->CR |= (1 << 5);
    }
    else
    {
        ADC->CR &= ~(1 << 5);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
void ADC_IRQHandler(void)
{
    struct adc_ctrl_block *acb;
    uint32_t status;
    uint32_t event = 0;

    acb = &g_acb;

		/* Check ADC Interrupt Flag */
		HAL_ADC_GetStatus(&status);
		if (status & (1 << 4))									// <ADCIFLAG>
		{
				HAL_ADC_ClearStatus(status);

				switch (acb->trigger)
				{
						case ADC_TRIGGER_ADST:
								event |= ADC_EVENT_SINGLE;
								break;
						case ADC_TRIGGER_TIMER_ID_10:
						case ADC_TRIGGER_TIMER_ID_11:
						case ADC_TRIGGER_TIMER_ID_12:
						case ADC_TRIGGER_TIMER_ID_30:
								event |= ADC_EVENT_TRIGGER;
								break;
						default :
								break;
				}

				acb->busy = 0;
		}

		/* Callback Function */
    if (event && acb->handler)
    {
        acb->handler(event, acb->context);
    }
}

