#include "gpio.h"

#define GPIO_PIN_MASK(pin) (1UL << (pin))

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config) {
    uint32_t pin_mask = GPIO_PIN_MASK(config->pin);

    // Set direction
    if (config->is_output) {
        GPIOx->OUTENSET = pin_mask;
    } else {
        GPIOx->OUTENCLR = pin_mask;
    }

    // Configure alternate function
    if (config->use_alt_func) {
        GPIOx->ALTFUNCSET = pin_mask;
    } else {
        GPIOx->ALTFUNCCLR = pin_mask;
    }

    // Configure interrupt
    if (config->enable_interrupt) {
        GPIOx->INTENSET = pin_mask;
        if (config->interrupt_type_edge) {
            GPIOx->INTTYPESET = pin_mask;
        } else {
            GPIOx->INTTYPECLR = pin_mask;
        }
        if (config->interrupt_pol_high) {
            GPIOx->INTPOLSET = pin_mask;
        } else {
            GPIOx->INTPOLCLR = pin_mask;
        }
    } else {
        GPIOx->INTENCLR = pin_mask;
    }
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, bool state) {
    uint32_t pin_mask = GPIO_PIN_MASK(pin);
    if (state) {
        GPIOx->DATAOUT |= pin_mask;
    } else {
        GPIOx->DATAOUT &= ~pin_mask;
    }
}

bool GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin) {
    return (GPIOx->DATA & GPIO_PIN_MASK(pin)) != 0;
}

void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->DATAOUT ^= GPIO_PIN_MASK(pin);
}

void GPIO_EnableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin, bool is_edge, bool is_high) {
    uint32_t pin_mask = GPIO_PIN_MASK(pin);
    GPIOx->INTENSET = pin_mask;
    if (is_edge) {
        GPIOx->INTTYPESET = pin_mask;
    } else {
        GPIOx->INTTYPECLR = pin_mask;
    }
    if (is_high) {
        GPIOx->INTPOLSET = pin_mask;
    } else {
        GPIOx->INTPOLCLR = pin_mask;
    }
}

void GPIO_DisableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->INTENCLR = GPIO_PIN_MASK(pin);
}

bool GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint8_t pin) {
    // Note: This function assumes that there's an INTSTATUS register.
    // If not, you may need to modify this based on how interrupt status is checked in your specific GPIO controller.
    return (GPIOx->INTSTATUS_CLEAR & GPIO_PIN_MASK(pin)) != 0;
}

void GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin) {
    GPIOx->INTSTATUS_CLEAR = GPIO_PIN_MASK(pin);
}