#ifndef PL230_DMA_H
#define PL230_DMA_H

#include <stdint.h>
#include <stdbool.h>

// DMA channel structure
typedef struct {
    uint32_t source;
    uint32_t destination;
    uint32_t control;
    uint32_t unused;
} DMA_Channel;

// DMA controller structure
typedef struct {
    volatile uint32_t status;
    volatile uint32_t config;
    volatile uint32_t ctrl_base_ptr;
    volatile uint32_t alt_ctrl_base_ptr;
    volatile uint32_t wait_on_req_status;
    volatile uint32_t chnl_sw_request;
    volatile uint32_t chnl_useburst_set;
    volatile uint32_t chnl_useburst_clr;
    volatile uint32_t chnl_req_mask_set;
    volatile uint32_t chnl_req_mask_clr;
    volatile uint32_t chnl_enable_set;
    volatile uint32_t chnl_enable_clr;
    volatile uint32_t chnl_pri_alt_set;
    volatile uint32_t chnl_pri_alt_clr;
    volatile uint32_t chnl_priority_set;
    volatile uint32_t chnl_priority_clr;
} DMA_TypeDef;

// DMA configuration structure
typedef struct {
    uint32_t channel_num;
    uint32_t source;
    uint32_t destination;
    uint32_t size;
    bool increment_source;
    bool increment_destination;
} DMA_Config;

// Function prototypes
void DMA_Init(DMA_TypeDef *DMAx);
void DMA_ConfigureChannel(DMA_TypeDef *DMAx, DMA_Config *config);
void DMA_StartTransfer(DMA_TypeDef *DMAx, uint32_t channel);
void DMA_StopTransfer(DMA_TypeDef *DMAx, uint32_t channel);
bool DMA_IsTransferComplete(DMA_TypeDef *DMAx, uint32_t channel);
void DMA_ClearInterrupt(DMA_TypeDef *DMAx, uint32_t channel);

#endif // PL230_DMA_H