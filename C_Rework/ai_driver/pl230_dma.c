#include "dma.h"

#define DMA_BASE_ADDRESS 0x40000000  // Replace with actual base address
#define DMA ((DMA_TypeDef*)DMA_BASE_ADDRESS)

#define DMA_CHANNELS 8  // Assuming 8 channels, adjust if different

static DMA_Channel dma_channels[DMA_CHANNELS];

void DMA_Init(DMA_TypeDef *DMAx) {
    // Enable DMA controller
    DMAx->config |= 1;
    
    // Set the base address of the channel control data structure
    DMAx->ctrl_base_ptr = (uint32_t)dma_channels;
}

void DMA_ConfigureChannel(DMA_TypeDef *DMAx, DMA_Config *config) {
    if (config->channel_num >= DMA_CHANNELS) {
        return;  // Invalid channel number
    }

    DMA_Channel *channel = &dma_channels[config->channel_num];

    channel->source = config->source;
    channel->destination = config->destination;

    // Configure control word
    channel->control = config->size & 0x3FF;  // Transfer size (10 bits)
    channel->control |= (2 << 30);  // Set to 2 for 32-bit transfers
    
    if (config->increment_source) {
        channel->control |= (1 << 26);  // Source increment
    }
    if (config->increment_destination) {
        channel->control |= (1 << 27);  // Destination increment
    }

    // Enable the channel
    DMAx->chnl_enable_set = (1 << config->channel_num);
}

void DMA_StartTransfer(DMA_TypeDef *DMAx, uint32_t channel) {
    if (channel >= DMA_CHANNELS) {
        return;  // Invalid channel number
    }

    // Set the channel's request bit
    DMAx->chnl_sw_request = (1 << channel);
}

void DMA_StopTransfer(DMA_TypeDef *DMAx, uint32_t channel) {
    if (channel >= DMA_CHANNELS) {
        return;  // Invalid channel number
    }

    // Clear the channel's enable bit
    DMAx->chnl_enable_clr = (1 << channel);
}

bool DMA_IsTransferComplete(DMA_TypeDef *DMAx, uint32_t channel) {
    if (channel >= DMA_CHANNELS) {
        return false;  // Invalid channel number
    }

    // Check if the channel's done bit is set
    return (DMAx->status & (1 << channel)) != 0;
}

void DMA_ClearInterrupt(DMA_TypeDef *DMAx, uint32_t channel) {
    if (channel >= DMA_CHANNELS) {
        return;  // Invalid channel number
    }

    // Clear the channel's interrupt bit
    DMAx->status = (1 << channel);
}