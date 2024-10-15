#include "gpio.h"

#define GPIO_PIN_MASK 0x000000FF

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config) {
    uint32_t pin_mask = 1 << config->pin;

    // Set direction
    if (config->is_output) {
        GPIOx->DIR |= pin_mask;
    } else {
        GPIOx->DIR &= ~pin_mask;
    }

    // Configure pull-up/pull-down
    if (config->pull_up) {
        GPIOx->PUR |= pin_mask;
    } else {
        GPIOx->PUR &= ~pin_mask;
    }
    if (config->pull_down) {
        GPIOx->PDR |= pin_mask;
    } else {
        GPIOx->PDR &= ~pin_mask;
    }

    // Configure open drain
    if (config->open_drain) {
        GPIOx->ODR |= pin_mask;
    } else {
        GPIOx->ODR &= ~pin_mask;
    }

    // Configure drive strength
    switch (config->drive_strength) {
        case 2:
            GPIOx->DR2R |= pin_mask;
            GPIOx->DR4R &= ~pin_mask;
            GPIOx->DR8R &= ~pin_mask;
            break;
        case 4:
            GPIOx->DR2R &= ~pin_mask;
            GPIOx->DR4R |= pin_mask;
            GPIOx->DR8R &= ~pin_mask;
            break;
        case 8:
            GPIOx->DR2R &= ~pin_mask;
            GPIOx->DR4R &= ~pin_mask;
            GPIOx->DR8R |= pin_mask;
            break;
        default:
            // Default to 2mA if invalid value is provided
            GPIOx->DR2R |= pin_mask;
            GPIOx->DR4R &= ~pin_mask;
            GPIOx->DR8R &= ~pin_mask;
    }

    // Configure analog mode
    if (config->is_analog) {
        GPIOx->AMSEL |= pin_mask;
        GPIOx->DEN &= ~pin_mask;
    } else {
        GPIOx->AMSEL &= ~pin_mask;
        GPIOx->DEN |= pin_mask;
    }
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, bool state) {
    GPIOx->DATA[1 << pin] = state ? (1 << pin) : 0;
}

bool GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin) {
    return (GPIOx->DATA[1 << pin] != 0);
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->DATA[1 << pin] ^= (1 << pin);
}

void GPIO_EnableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_TriggerType trigger) {
    uint32_t pin_mask = 1 << pin;

    // Disable the interrupt first
    GPIOx->IE &= ~pin_mask;

    // Configure the interrupt type
    switch (trigger) {
        case GPIO_TRIGGER_LEVEL_LOW:
            GPIOx->IS |= pin_mask;
            GPIOx->IEV &= ~pin_mask;
            break;
        case GPIO_TRIGGER_LEVEL_HIGH:
            GPIOx->IS |= pin_mask;
            GPIOx->IEV |= pin_mask;
            break;
        case GPIO_TRIGGER_EDGE_FALLING:
            GPIOx->IS &= ~pin_mask;
            GPIOx->IBE &= ~pin_mask;
            GPIOx->IEV &= ~pin_mask;
            break;
        case GPIO_TRIGGER_EDGE_RISING:
            GPIOx->IS &= ~pin_mask;
            GPIOx->IBE &= ~pin_mask;
            GPIOx->IEV |= pin_mask;
            break;
        case GPIO_TRIGGER_EDGE_BOTH:
            GPIOx->IS &= ~pin_mask;
            GPIOx->IBE |= pin_mask;
            break;
    }

    // Clear any previous interrupt
    GPIOx->IC = pin_mask;

    // Enable the interrupt
    GPIOx->IE |= pin_mask;
}

void GPIO_DisableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->IE &= ~(1 << pin);
}

bool GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint8_t pin) {
    return (GPIOx->RIS & (1 << pin)) != 0;
}

void GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->IC = (1 << pin);
}