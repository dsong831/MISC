/**
 *******************************************************************************
 * @file        main.c
 * @brief       Application main
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/

#include "main.h"
#include "debug_log.h"


static struct app_gpio_def app_leds[] = APP_GPIO_LEDS;

static volatile uint32_t msec;

void SysTick_Handler(void)
{
    /* Simply decrement count */
    if (msec > 0)
    {
        msec--;
    }
}

static void mainloop(void)
{
    uint8_t i;
    uint32_t ret;

    /* Configure systick for every 1 msec */
    ret = SysTick_Config(SystemCoreClock / 1000);
    if (ret)
    {
        LOG("error configuring systick\n");
    }

    /* configure leds */
    for (i = 0; i < (sizeof(app_leds) / sizeof(struct app_gpio_def)); i++)
    {
        /* Set mode to output */
        HAL_GPIO_SetMode(app_leds[i].port, app_leds[i].pin, GPIO_MODE_OUTPUT);

        /* Set output type to push pull */
        HAL_GPIO_SetOutput(app_leds[i].port, app_leds[i].pin, GPIO_OUTPUT_PUSH_PULL);

        /* Set pull up/down disable */
        HAL_GPIO_SetPull(app_leds[i].port, app_leds[i].pin, GPIO_PULL_DISABLE);
    }

    while (1)
    {
        for (i = 0; i < 6; i++)
        {
            /* wait for a while and set gpio */
            msec = 50;
            while (msec > 0)
                ;
            HAL_GPIO_SetPin(app_leds[i].port, app_leds[i].pin);
        }

        for (i = 0; i < 6; i++)
        {
            /* wait for a while and clear gpio */
            msec = 50;
            while (msec > 0)
                ;
            HAL_GPIO_ClearPin(app_leds[i].port, app_leds[i].pin);
        }
    }
}

int main(void)
{
    /* Initialize all port */
    Port_Init();

    /* Configure system settings */
    System_Setting_Config();

    /* Initialize UART Debugging framework to use logging or command input */
    Debug_Init();

    LOG("\n");
    LOG("************************************************\n");
    LOG("\t MCU: %s\n", APP_DEVICE);
    LOG("\t Core: %s\n", APP_CORE);
    LOG("\t Debug: UART%d - %dbps\n", APP_UART_ID, APP_UART_BAUD);
    LOG("\t %s, %s\n", __DATE__, __TIME__);
    LOG("\t Starter Kit GPIO Blinky\n");
    LOG("************************************************\n");

    /* Start main loop */
    mainloop();
}

