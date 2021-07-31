#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "main.h"

#define MEM_LIMIT 30000

int main(int argc, char* argv[]) {
    char data[MEM_LIMIT] = {0};
    char *data_ptr = data;
    FILE *cmd_file;
    char *commands;
    size_t cmd_index = 0;
    size_t cmd_len;

    // open file of commands
    //cmd_file = fopen(argv[1], "r");
    cmd_file = fopen("test.bf", "r");
    // read commands from file, store in array
    errno = 0;
    cmd_len = collect_commands(cmd_file, &commands);
    fclose(cmd_file);

    // run commands
    while(cmd_index <= cmd_len) {
        if(commands[cmd_index] == '[') {
            cmd_index = run_block(commands, cmd_index, &data_ptr);
        }
        execute_command(commands[cmd_index++], &data_ptr);
    }
    return 0;
}

/**
 * Reads the file to collect all valid Brainfuck commands from it.
 * Stores the runnable commands into the passed array.
 * Returns the number of runnable commands found in the file. Non-BF
 * commands are ignored. Initial comment loops are also ignored.
 */
size_t collect_commands(FILE *commands_file, char **commands) {
    char valid_commands[] = "+-<>[],.";  // '[]' checked elsewhere
    char c;
    size_t cmd_len = 0;
    size_t size = 100;
    bool balanced = true;
    char *buffer = check_mem(malloc(sizeof(char) * size));

    while((c = getc(commands_file)) != EOF) {
        if(cmd_len >= size - 1)  // resize if too small
            buffer = check_mem(realloc(buffer, size *= 2));

        // skip inital comment loop here
        if(strchr(valid_commands, c) != NULL) {
            // check for balanced loops
            if(strchr("[]", c) != NULL) {
                if(c == '[' && balanced)
                    balanced = false;
                else if(c == ']' && !balanced)
                    balanced = true;
                else {  // '[]' symbols are not balanced correctly
                    // set error code
                    return -1;
                }
            }
            buffer[cmd_len++] = c;
        }
    }
    buffer = realloc(buffer, cmd_len);
    *commands = buffer;

    return cmd_len;
}

/**
 * Runs a single command based on the symbol passed in. Returns the current
 * position of the command array after executing the command.
 */
void execute_command(char command, char **data_ptr) {

}

/**
 * runs a block of code between a pair of '[]' symbols. Returns the position
 * in the commands array immediately after the final ']' once the block is finished.
 */
size_t run_block(char *commands, size_t cmd_index, char **data_ptr) {
    // note position of '['
    // seek position of ']'
    // if current data cell == 0, skip block to after ']'
    // else, go into block
    return cmd_index;
}



void *check_mem(void *ptr) {
    if(ptr == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(1);
    }
    return ptr;
}