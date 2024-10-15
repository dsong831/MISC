#ifndef ARM_GPIO_H
#define ARM_GPIO_H

#include <stdint.h>
#include <stdbool.h>

// GPIO port structure
typedef struct {
    volatile uint32_t DATA;
    volatile uint32_t DATAOUT;
    volatile uint32_t OUTENSET;
    volatile uint32_t OUTENCLR;
    volatile uint32_t ALTFUNCSET;
    volatile uint32_t ALTFUNCCLR;
    volatile uint32_t INTENSET;
    volatile uint32_t INTENCLR;
    volatile uint32_t INTTYPESET;
    volatile uint32_t INTTYPECLR;
    volatile uint32_t INTPOLSET;
    volatile uint32_t INTPOLCLR;
    volatile uint32_t INTSTATUS_CLEAR;
} GPIO_TypeDef;

// GPIO pin configuration structure
typedef struct {
    uint8_t pin;
    bool is_output;
    bool use_alt_func;
    bool enable_interrupt;
    bool interrupt_type_edge;  // true for edge, false for level
    bool interrupt_pol_high;   // true for high/rising, false for low/falling
} GPIO_Config;

// Function prototypes
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, bool state);
bool GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_EnableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin, bool is_edge, bool is_high);
void GPIO_DisableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin);
bool GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif // ARM_GPIO_H