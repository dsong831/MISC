/**
 *******************************************************************************
 * @file        debug_retarget.c
 * @brief       Debugging retarget, forward fgetc/fputc to serial
 * @version
 * @date
 * @author
 * @note
 *
 * Copyright(C) 2017, ABOV Semiconductor
 * All rights reserved.
 *
 ******************************************************************************/



#if (CONFIG_DEBUG == 1)

#include <stdio.h>
#include <stdint.h>

#include "debug_serial.h"

#if (__CC_ARM)

#pragma import(__use_no_semihosting_swi)
struct __FILE { int handle; /* Add whatever you need here */ };

FILE __stdout;
FILE __stdin;
FILE __stderr;

int fputc(int c, FILE *f)
{
    (void)f;

    return serial_putc((char)c);
}

int fgetc(FILE *f)
{
    int ch;

    (void)f;

    while (1)
    {
        ch = serial_getc(NULL);
        if (ch != EOF)
        {
            break;
        }
    }

    return ch;
}

int ferror(FILE *f)
{
    (void)f;

    return EOF;
}

void __ttywrch(int c)
{
    serial_putc((char)c);
}

#elif (__GNUC__)

FILE __stdout;
FILE __stdin;
FILE __stderr;

int _write(int file, const char *p_char, int len)
{
    int i;

    (void)file;


    for (i = 0; i < len; i++)
    {
        serial_putc(p_char[i]);
    }

    return len;
}

int _read(int file, char *p_char, int len)
{
    int ch;

    (void)file;
    (void)len;

    while (1)
    {
        ch = serial_getc(NULL);
        if (ch != EOF)
        {
            break;
        }
    }
    *p_char = ch;

    return 1;
}

#elif (__ICCARM__)

int __write(int file, const char *p_char, int len)
{
    int i;

    (void)file;

    for (i = 0; i < len; i++)
    {
        serial_putc(p_char[i]);
    }

    return len;
}

int __read(int file, char *p_char, int len)
{
    int ch;

    (void)file;
    (void)len;

    while (1)
    {
        ch = serial_getc(NULL);
        if (ch != EOF)
        {
            break;
        }
    }
    *p_char = ch;

    return 1;
}

#endif

void _sys_exit(int return_code)
{
label:  goto label;  /* endless loop */
}

#endif

