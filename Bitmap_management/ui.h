#ifndef __UI_H__
#define __UI_H__

#define MAX_MENU_WIDTH 50
#define APP_NAME "BITMAP MANAGEMENT"
#define INVALID_INPUT_MESSAGE "ERROR: invalid input"
#define OPERATION_SUCCESSFUL_MESSAGE "Operation successful"
#define USER_INPUT_OPTION_ONE 1
#define USER_INPUT_OPTION_TWO 2
#define USER_INPUT_OPTION_THREE 3
#define USER_INPUT_OPTION_FOUR 4
#define USER_INPUT_OPTION_FIVE 5
#define USER_INPUT_OPTION_SIX 6
#define USER_INPUT_OPTION_SEVEN 7
#define USER_INPUT_OPTION_EIGHT 8
#define USER_INPUT_OPTION_NINE 9
#define USER_INPUT_OPTION_TEN 10
#define USER_INPUT_OPTION_ELEVEN 11
#define MAX_PARSE_STRING_LENGTH 100

typedef (*fptr_print_function)(struct bitmap *);

extern const char *ui_menu_message;
extern fptr_print_function bitmap_printer;

void main_menu();
void create_bitmap();
void print_bitmap();
void add_bit_in_bitmap();
void delete_bit_in_bitmap();
void and_bitmaps();
void or_bitmaps();
void not_bitmap();
void parse_string();
void exit_process(bool *is_exit);

#endif /* __UI_H__ */