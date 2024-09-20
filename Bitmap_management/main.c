#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "bit_management.h"
#include "ui.h"

int main(void)
{
    bool is_exit = false;
    i32 selected_menu_item = -1;

    bitmap_printer = bitmap_print_details;

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

            break;
        case USER_INPUT_OPTION_NINE:

            break;
        case USER_INPUT_OPTION_TEN:
            ui_menu_message = "";
            exit_process(&is_exit);
            break;
        case USER_INPUT_OPTION_ELEVEN:
            ui_menu_message = "";
            exit_process(&is_exit);
            break;
        default:
            break;
        }
    }

    // u8 *str = "00000000";
    // struct bitmap *bm2 = bitmap_parse_str(str);
    // struct bitmap *bm = bitMap_create(10);
    // // bitmap_print(bm);
    // // // struct bitmap *cpy = bitmap_clone(bm);
    // // // bitmap_print(cpy);
    // // struct bitmap *cpy = bitMap_create(20);
    // // bitmap_print(cpy);
    // // bitmap_not(bm);
    // // bitmap_print(bm);
    // // // bitmap_add_value(bm, 132);
    // // // bitmap_print(bm);
    // bitmap_add_value(bm, 4);
    // bitmap_add_value(bm, 41);
    // // bitmap_add_value(bm, 50);
    // // bitmap_add_value(bm, 68);
    // // bitmap_add_value(bm, 69);
    // bitmap_print(bm);
    // bitmap_print_bitmap_bits(bm);
    // // bitmap_add_value(cpy, 14);
    // // bitmap_print(cpy);
    // bitmap_del_value(bm, 41);
    // bitmap_print(bm);
    // bitmap_print_bitmap_bits(bm);

    // bitmap_add_value(bm, 38);
    // bitmap_print(bm);
    // bitmap_and(bm, cpy);
    // bitmap_print(bm);
    // bitmap_print(cpy);

    // struct bitmap *new = bitmap_parse_str("1,3-5,10");
    // bitmap_print(new);

    // struct bitmap *bm2 = bitmap_parse_str("1,7,11,20-23,56-57");
    // bitmap_print(bm2);
    // bitmap_print_bitmap_bits(bm2);

    return 0;
}