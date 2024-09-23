#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../h_files/utils.h"
#include "../h_files/bit_management.h"
#include "../h_files/ui.h"

int main(void)
{
    bool is_exit = false;
    i32 selected_menu_item = -1;

    /*
     *  there are two bitmap print function - i) bitmap_print and ii) bitmap_print_details
     *  the first one only prints the string such as "1,3-5,6"
     *  the second one is for my own usage - prints all the elements of bitmap struct i.e. numbers, first_value, last_value, etc
     *  I use the second one to debug during development
     */

    bitmap_printer = bitmap_print;

    while (!is_exit)
    {
        main_menu();

        printf(">> ");
        selected_menu_item = get_integer_from_user_input();

        if (selected_menu_item == -1)
        {
            ui_menu_message = INVALID_INPUT_MESSAGE;
            continue;
        }

        switch (selected_menu_item)
        {
            case USER_INPUT_OPTION_ONE:
                ui_menu_message = "";
                create_bitmap();
                break;
            case USER_INPUT_OPTION_TWO:
                ui_menu_message = "";
                print_bitmap();
                break;
            case USER_INPUT_OPTION_THREE:
                ui_menu_message = "";
                add_bit_in_bitmap();
                break;
            case USER_INPUT_OPTION_FOUR:
                ui_menu_message = "";
                delete_bit_in_bitmap();
                break;
            case USER_INPUT_OPTION_FIVE:
                ui_menu_message = "";
                and_bitmaps();
                break;
            case USER_INPUT_OPTION_SIX:
                ui_menu_message = "";
                or_bitmaps();
                break;
            case USER_INPUT_OPTION_SEVEN:
                ui_menu_message = "";
                not_bitmap();
                break;
            case USER_INPUT_OPTION_EIGHT:
                ui_menu_message = "";
                parse_string();
                break;
            case USER_INPUT_OPTION_NINE:
                ui_menu_message = "";
                clone_bitmap();
                break;
            case USER_INPUT_OPTION_TEN:
                ui_menu_message = "";
                destroy_bitmap();
                break;
            case USER_INPUT_OPTION_ELEVEN:
                ui_menu_message = "";
                exit_process(&is_exit);
                break;
            default:
                break;
        }
    }

    return 0;
}