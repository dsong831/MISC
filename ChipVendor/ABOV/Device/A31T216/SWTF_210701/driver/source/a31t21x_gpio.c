/**
 *******************************************************************************
 * @file        a31t21x_gpio.c
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

#include "a31t21x_gpio.h"


#define PCU_MOD_Msk							(0x03UL)
#define PCU_TYP_Msk							(0x01UL)
#define PCU_AFSR_Msk						(0x0FUL)
#define PCU_PUPD_Msk						(0x03UL)

#define GPIO_REG_OFFSET 				(0x100)

static __INLINE PCU_Type *GPIO_REG(enum gpio_port port)
{
    return (PCU_Type *)(PA_BASE + (GPIO_REG_OFFSET * port));
}

static __INLINE IRQn_Type GPIO_IRQ_NUM(enum gpio_port port)
{
    return (IRQn_Type)((uint8_t)GPIOAB_IRQn + (port/2));
}

struct gpio_ctrl_block
{
    enum gpio_port      port;
    enum gpio_pin       pin;
    gpio_irq_handler_t  handler;
    void *              context;
};

static struct gpio_ctrl_block g_gcb[CONFIG_GPIO_NUM_IRQ];

static uint8_t gpio_irq_setup[PORT_MAX];

static int8_t GPIO_EmptySlot(enum gpio_port port, enum gpio_pin pin)
{
    uint8_t i;

    for (i = 0; i < CONFIG_GPIO_NUM_IRQ; i++)
    {
        if (g_gcb[i].port == port && g_gcb[i].pin == pin)
        {
            return i;
        }

        if (g_gcb[i].handler == NULL)
        {
            return i;
        }
    }
    return -1;
}

int32_t HAL_GPIO_SetIRQHandler(enum gpio_port port, enum gpio_pin pin, gpio_irq_handler_t handler, void *context)
{
    int8_t i;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

    i = GPIO_EmptySlot(port, pin);
    if (i == -1)
    {
        return DRIVER_ERROR_NO_RESOURCE;
    }

    g_gcb[i].port = port;
    g_gcb[i].pin = pin;
    g_gcb[i].handler = handler;
    g_gcb[i].context = context;

    return DRIVER_ERROR_OK;
}

static void GPIO_SetupIRQ(enum gpio_port port, uint8_t irq_prio)
{
    if (gpio_irq_setup[port])
    {
        return;
    }

    NVIC_ClearPendingIRQ(GPIO_IRQ_NUM(port));
    NVIC_SetPriority(GPIO_IRQ_NUM(port), irq_prio);
    NVIC_EnableIRQ(GPIO_IRQ_NUM(port));

    gpio_irq_setup[port] = 1;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_Init(void)
{
		PCU_Type *pcu;
    uint8_t i;

		/* Peripheral Enable(0:Disable, 1:Enable) */
		SCU->PER1 = SCU->PER1
			| (1<<13)	// GPIOF
			| (1<<12)	// GPIOE
			| (1<<11)	// GPIOD
			| (1<<10)	// GPIOC
			| (1<<9)	// GPIOB
			| (1<<8)	// GPIOA
			;
		/* Peripheral Clock Enable(0:Disable, 1:Enable) */
		SCU->PCER1 = SCU->PCER1
			| (1<<13)	// GPIOF
			| (1<<12)	// GPIOE
			| (1<<11)	// GPIOD
			| (1<<10)	// GPIOC
			| (1<<9)	// GPIOB
			| (1<<8)	// GPIOA
			;

		/* Set Port Output to Low except below functions */
    for (i = 0; i < PORT_MAX; i++)
    {
				// Get Register Name
        pcu = GPIO_REG((enum gpio_port)i);

				// PB3(BOOT)
        if (i == PORTB)
        {
						pcu->OUTDR	= 0;
						pcu->MOD		= 0x55555595UL;
						pcu->AFSR1	= (1<<12);
						pcu->AFSR2	= 0;
        }
				// PE12(nRESET)
        else if (i == PORTE)
        {
						pcu->OUTDR	= 0;
						pcu->MOD		= 0x56555555UL;
						pcu->AFSR1	= 0;
						pcu->AFSR2	= (1<<16);
        }
				// PF7(XOUT),PF6(XIN),PF5(SXOUT),PF4(SXIN),PF1(SWCLK),PF0(SWDIO)
        else if (i == PORTF)
        {
						pcu->OUTDR	= 0;
						pcu->MOD		= 0x5555AA5AUL;
						pcu->AFSR1	= (4<<28)|(4<<24)|(4<<20)|(4<<16)|(3<<4)|(3<<0);
						pcu->AFSR2	= 0;
        }
				// Others
        else
        {
						pcu->OUTDR	= 0;
						pcu->MOD		= 0x55555555UL;
						pcu->AFSR1	= 0;
						pcu->AFSR2	= 0;
        }
        pcu->TYP = 0;		// Push-pull Output
        pcu->PUPD = 0;	// Pull-up/down Disable
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetMode(enum gpio_port port, enum gpio_pin pin, enum gpio_mode mode)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX && mode < GPIO_MODE_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Port Mode Register */
    reg_val = pcu->MOD;
    reg_val &= ~(PCU_MOD_Msk << (pin << 1));
    reg_val |= (mode << (pin << 1));
    pcu->MOD = reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetFunction(enum gpio_port port, enum gpio_pin pin, uint32_t func)
{
    PCU_Type *pcu;
    uint8_t pin_offset;
    uint32_t reg_val;

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Alternative function Register */
    if (pin > 7)
    {
        pin_offset = ((pin - 8) << 2);

        reg_val = pcu->AFSR2;
        reg_val &= ~(PCU_AFSR_Msk << pin_offset);
        reg_val |= (func << pin_offset);

        pcu->AFSR2 = reg_val;
    }
    else
    {
        pin_offset = (pin << 2);

        reg_val = pcu->AFSR1;
        reg_val &= ~(PCU_AFSR_Msk << pin_offset);
        reg_val |= (func << pin_offset);

        pcu->AFSR1 = reg_val;

    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetOutput(enum gpio_port port, enum gpio_pin pin, enum gpio_output_type type)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Output Type Register */
    reg_val = pcu->TYP;
    reg_val &= ~(PCU_TYP_Msk << pin);
    reg_val |= (type << pin);
    pcu->TYP = reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetPull(enum gpio_port port, enum gpio_pin pin, enum gpio_pull pull)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Pull-up/down Register */
    reg_val = pcu->PUPD;
    reg_val &= ~(PCU_PUPD_Msk << (pin << 1));
    reg_val |= (pull << (pin << 1));
    pcu->PUPD = reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetDebounce(enum gpio_port port, enum gpio_pin pin, uint8_t enable)
{
    PCU_Type *pcu;
    uint32_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Debounce Control Register */
    reg_val = pcu->DBCR;
    reg_val &= ~(1 << pin);
    if (enable)
    {
        reg_val |= (1 << pin);
    }
    pcu->DBCR = reg_val;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetDebounceClock(enum gpio_port port, enum gpio_dbc_clk_src clk_src,
                                  uint8_t clk_div)
{
    uint32_t clk_val = 0;

    DRIVER_ASSERT(port < PORT_MAX);
    DRIVER_ASSERT(clk_div > 0);

		/* Set MCCR4 Register */
    switch (port)
    {
        case PORTA:
        case PORTB:
				case PORTC:
            clk_val = SCU->MCCR4;
            clk_val &= ~(0x7FF << 0);
            SCU->MCCR4 = clk_val | (clk_src << 8) | (clk_div << 0);
            break;
        case PORTD:
        case PORTE:
        case PORTF:
            clk_val = SCU->MCCR4;
            clk_val &= ~(0x7FF << 16);
            SCU->MCCR4 = clk_val | (clk_src << 24) | (clk_div << 16);
            break;
        default:
            break;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_SetPin(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Output Bit Set Register */
    pcu->BSR = (1 << pin);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_ClearPin(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Output Bit Clear Register */
    pcu->BCR = (1 << pin);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_ReadPin(enum gpio_port port, enum gpio_pin pin, uint8_t *value)
{
    PCU_Type *pcu;
    uint16_t reg_val;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Get Value From Input Data Register */
    reg_val = pcu->INDR;

    if ((reg_val >> (pin)) & 0x01)
    {
        *value = 1;
    }
    else
    {
        *value = 0;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_WritePort(enum gpio_port port, uint8_t value)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Output Data Register */
    pcu->OUTDR = value;

    return DRIVER_ERROR_OK;
}

int32_t HAL_GPIO_ReadPort(enum gpio_port port, uint8_t *value)
{
    PCU_Type *pcu;

    DRIVER_ASSERT(port < PORT_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Get Input Data Register */
    *value = pcu->INDR;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_EnableIRQ(enum gpio_port port, enum gpio_pin pin,
                           enum gpio_irq_trig trig,
                           enum gpio_irq_mode mode,
                           uint8_t irq_prio)
{
    PCU_Type *pcu;
    uint32_t ier, icr, isr;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Interrupt Enable/Control Register */
    ier = pcu->IER;
    icr = pcu->ICR;
    isr = pcu->ISR;

    ier &= ~(0x03 << (pin << 1));
    icr &= ~(0x03 << (pin << 1));
    isr |= (0x03 << (pin << 1));

    ier |= (trig << (pin << 1));
    icr |= (mode << (pin << 1));

    pcu->IER = ier;
    pcu->ICR = icr;
    pcu->ISR = isr;

		/* Set NVIC */
    GPIO_SetupIRQ(port, irq_prio);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_GPIO_DisableIRQ(enum gpio_port port, enum gpio_pin pin)
{
    PCU_Type *pcu;
    uint32_t ier, icr, isr;

    DRIVER_ASSERT(port < PORT_MAX && pin < PIN_MAX);

		/* Get Register Name */
    pcu = GPIO_REG(port);

		/* Set Interrupt Enable/Control Register */
    ier = pcu->IER;
    icr = pcu->ICR;
    isr = pcu->ISR;

    ier &= ~(0x03 << (pin << 1));
    icr &= ~(0x03 << (pin << 1));
    isr |= (0x03 << (pin << 1));

    pcu->IER = ier;
    pcu->ICR = icr;
    pcu->ISR = isr;

		/* Clear NVIC */
    if (!ier)
    {
        NVIC_DisableIRQ(GPIO_IRQ_NUM(port));
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void GPIO_Handler(enum gpio_port port)
{
    PCU_Type *pcu;
    uint32_t isr;
    uint8_t pin;
    uint8_t i;
    struct gpio_ctrl_block *gcb;

		/* Get Register Name */
    pcu = GPIO_REG(port);
    isr = pcu->ISR;

    gcb = &g_gcb[port];

    for (pin = 0; pin < PIN_MAX; pin++)
    {
        if ((isr >> (pin << 1)) & 0x03)
        {
            for (i = 0; i < CONFIG_GPIO_NUM_IRQ; i++)
            {
                gcb = &g_gcb[i];
                if (gcb->port == port && gcb->pin == pin)
                {
                    if (gcb->handler)
                    {
                        gcb->handler(gcb->context);
                    }
                    break;
                }
            }
        }
    }

		/* Clear Interrupt Flags */
    pcu->ISR = isr;
}

//////////////////////////////////////////////////////////////////////////////////////
void GPIOAB_IRQHandler(void)
{
    GPIO_Handler(PORTA);
		GPIO_Handler(PORTB);
}

void GPIOCD_IRQHandler(void)
{
    GPIO_Handler(PORTC);
		GPIO_Handler(PORTD);
}

void GPIOE_IRQHandler(void)
{
    GPIO_Handler(PORTE);
}

void GPIOF_IRQHandler(void)
{
    GPIO_Handler(PORTF);
}

