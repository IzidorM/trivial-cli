/**
 * Simple command line interface for use in embedded systems
 * Author:    Izidor Makuc
 * Created:   3.09.2016
 * 
 * License: Public Domain (see UNLICENSE file or http://unlicense.org/)
 * Usage: See README file
 **/
#ifndef TRIVIAL_CLI_H
#define TRIVIAL_CLI_H

#define ASSERT(x)
#define TRIVIAL_CLI_COMMAND_BUFF_SIZE 32

struct trivial_cli;
struct trivial_cli_cmd;

struct trivial_cli_cmd {
        struct trivial_cli_cmd *next;
        const char *command_name;
        const char *command_description;
        void (*command_function)(struct trivial_cli *cli, char *command_input_string);
};

struct trivial_cli {
        char (*character_received)(void);
        void (*character_send)(char c);
        char command_buff[TRIVIAL_CLI_COMMAND_BUFF_SIZE];
        uint32_t command_buff_index;
        struct trivial_cli_cmd cmd_list;
        char cmd_end_char;
};

void trivial_cli_get_arg(struct trivial_cli *cli, char *buff, uint32_t buff_size);
void trivial_cli_add_cmd(struct trivial_cli *cli, struct trivial_cli_cmd *c);
void trivial_cli_init(struct trivial_cli *cli,
                      char char_receive(void), void (*char_send)(char c), char end_char);
struct trivial_cli_cmd *trivial_cli_new_char_receved(struct trivial_cli *cli, char c);

#endif
