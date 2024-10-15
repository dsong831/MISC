#ifndef ARM_GPIO_H
#define ARM_GPIO_H

#include <stdint.h>
#include <stdbool.h>

// GPIO port structure
typedef struct {
    volatile uint32_t DATA[256];  // Allows direct access to any GPIO pin
    volatile uint32_t DIR;
    volatile uint32_t IS;
    volatile uint32_t IBE;
    volatile uint32_t IEV;
    volatile uint32_t IE;
    volatile uint32_t RIS;
    volatile uint32_t MIS;
    volatile uint32_t IC;
    volatile uint32_t AFSEL;
    volatile uint32_t reserved[55];
    volatile uint32_t DR2R;
    volatile uint32_t DR4R;
    volatile uint32_t DR8R;
    volatile uint32_t ODR;
    volatile uint32_t PUR;
    volatile uint32_t PDR;
    volatile uint32_t SLR;
    volatile uint32_t DEN;
    volatile uint32_t LOCK;
    volatile uint32_t CR;
    volatile uint32_t AMSEL;
} GPIO_TypeDef;

// GPIO pin configuration structure
typedef struct {
    uint8_t pin;
    bool is_output;
    bool pull_up;
    bool pull_down;
    bool open_drain;
    uint8_t drive_strength;  // 2, 4, or 8 mA
    bool is_analog;
} GPIO_Config;

// GPIO interrupt trigger types
typedef enum {
    GPIO_TRIGGER_LEVEL_LOW = 0,
    GPIO_TRIGGER_LEVEL_HIGH,
    GPIO_TRIGGER_EDGE_FALLING,
    GPIO_TRIGGER_EDGE_RISING,
    GPIO_TRIGGER_EDGE_BOTH
} GPIO_TriggerType;

// Function prototypes
void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_Config *config);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t pin, bool state);
bool GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_EnableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin, GPIO_TriggerType trigger);
void GPIO_DisableInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin);
bool GPIO_GetInterruptStatus(GPIO_TypeDef *GPIOx, uint8_t pin);
void GPIO_ClearInterrupt(GPIO_TypeDef *GPIOx, uint8_t pin);

#endif // ARM_GPIO_H