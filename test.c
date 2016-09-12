/**
 * Simple command line interface for use in embedded systems
 * Author:    Izidor Makuc
 * Created:   3.09.2016
 * 
 * License: Public Domain (see UNLICENSE file or http://unlicense.org/)
 * Usage: See README file
 **/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "trivial_cli.h"

void tc_test_function(struct trivial_cli *cli, char *s)
{
        printf("TEST\n");
        printf("arg1:");
        char arg_buff[16];
        trivial_cli_get_arg(cli, arg_buff, sizeof(arg_buff));
        printf("%s", arg_buff);
}

struct trivial_cli_cmd test_cmd = {
        .next = NULL,
        .command_name = "test",
        .command_description = "Test desc\n",
        .command_function = tc_test_function,
};


char *string_to_receive;

char cr(void)
{
        char r = '\0';
        if (string_to_receive)
        {
                r = *string_to_receive;
                string_to_receive += 1;
        }
        return r;
}


void cs(char c)
{
        printf("-");
        putchar(c);
}

void run_tests(struct trivial_cli *cli)
{
        trivial_cli_init(cli, cr, cs, '\r');
        trivial_cli_add_cmd(cli, &test_cmd);

        struct trivial_cli_cmd *tmp;
        printf("> ");
        char help[] = "help\r";
        char test[] = "test\r";
        char arg[] = "arg_test\r";
        
        string_to_receive = help;

        for (char c = cr(); '\0' != c; c = cr())
        {
                tmp = trivial_cli_new_char_receved(cli, c);
                if (tmp)
                {
                        tmp->command_function(cli, NULL);
                        printf("> ");
                }
        }

        string_to_receive = test;
        for (char c = cr(); '\0' != c; c = cr())
        {
                tmp = trivial_cli_new_char_receved(cli, c);
                if (tmp)
                {
                        string_to_receive = arg;                        
                        tmp->command_function(cli, NULL);
                        printf("> ");
                }
        }
}

int main(void)
{
        struct trivial_cli cli;
        run_tests(&cli);
}
