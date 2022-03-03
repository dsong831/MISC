/**
 *******************************************************************************
 * @file        debug_cmd.c
 * @brief       Debugging with command line input
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
#include <string.h>

#include "debug_log.h"
#include "debug_cmd.h"
#include "debug_serial.h"

#define MAX_CMD_NUM_ARGS 10
#define MAX_CMD_LINE_LEN 96

static const struct debug_cmd *cmd_list;
static uint8_t cmd_size;
static char cmd_buf[MAX_CMD_LINE_LEN + 1];
static uint8_t cmd_buf_index;
static volatile uint8_t cmd_ready;

static void tokenize(char *cmd_line, int *argc, char *argv[])
{
    int found_arg = 1;
    int argn = 0;

    /* parse input string as argc and argv */
    while (*cmd_line)
    {
        if (*cmd_line == '\n')
        {
            *cmd_line = '\0';
            break;
        }
        if ((*cmd_line == ' '))
        {
            found_arg = 1;
            *cmd_line = '\0';
        }
        else if (found_arg)
        {
            argv[argn++] = cmd_line;
            found_arg = 0;
        }

        cmd_line++;

        if (argn >= MAX_CMD_NUM_ARGS)
        {
            break;
        }
    }

    *argc = argn;
}

static void cmd_help(int index, int usage)
{
    LOG("%-5s: %s\n", cmd_list[index].str, cmd_list[index].desc);
    if (usage && cmd_list[index].usage)
    {
        LOG("%s\n", cmd_list[index].usage);
    }
}

static int execute(char *param)
{
    int argc = 0;
    char *argv[MAX_CMD_NUM_ARGS] = { NULL, };
    int ret = DEBUG_CMD_SUCCESS;
    int i;

    tokenize(param, &argc, argv);

    if (argv[0] == NULL || argc == 0)
    {
        /* check for default handler */
        argc = 1;
        argv[0] = "";

    }
    else if (!strcmp(argv[0], "help"))
    {

        /* check for help */
        for (i = 0; i < cmd_size; i++)
        {
            /* specific command */
            if (argc == 2)
            {
                if (!strcmp(cmd_list[i].str, argv[1]))
                {
                    cmd_help(i, 1);
                    break;
                }
            }
            else
            {
                cmd_help(i, 0);
            }
        }

        goto out;
    }

    /* find the command from the list */
    for (i = 0; i < cmd_size; i++)
    {
        if (!strcmp(argv[0], cmd_list[i].str))
        {
            ret = cmd_list[i].func(argc, argv);
            if (ret == DEBUG_CMD_INVALID)
            {
                cmd_help(i, 1);
            }
            goto out;
        }
    }

    /* end of command list */
    if (strlen(argv[0]) > 0)
    {
        LOG("\"%s\" not a valid command\n", argv[0]);
    }

out:
    /* print a prompt */
    LOG("# ");

    return ret;
}

static void read_cmd_byte_cb(uint8_t ch)
{
    if (ch == '\r' || ch == '\n')
    {
        cmd_ready = 1;
        cmd_buf[cmd_buf_index++] = '\0';
    }
    else
    {
        /* only printable characters recognized for command */
        if (ch >= 0x20 && ch <= 0x7E && cmd_buf_index < (MAX_CMD_LINE_LEN - 1))
        {
            cmd_buf[cmd_buf_index++] = ch;
        }
    }
    serial_getc(read_cmd_byte_cb);
}

void debug_cmd_init(const struct debug_cmd *list, uint8_t list_size)
{
    cmd_list = list;
    cmd_size = list_size;

    /* receive a character one by one */
    cmd_buf_index = 0;
    cmd_ready = 0;
    serial_getc(read_cmd_byte_cb);
}

void debug_cmd_deinit(void)
{
}

void debug_cmd_process(void)
{
    /* execute the command when signaled it is ready */
    if (cmd_ready)
    {
        execute(cmd_buf);
        cmd_buf_index = 0;
        cmd_ready = 0;
    }
}

#endif

