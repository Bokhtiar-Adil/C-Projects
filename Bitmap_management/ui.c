#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"
#include "bit_management.h"
#include "ui.h"

static i32 ui_iter = 0;
const char *ui_menu_message = "";
struct bitmap *ui_bitmap_1 = NULL;
struct bitmap *ui_bitmap_2 = NULL;
fptr_print_function bitmap_printer = NULL;

void clear_menu()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*
 *  Takes a string 'text' and prints in the middle of the given width in console
 */

void print_middle_aligned(const char *text, i32 given_width)
{
    for (ui_iter = 0; ui_iter < (given_width - strlen(text)) / 2; ui_iter++)
        printf(" ");

    printf("%s", text);
    ui_iter += strlen(text);

    for (; ui_iter <= given_width; ui_iter++)
        printf(" ");

    return;
}

void print_header()
{
    clear_menu();

    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");
    print_middle_aligned(APP_NAME, MAX_MENU_WIDTH);
    printf("\n");

    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");

    return;
}

void get_integer_from_user_until_valid_value_is_given(i32 *value, i32 min_value, i32 max_value)
{
    i32 input_character_count = 0;
    const char *message = INVALID_INPUT_MESSAGE;

    do
    {
        if (input_character_count != 0)
            printf("%s\n>> ", message);

        *value = get_integer_from_user_input();
        input_character_count++;

    } while (*value < min_value || *value > max_value);

    return;
}

void ui_message_and_user_input_area()
{
    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");

    if (strlen(ui_menu_message) != 0)
        printf("%s\n", ui_menu_message);

    printf("Insert your command here: ");
    printf("\n");

    return;
}

i32 get_which_bitmap()
{
    i32 which = 0;

    printf("1. Bitmap-1\n");
    printf("2. Bitmap-2\n");
    printf("3. Back\n");
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&which, 1, 3);

    return which;
}

void main_menu()
{
    print_header();
    print_middle_aligned("Main Menu", MAX_MENU_WIDTH);
    printf("\n");
    printf("\n");
    printf("1. Create a bitmap\n");
    printf("2. Print a bitmap \n");
    printf("3. Add a bit in bitmap \n");
    printf("4. Delete a bit in bitmap\n");
    printf("5. AND operation\n");
    printf("6. OR operation\n");
    printf("7. NOT operation\n");
    printf("8. Parse a string\n");
    printf("9. Clone a bitmap\n");
    printf("10. Parse a string\n");
    printf("11. Exit\n");

    ui_message_and_user_input_area();

    return;
}

void create_bitmap()
{
    i32 capacity = 0;
    struct bitmap *new = NULL;

    printf("** CREATE BITMAP **\n");
    printf("Insert capacity (Range: 1 to 65535) >> ");
    get_integer_from_user_until_valid_value_is_given(&capacity, 1, 65535);
    new = bitMap_create((u16)capacity);

    if (new == NULL)
    {
        ui_menu_message = utils_check_message;

        return;
    }

    if (ui_bitmap_1 == NULL)
        ui_bitmap_1 = new;
    else if (ui_bitmap_2 == NULL)
        ui_bitmap_2 = new;
    else
    {
        free(ui_bitmap_1);
        ui_bitmap_1 = NULL;
        ui_bitmap_1 = new;
    }

    ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;

    return;
}

void print_bitmap()
{
    i32 which = 0;

    printf("** PRINT BITMAP **\n");
    which = get_which_bitmap();

    if (which == 1)
    {
        if (ui_bitmap_1 != NULL)
            bitmap_printer(ui_bitmap_1);
        else
            printf("%s\n", NOT_YET_CREATED_MESSAGE);
    }
    else if (which == 2)
    {
        if (ui_bitmap_2 != NULL)
            bitmap_printer(ui_bitmap_2);
        else
            printf("%s\n", NOT_YET_CREATED_MESSAGE);
    }
    else
        return;

    printf("Press enter to exit >> ");
    getchar();

    return;
}

void add_bit_in_bitmap()
{
    i32 which = 0;
    i32 value_to_add = 0;
    bool is_done = false;
    struct bitmap *which_bitmap = NULL;

    printf("** ADD BIT **\n");
    which = get_which_bitmap();

    if (which == 3)
        return;

    which_bitmap = (which == 1) ? ui_bitmap_1 : ui_bitmap_2;

    if (which_bitmap == NULL)
    {
        ui_menu_message = NOT_YET_CREATED_MESSAGE;

        return;
    }

    printf("Insert value (Range: 1 to %hu) >> ", which_bitmap->max_value);
    get_integer_from_user_until_valid_value_is_given(&value_to_add, 1, which_bitmap->max_value);
    is_done = bitmap_add_value(which_bitmap, (u16)value_to_add);

    if (is_done)
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
    else
        ui_menu_message = utils_check_message;

    return;
}

void delete_bit_in_bitmap()
{
    i32 which = 0;
    i32 value_to_add = 0;
    bool is_done = false;
    struct bitmap *which_bitmap = NULL;

    printf("** DELETE BIT **\n");
    which = get_which_bitmap();

    if (which == 3)
        return;

    which_bitmap = (which == 1) ? ui_bitmap_1 : ui_bitmap_2;

    if (which_bitmap == NULL)
    {
        ui_menu_message = NOT_YET_CREATED_MESSAGE;

        return;
    }

    printf("Insert value (Range: 1 to %hu) >> ", which_bitmap->max_value);
    get_integer_from_user_until_valid_value_is_given(&value_to_add, 1, which_bitmap->max_value);
    is_done = bitmap_del_value(which_bitmap, (u16)value_to_add);

    if (is_done)
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
    else
        ui_menu_message = utils_check_message;

    return;
}

void and_bitmaps()
{
    bool is_done = false;

    if (ui_bitmap_1 == NULL)
    {
        ui_menu_message = "ERROR: Bitmap-1 hasn't been created yet";

        return;
    }
    else if (ui_bitmap_2 == NULL)
    {
        ui_menu_message = "ERROR: Bitmap-2 hasn't been created yet";

        return;
    }
    else
        ;

    printf("Old bitmaps:\n");
    printf("Bitmap-1:\n");
    bitmap_printer(ui_bitmap_1);
    printf("Bitmap-2:\n");
    bitmap_printer(ui_bitmap_2);
    is_done = bitmap_and(ui_bitmap_1, ui_bitmap_2);

    if (is_done)
    {
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
        printf("Changed bitmap-1:\n");
        bitmap_printer(ui_bitmap_1);
    }
    else
        ui_menu_message = utils_check_message;

    printf("Press enter to exit >> ");
    getchar();

    return;
}

void or_bitmaps()
{
    bool is_done = false;

    if (ui_bitmap_1 == NULL)
    {
        ui_menu_message = "ERROR: Bitmap-1 hasn't been created yet";

        return;
    }
    else if (ui_bitmap_2 == NULL)
    {
        ui_menu_message = "ERROR: Bitmap-2 hasn't been created yet";

        return;
    }
    else
        ;

    printf("Old bitmaps:\n");
    printf("Bitmap-1:\n");
    bitmap_printer(ui_bitmap_1);
    printf("Bitmap-2:\n");
    bitmap_printer(ui_bitmap_2);
    is_done = bitmap_or(ui_bitmap_1, ui_bitmap_2);

    if (is_done)
    {
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
        printf("Changed bitmap-1:\n");
        bitmap_printer(ui_bitmap_1);
    }
    else
        ui_menu_message = utils_check_message;

    printf("Press enter to exit >> ");
    getchar();

    return;
}

void not_bitmap()
{
    i32 which = 0;
    i32 value_to_add = 0;
    bool is_done = false;
    struct bitmap *which_bitmap = NULL;

    printf("** NOT BITMAP **\n");
    which = get_which_bitmap();

    if (which == 3)
        return;

    which_bitmap = (which == 1) ? ui_bitmap_1 : ui_bitmap_2;

    if (which_bitmap == NULL)
    {
        ui_menu_message = NOT_YET_CREATED_MESSAGE;

        return;
    }

    printf("Old bitmap:\n");
    printf("Bitmap-1:\n");
    bitmap_printer(ui_bitmap_1);
    is_done = bitmap_not(ui_bitmap_1);

    if (is_done)
    {
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
        printf("Changed bitmap-1:\n");
        bitmap_printer(ui_bitmap_1);
    }
    else
        ui_menu_message = utils_check_message;

    return;
}

void clone_bitmap()
{
    struct bitmap *new = NULL;
    
    new = bitmap_clone(ui_bitmap_2);

    if (new == NULL)
        ui_menu_message = utils_check_message;
    else
    {
        ui_bitmap_1 = new;
        ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
    }

    return;
}

void parse_string()
{
    char tmp = 0;
    char str[MAX_PARSE_STRING_LENGTH] = {0};
    i32 len = 0;
    bool get_number = false;
    bool get_hiphen = false;
    bool get_comma = false;
    bool is_valid = true;

    ui_iter = 0;
    printf("Insert a string within %d characters >> ", MAX_PARSE_STRING_LENGTH);

    /*
     *   This loop will keep taking inputs until 'enter' button is pressed
     *   wiil save the first 8 characters in the buffer as the student id is 8 character-long
     */

    while (tmp != '\n')
    {
        scanf("%c", &tmp);

        if (ui_iter < MAX_PARSE_STRING_LENGTH - 1 && tmp != '\n')
        {
            str[ui_iter] = tmp;
            ui_iter++;
        }
    }

    str[ui_iter] = '\0';
    len = strlen(str);
    ui_iter = 0;
    get_number = true;

    while (ui_iter < len)
    {

    }

    return;
}

void exit_process(bool *is_exit)
{
    i32 user_choice = 0;

    printf("Are you sure to exit?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&user_choice, 1, 2);

    if (user_choice == 1)
    {
        if (ui_bitmap_1 != NULL)
        {
            bitmap_destroy(ui_bitmap_1);
            ui_bitmap_1 = NULL;
        }

        if (ui_bitmap_2 != NULL)
        {
            bitmap_destroy(ui_bitmap_2);
            ui_bitmap_2 = NULL;
        }
        *is_exit = true;
    }

    return;
}
