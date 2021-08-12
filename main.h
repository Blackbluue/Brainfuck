#define MEM_LIMIT 30000

size_t collect_commands(FILE *commands_file, char **commands);
int skip_comment_loop(FILE *commands_file);
size_t execute_command(char *commands, size_t cmd_index, unsigned char **data_ptr, unsigned char *left_lim, unsigned char *right_lim);
size_t find_block(char *commands, size_t cmd_index);
void run_block(char *commands, size_t start, size_t end, unsigned char **data_ptr, unsigned char *left_lim, unsigned char *right_lim);
void *check_mem(void *ptr);
void test_print(char command, unsigned char *data_ptr);