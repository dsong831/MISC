#include "spi.h"

#define SPI_BASE_ADDRESS 0x40004000  // Replace with actual base address
#define SPI ((SSP_TypeDef*)SPI_BASE_ADDRESS)

#define SPI_CR0_SCR_MASK    0xFF00
#define SPI_CR0_SCR_SHIFT   8
#define SPI_CR0_SPH_MASK    0x0080
#define SPI_CR0_SPO_MASK    0x0040
#define SPI_CR0_FRF_MASK    0x0030
#define SPI_CR0_DSS_MASK    0x000F

#define SPI_CR1_SOD_MASK    0x0008
#define SPI_CR1_MS_MASK     0x0004
#define SPI_CR1_SSE_MASK    0x0002
#define SPI_CR1_LBM_MASK    0x0001

void SPI_Init(SSP_TypeDef *SSPx, SPI_Config *config) {
    uint32_t cr0 = 0;
    uint32_t cr1 = 0;
    uint32_t cpsr;

    // Configure CR0
    cr0 |= ((config->data_size - 1) & SPI_CR0_DSS_MASK);
    cr0 |= (config->clock_phase ? SPI_CR0_SPH_MASK : 0);
    cr0 |= (config->clock_polarity ? SPI_CR0_SPO_MASK : 0);

    // Calculate and set clock prescaler
    cpsr = SPI_BASE_ADDRESS / config->clock_frequency;
    if (cpsr < 2) cpsr = 2;
    if (cpsr > 254) cpsr = 254;
    SSPx->CPSR = cpsr;

    // Configure CR1
    cr1 |= (config->is_master ? 0 : SPI_CR1_MS_MASK);

    SSPx->CR0 = cr0;
    SSPx->CR1 = cr1;
}

void SPI_SetFormat(SSP_TypeDef *SSPx, SPI_Format format) {
    uint32_t cr0 = SSPx->CR0;
    cr0 &= ~SPI_CR0_FRF_MASK;
    cr0 |= (format << 4) & SPI_CR0_FRF_MASK;
    SSPx->CR0 = cr0;
}

void SPI_Enable(SSP_TypeDef *SSPx) {
    SSPx->CR1 |= SPI_CR1_SSE_MASK;
}

void SPI_Disable(SSP_TypeDef *SSPx) {
    SSPx->CR1 &= ~SPI_CR1_SSE_MASK;
}

void SPI_TransmitData(SSP_TypeDef *SSPx, uint16_t data) {
    while (SSPx->SR & 0x02);  // Wait until TX FIFO is not full
    SSPx->DR = data;
}

uint16_t SPI_ReceiveData(SSP_TypeDef *SSPx) {
    while (!(SSPx->SR & 0x04));  // Wait until RX FIFO is not empty
    return SSPx->DR;
}

bool SPI_IsBusy(SSP_TypeDef *SSPx) {
    return (SSPx->SR & 0x10) != 0;
}

void SPI_EnableDMA(SSP_TypeDef *SSPx, bool tx_enable, bool rx_enable) {
    uint32_t dmacr = SSPx->DMACR;
    if (tx_enable) dmacr |= 0x02;
    if (rx_enable) dmacr |= 0x01;
    SSPx->DMACR = dmacr;
}

void SPI_DisableDMA(SSP_TypeDef *SSPx) {
    SSPx->DMACR = 0;
}

/*

사용 예시:

```c
SPI_Config config = {
    .clock_frequency = 1000000,  // 1 MHz
    .data_size = 8,              // 8-bit data
    .clock_phase = false,
    .clock_polarity = false,
    .is_master = true
};

SPI_Init(SPI, &config);
SPI_SetFormat(SPI, SPI_FORMAT_SPI);
SPI_Enable(SPI);

// Transmit data
SPI_TransmitData(SPI, 0xAA);

// Receive data
uint16_t received_data = SPI_ReceiveData(SPI);

SPI_Disable(SPI);
```

사용 예시:

```c
DMA_Config config = {
    .channel_num = 0,
    .source = 0x20000000,
    .destination = 0x20001000,
    .size = 1024,
    .increment_source = true,
    .increment_destination = true
};

DMA_Init(DMA);
DMA_ConfigureChannel(DMA, &config);
DMA_StartTransfer(DMA, 0);

while (!DMA_IsTransferComplete(DMA, 0)) {
    // Wait for transfer to complete
}

DMA_ClearInterrupt(DMA, 0);
```

*/