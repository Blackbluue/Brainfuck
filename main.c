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
    cmd_file = fopen("./test.bf", "r");
    // read commands from file, store in array
    errno = 0;
    cmd_len = collect_commands(cmd_file, &commands);
    fclose(cmd_file);

    // run commands
    while(cmd_index <= cmd_len) {
        //if(commands[cmd_index] == '[') {
        //    cmd_index = run_block(commands, cmd_index, &data_ptr);
        //}
        //execute_command(commands[cmd_index++], &data_ptr);
        //printf("test: %lu\n", cmd_index);
        putchar(commands[cmd_index++]);
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
    char valid_commands[] = "+-<>[],.";  // '[]' checked elsewhere
    char c;
    size_t cmd_len = 0;
    size_t size = 100;
    int balanced = 0;
    char *buffer = check_mem(malloc(sizeof(char) * size));

    // will ignore first loop if it is the first thing in the file
    // skip_comment_loop(commands_file);
    while((c = getc(commands_file)) != EOF) {
        if(cmd_len >= size - 1)  // resize if too small
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
 * Skip the initial comment loop of the commands file, if present. Returns non-zero
 * on errors.
 */
int skip_comment_loop(FILE *commands_file) {
    char c;
    // add 1 for '[', subtract 1 for ']'
    // needs to end in 0, or block is unbalanced
    bool balanced = false;

    // check if initial comment loop is present
    // rewind file to beginning if not
    if((c = getc(commands_file)) != '[') {
        rewind(commands_file);
        return 0;
    }
        
    while((c = getc(commands_file)) != EOF) {
        if(c == '[' && !balanced) {
            return -1;
        } else if(c == ']' && balanced) {
            balanced--;
            continue;
        } else  // skip all other characters
            continue;
    }
    // check if braces are balanced
    if(balanced != 0) {
        return -1;
    }
    return 0;
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