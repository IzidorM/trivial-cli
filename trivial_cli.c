/**
 * Simple command line interface for use in embedded systems
 * Author:    Izidor Makuc
 * Created:   3.09.2016
 * 
 * License: Public Domain (see UNLICENSE file or http://unlicense.org/)
 * Usage: See README file
 **/

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "trivial_cli.h"

static void tc_output_string(struct trivial_cli *cli, const char *s)
{
        uint32_t i;
        for (i = 0; s[i] != '\0'; i++)
        {
                cli->character_send(s[i]);
        }
}

static void tc_help_function(struct trivial_cli *cli, char *s)
{
        struct trivial_cli_cmd *tmp = &cli->cmd_list;

        for (; NULL != tmp; tmp = tmp->next)
        {
                tc_output_string(cli, tmp->command_name);
                tc_output_string(cli, "\n\r");
                if (tmp->command_description)
                {
                        tc_output_string(cli, "\t");
                        tc_output_string(cli, tmp->command_description);
                        tc_output_string(cli, "\n\r");

                }
        }
}

static struct trivial_cli_cmd *trivial_cli_search_command(struct trivial_cli *cli)
{
        struct trivial_cli_cmd *tmp_command = NULL;
        struct trivial_cli_cmd *tmp = &cli->cmd_list;

        for (; NULL != tmp; tmp = tmp->next)
        {
                if (0 == strcmp(tmp->command_name, cli->command_buff))
                {
                        tmp_command = tmp;
                }
        }

        return tmp_command;
}

void trivial_cli_add_cmd(struct trivial_cli *cli, struct trivial_cli_cmd *c)
{
        struct trivial_cli_cmd *tmp = &cli->cmd_list;
        ASSERT(tmp);
        
        for (; NULL != tmp->next; tmp = tmp->next);

        tmp->next = c;
}

void trivial_cli_init(struct trivial_cli *cli,
                      char char_receive(void), void (*char_send)(char c), char end_char)
{
        ASSERT(char_receive);
        ASSERT(char_send);

        cli->character_received = char_receive;
        cli->character_send = char_send;
        cli->command_buff_index = 0;

        cli->cmd_list.next = NULL;
        cli->cmd_list.command_name = "help";
        cli->cmd_list.command_description = NULL;
        cli->cmd_list.command_function = tc_help_function;
        cli->cmd_end_char= end_char;
}


void trivial_cli_get_arg(struct trivial_cli *cli, char *buff, uint32_t buff_size)
{
        uint32_t index = 0;
        for (;;)
        {
                char c = cli->character_received();
                if (cli->cmd_end_char == c)
                {
                        buff[index] = '\0';
                        break;
                }
                else if( '\b' == c) //backspace
                {
                        if (index)
                        {
                                index -= 1;
                                cli->character_send('\b');
                                cli->character_send(' ');
                                cli->character_send('\b');
                        }
                }
                else if ((index + 1) < buff_size)
                {
                        buff[index] = c;
                        cli->character_send(c);
                        index += 1;
                }
        }
}

struct trivial_cli_cmd *trivial_cli_new_char_receved(struct trivial_cli *cli, char c)
{
        struct trivial_cli_cmd *tmp_command = NULL;
        if (cli->cmd_end_char == c)
        {
                cli->character_send('\n');
                cli->character_send('\r');
                cli->command_buff[cli->command_buff_index] = '\0';
                if (cli->command_buff_index)
                {
                        tmp_command = trivial_cli_search_command(cli);
                        cli->command_buff_index = 0;
                }
                else
                {
                        tc_output_string(cli, "> ");
                }
        }
        else if( '\b' == c) //backspace
        {
                if (cli->command_buff_index)
                {
                        cli->command_buff_index -= 1;
                        cli->character_send('\b');
                        cli->character_send(' ');
                        cli->character_send('\b');
                }
        }
        else if ((cli->command_buff_index + 1) < sizeof(cli->command_buff))
        {
                cli->command_buff[cli->command_buff_index] = c;
                cli->character_send(c);
                cli->command_buff_index += 1;
        }
        return tmp_command;
}
