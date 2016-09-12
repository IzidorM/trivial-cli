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

// Example of a command definition. 
void tc_test_function(struct trivial_cli *cli, char *s)
{
        printf("Executing test command\n");
        printf("Input arg1: ");
        char arg_buff[16];
        trivial_cli_get_arg(cli, arg_buff, sizeof(arg_buff));
        printf("\n\r Input: %s\n\r", arg_buff);
}

struct trivial_cli_cmd test_cmd = {
        .next = NULL, // It will be set to point to next command automaticaly
        .command_name = "test",
        .command_description = "Test desc\n",
        .command_function = tc_test_function,
};

void put_char(char c)
{
        putchar(c);
}

char get_char(void)
{
        char c = getchar();
        return c;
}

void command_prompt(void)
{
        printf("> ");
}

void run_term(struct trivial_cli *cli)
{
        printf("Running test terminal\n\r");

        // init function need getchar and putcahr callbacks and end command character
        // for most embedded systems you want to use '\r' instead of '\n'
        trivial_cli_init(cli, get_char, put_char, '\n');
        trivial_cli_add_cmd(cli, &test_cmd);

        struct trivial_cli_cmd *tmp;
        command_prompt();

        for (;;)
        {
                // call this function for every new character
                // if the command is found, it will return a "command object"...
                tmp = trivial_cli_new_char_receved(cli, get_char());
                if (tmp)
                {
                        // run your command handling function
                        // or do whatever you want with it ...
                        tmp->command_function(cli, NULL);
                        command_prompt();
                }
        }
}

int main(void)
{
        struct trivial_cli cli;
        //run_tests(&cli);
        run_term(&cli);
}
