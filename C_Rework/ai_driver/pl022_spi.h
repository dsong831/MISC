#ifndef PL022_SPI_H
#define PL022_SPI_H

#include <stdint.h>
#include <stdbool.h>

// SSP controller structure
typedef struct {
    volatile uint32_t CR0;
    volatile uint32_t CR1;
    volatile uint32_t DR;
    volatile uint32_t SR;
    volatile uint32_t CPSR;
    volatile uint32_t IMSC;
    volatile uint32_t RIS;
    volatile uint32_t MIS;
    volatile uint32_t ICR;
    volatile uint32_t DMACR;
} SSP_TypeDef;

// SPI configuration structure
typedef struct {
    uint32_t clock_frequency;
    uint8_t data_size;
    bool clock_phase;
    bool clock_polarity;
    bool is_master;
} SPI_Config;

// SPI data frame format
typedef enum {
    SPI_FORMAT_SPI = 0,
    SPI_FORMAT_SSI = 1,
    SPI_FORMAT_MICROWIRE = 2
} SPI_Format;

// Function prototypes
void SPI_Init(SSP_TypeDef *SSPx, SPI_Config *config);
void SPI_SetFormat(SSP_TypeDef *SSPx, SPI_Format format);
void SPI_Enable(SSP_TypeDef *SSPx);
void SPI_Disable(SSP_TypeDef *SSPx);
void SPI_TransmitData(SSP_TypeDef *SSPx, uint16_t data);
uint16_t SPI_ReceiveData(SSP_TypeDef *SSPx);
bool SPI_IsBusy(SSP_TypeDef *SSPx);
void SPI_EnableDMA(SSP_TypeDef *SSPx, bool tx_enable, bool rx_enable);
void SPI_DisableDMA(SSP_TypeDef *SSPx);

#endif // PL022_SPI_H