/**
 *******************************************************************************
 * @file        a31t21x_i2c.c
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

#include "a31t21x_i2c.h"

#define I2C_MASTER_OPERATION        0
#define I2C_SLAVE_OPERATION         1

#define I2C_MASTER_TX_ADDR_ACK      0x87
#define I2C_MASTER_TX_DATA_ACK      0x47
#define I2C_MASTER_RX_ADDR_ACK      0x85
#define I2C_MASTER_RX_DATA_ACK      0x45
#define I2C_MASTER_RX_DATA_NOACK    0x44

#define I2C_SLAVE_TX_SEL_ACK        0x17
#define I2C_SLAVE_TX_DATA_NOACK     0x46
#define I2C_SLAVE_TX_DATA_ACK       0x47
#define I2C_SLAVE_TX_GC_ACK         0x97
#define I2C_SLAVE_RX_SEL_ACK        0x15
#define I2C_SLAVE_RX_DATA_ACK       0x45
#define I2C_SLAVE_RX_GC_ACK         0x95

#define I2C_TX_COMPLETE             0x22
#define I2C_RX_COMPLETE             0x20

#define I2C_REG_OFFSET              0x100

static __INLINE I2C_Type *I2C_REG(enum i2c_id id)
{
    return (I2C_Type *)(I2C0_BASE + (I2C_REG_OFFSET * id));
}

static __INLINE IRQn_Type I2C_IRQ_NUM(enum i2c_id id)
{
		if (id == 0)
		{
				return (IRQn_Type)((uint8_t)I2C0_IRQn);
		}
		else
		{
				return (IRQn_Type)((uint8_t)I2C0_IRQn + id + 3);
		}

}

struct i2c_ctrl_block
{
    i2c_irq_handler_t   handler;
    void *              context;
    uint32_t            tx_busy : 1;
    uint32_t            rx_busy : 1;
    uint8_t *           tx_reg;
    uint8_t *           tx_buf;
    uint8_t *           rx_buf;
    uint8_t             tx_reg_len;
    uint32_t            tx_len;
    uint32_t            rx_len;
    volatile uint32_t   tx_count;
    volatile uint32_t   rx_count;
    uint8_t             mode;
    uint8_t             slave_addr;
    uint8_t             burst_rx;
    uint8_t             burst_tx;
    uint32_t            error;
};

static struct i2c_ctrl_block g_icb[I2C_ID_MAX];

static int I2C_ErrorCheck(uint8_t status)
{
    if (status & (1 << 3))																// <MLOST>
    {
        return DRIVER_ERROR_I2C_MASTERSHIP_LOST;
    }
    else if ((status & (1 << 1)) && !(status & (1 << 0)))	// <TMODE>, <RXACK>
    {
        return DRIVER_ERROR_I2C_NO_ACK;
    }

    return DRIVER_ERROR_I2C_HW;
}

static void I2C_IOTypeCheck(I2C_Type *i2c, enum i2c_io_type io_type)
{
    if (io_type == I2C_IO_TYPE_NON_BLOCK)
    {
        i2c->CR |= (1 << 5);			// <IEN>
    }
    else
    {
        i2c->CR &= ~(1 << 5);			// <IEN>
    }
}

static void I2C_OperationStop(struct i2c_ctrl_block *icb, I2C_Type *i2c)
{
    i2c->CR |= (1 << 1);					// <STOP>
    i2c->ST = 0x800000FF;
    while (!(i2c->CR & (1 << 4)))	// <IFLAG>
    {
    }

    icb->tx_busy = 0;
    icb->rx_busy = 0;
}

static uint8_t I2C_OperationCompleteWait(struct i2c_ctrl_block *icb, I2C_Type *i2c)
{
    uint8_t status;

    i2c->ST = 0x800000FF;
    while (!(i2c->CR & (1 << 4)))	// <IFLAG>
    {
    }

    status = i2c->ST;

    if (icb->mode == I2C_MASTER_OPERATION)
    {
        switch (status)
        {
            case I2C_MASTER_TX_ADDR_ACK:
            case I2C_MASTER_TX_DATA_ACK:
            case I2C_MASTER_RX_ADDR_ACK:
            case I2C_MASTER_RX_DATA_ACK:
            case I2C_MASTER_RX_DATA_NOACK:
                break;
            default:
                I2C_OperationStop(icb, i2c);
                break;
        }
    }

    return status;
}

//////////////////////////////////////////////////////////////////////////////////////
static int I2C_Configure(struct i2c_ctrl_block *icb,
                         I2C_Type *i2c,
                         struct i2c_cfg *cfg)
{
    uint16_t scll = 0;
    uint16_t sclh = 0;

		i2c->CR |= (1 << 7);	// <EN>
    i2c->CR |= (1 << 3);	// <ACKEN>

    switch (cfg->freq)
    {
        case I2C_FREQ_400KHZ:
            scll = ((SystemPeriClock/800000) - 2) / 4;
            sclh = ((SystemPeriClock/800000) - 2) / 4;
            break;
        case I2C_FREQ_200KHZ:
            scll = ((SystemPeriClock/400000) - 2) / 4;
            sclh = ((SystemPeriClock/400000) - 2) / 4;
            break;
        case I2C_FREQ_100KHZ:
            scll = ((SystemPeriClock/200000) - 2) / 4;
            sclh = ((SystemPeriClock/200000) - 2) / 4;
            break;
        default:
            DRIVER_ASSERT(0);
    }

    i2c->SDHR = 3;
    i2c->SCHR = sclh;
    i2c->SCLR = scll;

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_Init(enum i2c_id id, struct i2c_cfg *cfg,
                     i2c_irq_handler_t handler,
                     void *context)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    int32_t ret;

    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(cfg != NULL);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    memset(icb, 0, sizeof(struct i2c_ctrl_block));

		/* Enable I2C Block */
	  SCU->PER2 &= ~(1 << (4 + id));
    SCU->PCER2 &= ~(1 << (4 + id));
    SCU->PER2 |= (1 << (4 + id));
    SCU->PCER2 |= (1 << (4 + id));

    icb->handler = handler;
    icb->context = context;

		/* Configure I2C Parameter */
    ret = I2C_Configure(icb, i2c, cfg);
    if (ret != DRIVER_ERROR_OK)
    {
        HAL_I2C_Deinit(id);
        return ret;
    }

		/* Set NVIC */
    NVIC_ClearPendingIRQ(I2C_IRQ_NUM(id));
    NVIC_SetPriority(I2C_IRQ_NUM(id), cfg->irq_prio);
    NVIC_EnableIRQ(I2C_IRQ_NUM(id));

    return ret;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_Deinit(enum i2c_id id)
{
    DRIVER_ASSERT(id < I2C_ID_MAX);

		/* Disable I2C Block */
    SCU->PER2 &= ~(1 << (4 + id));
    SCU->PCER2 &= ~(1 << (4 + id));

		/* Clear NVIC */
    NVIC_DisableIRQ(I2C_IRQ_NUM(id));

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_MasterTransmit(enum i2c_id id, uint8_t slave_addr, uint8_t *out, uint32_t len,
                               enum i2c_io_type io_type)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint8_t status;

    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    if (icb->tx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Parameter */
    icb->tx_busy = 1;
    icb->tx_buf = out;
    icb->tx_len = len;
    icb->rx_buf = NULL;
    icb->rx_len = 0;
    icb->tx_count = 0;
    icb->mode = I2C_MASTER_OPERATION;

    I2C_IOTypeCheck(i2c, io_type);
		i2c->ST = 0x800000FF;
    i2c->CR |= (1 << 3);	// <ACKEN>

		/* Set Slave Address */
    i2c->DR = slave_addr << 1;

		/* Start I2C */
    i2c->CR |= (1 << 0);		// Jump to IQR when the case of NON-BLOCK TX

		/* BLOCK TX */
    if (io_type == I2C_IO_TYPE_BLOCK)
    {
				while (!(i2c->CR & (1 << 4)))	// <IFLAG>
				{
				}

				status = i2c->ST;
        if (status != I2C_MASTER_TX_ADDR_ACK)
        {
            return I2C_ErrorCheck(status);
        }

        while (icb->tx_len > icb->tx_count)
        {
            i2c->DR = icb->tx_buf[icb->tx_count++];
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_TX_DATA_ACK)
            {
                return I2C_ErrorCheck(status);
            }
        }

        I2C_OperationStop(icb, i2c);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_MasterReceive(enum i2c_id id, uint8_t slave_addr, uint8_t *in, uint32_t len,
                              enum i2c_io_type io_type)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint8_t status;
    uint32_t tmp;
    
    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    if (icb->rx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Parameter */
    icb->rx_busy = 1;
    icb->tx_buf = NULL;
    icb->tx_len = 0;
    icb->rx_buf = in;
    icb->rx_len = len;
    icb->rx_count = 0;
    icb->mode = I2C_MASTER_OPERATION;

    I2C_IOTypeCheck(i2c, io_type);
    i2c->ST = 0x800000FF;
    i2c->CR |= (1 << 3);	// <ACKEN>

		/* Set Slave Address */
    i2c->DR = slave_addr << 1 | 0x01;

		/* Start I2C */
    i2c->CR |= (1 << 0);		// Jump to IQR when the case of NON-BLOCK TX

		/* BLOCK TX */
    if (io_type == I2C_IO_TYPE_BLOCK)
    {
				while (!(i2c->CR & (1 << 4)))	// <IFLAG>
				{
				}

				status = i2c->ST;
        if (status != I2C_MASTER_RX_ADDR_ACK)
        {
            return I2C_ErrorCheck(status);
        }

        while (icb->rx_len > icb->rx_count)
        {
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_RX_DATA_ACK && status != I2C_MASTER_RX_DATA_NOACK)
            {
                return I2C_ErrorCheck(status);
            }
            tmp = i2c->DR;
            icb->rx_buf[icb->rx_count++] = tmp;

						// Last data has NoAck status
            if (icb->rx_count + 1 == icb->rx_len)
            {
                i2c->CR &= ~(1 << 3);	// <ACKEN>
            }
        }

        I2C_OperationStop(icb, i2c);
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_MasterBurstTransmit(enum i2c_id id, uint8_t slave_addr,
                                    uint8_t *reg, uint8_t reg_len,
                                    uint8_t *out, uint32_t len,
                                    enum i2c_io_type io_type)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint8_t status;

    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    if (icb->tx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Parameter */
    icb->tx_busy = 1;
    icb->tx_reg = reg;
    icb->tx_reg_len = reg_len;
    icb->tx_buf = out;
    icb->tx_len = len;
    icb->tx_count = 0;
    icb->burst_tx = 1;
    icb->mode = I2C_MASTER_OPERATION;

    I2C_IOTypeCheck(i2c, io_type);
    i2c->ST = 0x800000FF;
    i2c->CR |= (1 << 3);	// <ACKEN>

		/* Set Slave Address */
    i2c->DR = slave_addr << 1;

		/* Start I2C */
    i2c->CR |= (1 << 0);		// Jump to IQR when the case of NON-BLOCK TX

		/* BLOCK TX */
    if (io_type == I2C_IO_TYPE_BLOCK)
    {
				while (!(i2c->CR & (1 << 4)))	// <IFLAG>
				{
				}

				status = i2c->ST;
        if (status != I2C_MASTER_TX_ADDR_ACK)
        {
            return I2C_ErrorCheck(status);
        }

        while (icb->tx_reg_len > icb->tx_count)
        {
            i2c->DR = icb->tx_reg[icb->tx_count++];
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_TX_DATA_ACK)
            {
                return I2C_ErrorCheck(status);
            }
        }

        icb->tx_count = 0;
        while (icb->tx_len > icb->tx_count)
        {
            i2c->DR = icb->tx_buf[icb->tx_count++];
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_TX_DATA_ACK)
            {
                return I2C_ErrorCheck(status);
            }
        }

        I2C_OperationStop(icb, i2c);
    }
    else
    {
        icb->slave_addr = slave_addr;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_MasterBurstReceive(enum i2c_id id, uint8_t slave_addr,
                                   uint8_t *reg, uint8_t reg_len,
                                   uint8_t *in, uint32_t len,
                                   enum i2c_io_type io_type)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint8_t status;
    uint32_t tmp;
    
    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    if (icb->tx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Parameter */
    icb->tx_busy = 1;
    icb->tx_reg = reg;
    icb->tx_reg_len = reg_len;
    icb->tx_count = 0;
    icb->rx_buf = in;
    icb->rx_len = len;
    icb->rx_count = 0;
    icb->burst_rx = 1;
    icb->mode = I2C_MASTER_OPERATION;

    I2C_IOTypeCheck(i2c, io_type);
    i2c->ST = 0x800000FF;
    i2c->CR |= (1 << 3);	// <ACKEN>

		/* Set Slave Address */
    i2c->DR = slave_addr << 1;

		/* Start I2C */
    i2c->CR |= (1 << 0);		// Jump to IQR when the case of NON-BLOCK TX

		/* BLOCK TX */
    if (io_type == I2C_IO_TYPE_BLOCK)
    {
				while (!(i2c->CR & (1 << 4)))	// <IFLAG>
				{
				}

				status = i2c->ST;
        if (status != I2C_MASTER_TX_ADDR_ACK)
        {
            return I2C_ErrorCheck(status);
        }

        while (icb->tx_reg_len > icb->tx_count)
        {
            i2c->DR = icb->tx_reg[icb->tx_count++];
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_TX_DATA_ACK)
            {
                return I2C_ErrorCheck(status);
            }
        }

				// Re-start Process
        i2c->DR = slave_addr << 1 | 0x01;
        i2c->CR |= (1 << 0);
				i2c->ST = 0x800000FF;
				while (!(i2c->CR & (1 << 4)))	// <IFLAG>
				{
				}

				status = i2c->ST;
        if (status != I2C_MASTER_RX_ADDR_ACK)
        {
            return I2C_ErrorCheck(status);
        }

        while (icb->rx_len > icb->rx_count)
        {
            status = I2C_OperationCompleteWait(icb, i2c);
            if (status != I2C_MASTER_RX_DATA_ACK && status != I2C_MASTER_RX_DATA_NOACK)
            {
                return I2C_ErrorCheck(status);
            }
            tmp = i2c->DR;
            icb->rx_buf[icb->rx_count++] = tmp;

						// Last data has NoAck status
            if (icb->rx_count + 1 == icb->rx_len)
            {
                i2c->CR &= ~(1 << 3);	// <ACKEN>
            }
        }

        I2C_OperationStop(icb, i2c);
    }
    else
    {
        icb->slave_addr = slave_addr;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_SlaveTransmit(enum i2c_id id, uint8_t *out, uint32_t len,
                              enum i2c_io_type io_type)
{
    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

    return HAL_I2C_SlaveTransfer(id, out, len, NULL, 0, io_type);
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_SlaveReceive(enum i2c_id id, uint8_t *in, uint32_t len,
                             enum i2c_io_type io_type)
{
    DRIVER_ASSERT(id < I2C_ID_MAX);
    DRIVER_ASSERT(len > 0);

    return HAL_I2C_SlaveTransfer(id, NULL, 0, in, len, io_type);
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_SlaveTransfer(enum i2c_id id, uint8_t *out, uint32_t out_len,
                              uint8_t *in, uint32_t in_len, enum i2c_io_type io_type)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    uint8_t status;
    uint8_t complete = 0;
    uint32_t tmp;
    
    DRIVER_ASSERT(id < I2C_ID_MAX);

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    if (icb->tx_busy || icb->rx_busy)
    {
        return DRIVER_ERROR_BUSY;
    }

		/* Set Parameter */
    if (out_len)
    {
        icb->tx_busy = 1;
    }

    if (in_len)
    {
        icb->rx_busy = 1;
    }

    icb->tx_buf = out;
    icb->tx_len = out_len;
    icb->tx_count = 0;
    icb->rx_buf = in;
    icb->rx_len = in_len;
    icb->rx_count = 0;
    icb->mode = I2C_SLAVE_OPERATION;

    I2C_IOTypeCheck(i2c, io_type);

    i2c->CR |= (1 << 3);	// <ACKEN>

		/* BLOCK TX */
    if (io_type == I2C_IO_TYPE_BLOCK)
    {
        while (!complete)
        {
            status = I2C_OperationCompleteWait(icb, i2c);
            switch (status)
            {
                case I2C_SLAVE_TX_SEL_ACK:
                case I2C_SLAVE_TX_DATA_NOACK:
                case I2C_SLAVE_TX_DATA_ACK:
                case I2C_SLAVE_TX_GC_ACK:
                    if (icb->tx_len > icb->tx_count)
                    {
                        i2c->DR = icb->tx_buf[icb->tx_count++];
                    }
                    break;
                case I2C_SLAVE_RX_SEL_ACK:
                    break;
                case I2C_SLAVE_RX_DATA_ACK:
                case I2C_SLAVE_RX_GC_ACK:
                    if (icb->rx_len > icb->rx_count)
                    {
                        tmp = i2c->DR;
                        icb->rx_buf[icb->rx_count++] = tmp;
                    }
                    break;
                case I2C_TX_COMPLETE:
                    if (icb->rx_count == icb->rx_len)
                    {
                        complete = 1;
                    }
                    break;
                case I2C_RX_COMPLETE:
                    if (icb->tx_count == icb->tx_len)
                    {
                        complete = 1;
                    }
                    break;
                default:
                    return I2C_ErrorCheck(status);
            }
        }

        icb->tx_busy = 0;
        icb->rx_busy = 0;
    }

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
int32_t HAL_I2C_SetOwnAddress(enum i2c_id id, uint8_t own_addr)
{
    I2C_Type *i2c;

    DRIVER_ASSERT(id < I2C_ID_MAX);

		/* Get I2C Register */
    i2c = I2C_REG(id);

		/* Set Slave Own Address */
    i2c->SAR1 = (own_addr << 1) | (1 << 0);
    i2c->SAR2 = (own_addr << 1) | (1 << 0);

    return DRIVER_ERROR_OK;
}

//////////////////////////////////////////////////////////////////////////////////////
static void I2C_MasterIRQHandler(enum i2c_id id)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    volatile uint32_t event = 0;
    volatile uint8_t status;
    uint32_t tmp;

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    status = i2c->ST;

    switch (status)
    {
        case I2C_MASTER_TX_ADDR_ACK:
            if (icb->burst_rx || icb->burst_tx)
            {
                i2c->DR = icb->tx_reg[icb->tx_count++];
            }
            else
            {
                i2c->DR = icb->tx_buf[icb->tx_count++];
            }
            break;
        case I2C_MASTER_TX_DATA_ACK:
            if (icb->burst_rx || icb->burst_tx)
            {
                if (icb->tx_count == icb->tx_reg_len)
                {
                    if (icb->burst_rx)
                    {
                        icb->burst_rx = 0;
                        i2c->DR = icb->slave_addr << 1 | 0x01;
                        i2c->CR |= (1 << 0);	// <STARTC>
                    }
                    else
                    {
                        icb->burst_tx = 0;
                        icb->tx_count = 0;
                        i2c->DR = icb->tx_buf[icb->tx_count++];
                    }
                }
                else
                {
                    i2c->DR = icb->tx_reg[icb->tx_count++];
                }
            }
            else
            {
                if (icb->tx_len == icb->tx_count)
                {
                    i2c->CR |= (1 << 1);			// <STOPC>
                    icb->tx_busy = 0;
                }
                else
                {
                    i2c->DR = icb->tx_buf[icb->tx_count++];
                }
            }
            break;
        case I2C_MASTER_RX_ADDR_ACK:
            break;
        case I2C_MASTER_RX_DATA_ACK:
        case I2C_MASTER_RX_DATA_NOACK:
            tmp = i2c->DR;
            icb->rx_buf[icb->rx_count++] = tmp;
            if (icb->rx_count + 1 == icb->rx_len)
            {
                i2c->CR &= ~(1 << 3);				// <ACKEN>
            }
            if (icb->rx_count == icb->rx_len)
            {
                i2c->CR |= (1 << 1);				// <ACKEN>
                icb->rx_busy = 0;
            }
            break;
        case I2C_TX_COMPLETE:
            if (icb->error)
            {
                event = icb->error;
                icb->error = 0;
            }
            else
            {
                event |= I2C_EVENT_TRASMIT_DONE;
            }
            break;
        case I2C_RX_COMPLETE:
            if (icb->error)
            {
                event = icb->error;
                icb->error = 0;
            }
            else
            {
                event |= I2C_EVENT_RECEIVE_DONE;
            }
            break;
        default:
            i2c->CR |= (1 << 1);		// <STOPC>
            icb->tx_busy = 0;
            icb->rx_busy = 0;

            if (status & (1 << 3))	// <MLOST>
            {
                icb->error |= I2C_EVENT_MASTERSHIP_LOSS;
            }
            if ((status & (1 << 1)) && !(status & (1 << 0)))	// <TMOD> && <RXACK>
            {
                icb->error |= I2C_EVENT_NO_ACK;
            }
            if (!event)
            {
                icb->error |= I2C_EVENT_HW_ERR;
            }
            break;
    }

		/* Callback Function */
    if (icb->handler && event)
    {
        i2c->CR &= ~(1 << 5);	// Disable i2c interrupt
        icb->handler(event, icb->context);
    }

    i2c->ST = 0x800000FF;
}

//////////////////////////////////////////////////////////////////////////////////////
static void I2C_SlaveIRQHandler(enum i2c_id id)
{
    struct i2c_ctrl_block *icb;
    I2C_Type *i2c;
    volatile uint32_t event = 0;
    volatile uint8_t status;
    uint32_t tmp;

		/* Get I2C Register */
    icb = &g_icb[id];
    i2c = I2C_REG(id);

    status = i2c->ST;

    switch (status)
    {
        case I2C_SLAVE_TX_SEL_ACK:
            if (icb->tx_len > icb->tx_count)
            {
                i2c->DR = icb->tx_buf[icb->tx_count++];
            }
            break;
        case I2C_SLAVE_TX_DATA_NOACK:
        case I2C_SLAVE_TX_DATA_ACK:
        case I2C_SLAVE_TX_GC_ACK:
            if (icb->tx_len > icb->tx_count)
            {
                i2c->DR = icb->tx_buf[icb->tx_count++];
                i2c->CR &= ~(1 << 3);	// <ACKEN>
                if (icb->tx_len == icb->tx_count)
                {
                    icb->tx_busy = 0;
                }
            }
            break;
        case I2C_SLAVE_RX_SEL_ACK:
            break;
        case I2C_SLAVE_RX_DATA_ACK:
        case I2C_SLAVE_RX_GC_ACK:
            if (icb->rx_len > icb->rx_count)
            {
                tmp = i2c->DR;
                icb->rx_buf[icb->rx_count++] = tmp;
                if (icb->rx_len == icb->rx_count)
                {
                    i2c->CR |= (1 << 1);	// <STOPC>
                    icb->rx_busy = 0;
                }
            }
            break;
        case I2C_TX_COMPLETE:
            if (icb->error)
            {
                event = icb->error;
                icb->error = 0;
            }
            else
            {
                event |= I2C_EVENT_TRASMIT_DONE;
            }
            break;
        case I2C_RX_COMPLETE:
            if (icb->error)
            {
                event = icb->error;
                icb->error = 0;
            }
            else
            {
                event |= I2C_EVENT_RECEIVE_DONE;
            }
            break;
        default:
            i2c->CR |= (1 << 1);		// <STOPC>
            icb->tx_busy = 0;
            icb->rx_busy = 0;

            if (status & (1 << 3))	// <MLOST>
            {
                icb->error |= I2C_EVENT_MASTERSHIP_LOSS;
            }
            if ((status & (1 << 1)) && !(status & (1 << 0)))	// <TMOD> && <RXACK>
            {
                icb->error |= I2C_EVENT_NO_ACK;
            }
            if (!event)
            {
                icb->error |= I2C_EVENT_HW_ERR;
            }
            break;
    }

		/* Callback Function */
    if (icb->handler && event)
    {
        icb->handler(event, icb->context);
    }

    i2c->ST = 0x800000FF;
}

//////////////////////////////////////////////////////////////////////////////////////
void I2C0_IRQHandler(void)
{
    if (g_icb[I2C_ID_0].mode == I2C_MASTER_OPERATION)
    {
        I2C_MasterIRQHandler(I2C_ID_0);
    }
    else
    {
        I2C_SlaveIRQHandler(I2C_ID_0);
    }
}

void I2C1_IRQHandler(void)
{
    if (g_icb[I2C_ID_1].mode == I2C_MASTER_OPERATION)
    {
        I2C_MasterIRQHandler(I2C_ID_1);
    }
    else
    {
        I2C_SlaveIRQHandler(I2C_ID_1);
    }
}

