#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include "main.h"


int main(int argc, char* argv[]) {
    unsigned char data[MEM_LIMIT] = {0};
    unsigned char *data_ptr = data;  // pointer to single char
    unsigned char *left_lim = data;  // farthest left data pointer
    unsigned char *right_lim = data + MEM_LIMIT;  // farthest right data pointer
    FILE *cmd_file;
    char *commands;  // array of commands
    size_t cmd_index = 0;
    size_t cmd_len;

    // open file of commands
    //cmd_file = fopen(argv[1], "r");
    cmd_file = fopen("./test.bf", "r");
    // read commands from file, store in array
    // errno = 0;
    cmd_len = collect_commands(cmd_file, &commands);
    fclose(cmd_file);
    // Check for errors reading commands file
    if(cmd_len == -1) {
        puts("Error reading commdands file");
        return -1;
    }

    // run commands
    while(cmd_index < cmd_len) {
        cmd_index = execute_command(commands, cmd_index, &data_ptr, left_lim, right_lim);
    }
    return 0;
}

/**
 * Reads the file to collect all valid BF commands from it.
 * Stores the runnable commands into the passed array.
 * Returns the number of runnable commands found in the file. Non-BF
 * commands are ignored. Initial comment loops are also ignored.
 */
size_t collect_commands(FILE *commands_file, char **commands) {
    char valid_commands[] = "+-<>[],.";
    char c;
    size_t cmd_len = 0;
    size_t size = 100;
    int balanced = 0;
    char *buffer = check_mem(malloc(sizeof(char) * size));

    // will ignore first loop if it is the first thing in the file
    while((c = getc(commands_file)) != EOF) {
        if(cmd_len >= size - 1)  // resize array if too small
            buffer = check_mem(realloc(buffer, size *= 2));

        if(strchr(valid_commands, c) != NULL) {
            // check for balanced loops
            if(c == '[')
                balanced++;
            else if(c == ']')
                balanced--;
            buffer[cmd_len++] = c;
        }
    }
    // check if braces are balanced
    if(balanced != 0) {
        return -1;
    }
    buffer = realloc(buffer, cmd_len);
    *commands = buffer;
    return cmd_len;
}

/**
 * Runs a single command based on the symbol passed in. Returns the current
 * position of the command array after executing the command.
 */
size_t execute_command(char *commands, size_t cmd_index, unsigned char **data_ptr, unsigned char *left_lim, unsigned char *right_lim) {
    unsigned int input;
    size_t start = cmd_index;
    size_t end;
    switch(commands[cmd_index]) {
        case '+':
            if(**data_ptr == UCHAR_MAX)
                **data_ptr = 0;
            else
                (**data_ptr)++;
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case '-':
            if(**data_ptr == 0)
                **data_ptr = UCHAR_MAX;
            else
                (**data_ptr)--;
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case '<':
            if(*data_ptr != left_lim)
                (*data_ptr)--;
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case '>':
            if(*data_ptr != right_lim)
                (*data_ptr)++;
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case ',':
            scanf("%u", &input);
            if(input >= UCHAR_MAX)
                **data_ptr = UCHAR_MAX;
            else
                **data_ptr = input;
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case '.':
            putchar(**data_ptr);
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;
            break;
        case '[':
            end = find_block(commands, cmd_index);
            //test_print(commands[cmd_index], *data_ptr);
            run_block(commands, start, end, data_ptr, left_lim, right_lim);
            cmd_index = end + 1;
            break;
        case ']':
            //test_print(commands[cmd_index], *data_ptr);
            cmd_index++;  // no operation
            break;
        default:
            puts("Error reading commands");
            break;
    }
    return cmd_index;
}

/**
 * finds the end of a block of code between a pair of '[]' symbols, given
 * the start position. Returns the position in the commands of the final ']'.
 */
size_t find_block(char *commands, size_t cmd_index) {
    int balance = 1;
    // seek position of ']'
    while(balance != 0) {
        cmd_index++;
        if(commands[cmd_index] == '[')
            balance++;
        else if(commands[cmd_index] == ']')
            balance--;
    }

    return cmd_index;
}

/**
 * runs a block of code between a pair of '[]' symbols.
 */
void run_block(char *commands, size_t start, size_t end, unsigned char **data_ptr, unsigned char *left_lim, unsigned char *right_lim) {
    while(**data_ptr != 0) {
        for(size_t cmd_index = start + 1; cmd_index < end; cmd_index++) {
            execute_command(commands, cmd_index, data_ptr, left_lim, right_lim);
        }
    }
}

void *check_mem(void *ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return ptr;
}

void test_print(char command, unsigned char *data_ptr) {
    printf("command: %c\n", command);
    printf("data pointer: %p\n", data_ptr);
    printf("data value: %d\n\n", *data_ptr);
}