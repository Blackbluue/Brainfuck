size_t collect_commands(FILE *commands_file, char **commands);
void execute_command(char command, char **data_ptr);
size_t run_block(char *commands, size_t cmd_ptr, char **data_ptr);
void *check_mem(void *ptr);