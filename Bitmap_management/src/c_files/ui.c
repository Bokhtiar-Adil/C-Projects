#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../h_files/utils.h"
#include "../h_files/bit_management.h"
#include "../h_files/ui.h"

void print_middle_aligned(const char *text, i32 given_width);
void get_integer_from_user_until_valid_value_is_given(i32 *value, i32 min_value, i32 max_value);
void main_menu();
void create_bitmap();
void print_bitmap();
void add_bit_in_bitmap();
void delete_bit_in_bitmap();
void and_bitmaps();
void or_bitmaps();
void not_bitmap();
void clone_bitmap();
void parse_string();
void destroy_bitmap();
void exit_process(bool *is_exit);

static i32 ui_iter = 0;
const char *ui_menu_message = "";
struct bitmap *ui_bitmap_1 = NULL;
struct bitmap *ui_bitmap_2 = NULL;
fptr_print_function bitmap_printer = NULL;

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

/*
 *  this function keeps taking user input until a valid value within a given range is inserted
 */

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
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");
    print_middle_aligned(APP_NAME, MAX_MENU_WIDTH);
    printf("\n");

    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");
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
    printf("10. Destroy a bitmap\n");
    printf("11. Exit\n");

    for (ui_iter = 0; ui_iter <= MAX_MENU_WIDTH; ui_iter++)
        printf("*");

    printf("\n");

    if (strlen(ui_menu_message) != 0)
        printf("%s\n", ui_menu_message);

    printf("Insert your command here: ");
    printf("\n");

    return;
}

void create_bitmap()
{
    i32 capacity = 0;
    struct bitmap *new = NULL;

    printf("** CREATE BITMAP **\n");
    printf("Insert capacity (Range: 1 to 65535) >> ");
    get_integer_from_user_until_valid_value_is_given(&capacity, 1, 65535);
    new = bitmap_create((u16)capacity);

    if (new == NULL)
    {
        ui_menu_message = utils_check_message;

        return;
    }

    /*
     *  if bitmap_1 is null it stores there
     *  else it stores in bitmap_2
     *  if neither are null, overwrites bitmap_1
     */

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

    printf("** AND BITMAP **\n");
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

    printf("** OR BITMAP **\n");
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
    i32 value = 0;
    i32 from_index = 0;
    i32 to_index = 0;
    i32 index_so_far = 0;
    bool get_number = false;
    bool found_hipen = false;
    bool is_invalid = false;
    struct bitmap *new = NULL;

    ui_iter = 0;
    printf("** PARSE A STRING **\n");
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
    from_index = INVALID_VALUE_INDICATOR;
    to_index = INVALID_VALUE_INDICATOR;
    value = INVALID_VALUE_INDICATOR;
    index_so_far = INVALID_VALUE_INDICATOR;
    found_hipen = false;

    while (ui_iter < len + 1)
    {
        /*
         *  if any invalid character is found, loop ends
         */

        if ((str[ui_iter] < '0' || str[ui_iter] > '9') && str[ui_iter] != '-' && str[ui_iter] != ',' && str[ui_iter] != '\0')
        {
            is_invalid = true;
            break;
        }

        if (str[ui_iter] == '-')
        {
            /*
             *  if the current character is a hiphen, then -
             *  first it checks whether it is a duplicate one or not
             *  if it is not duplicate, sets the flag found_hipen to true
             */

            if (found_hipen == true)
            {
                is_invalid = true;
                break;
            }

            found_hipen = true;

            /*
             *  it checks if the extracted value is within limits and does not overlap previous values
             *  overlapping means - if str is like this -> "1,3-7,4-9" -> then it is invalid
             *  value < MIN_INDEX_OF_PARSE_STRING will be true if value == INVALID_VALUE_INDICATOR
             *  this essentially means that no new number was found between previous hipen/comma and this one
             *  example cases --> "-", ",-", "1,-", "1,3--", etc
             */

            if (value <= index_so_far || value < MIN_INDEX_OF_PARSE_STRING || value > MAX_INDEX_OF_PARSE_STRING)
            {
                is_invalid = true;
                break;
            }

            if (from_index == INVALID_VALUE_INDICATOR)
                from_index = value;

            value = INVALID_VALUE_INDICATOR;
        }
        else if (str[ui_iter] == ',' || str[ui_iter] == '\0')
        {
            /*
             *  if the current character is a comma, then -
             *  first it sets the falg found_hipen to false because now the next set of numbers is reached
             */

            found_hipen = false;

            /*
             *  it checks if the extracted value is within limits and does not overlap previous values
             *  overlapping means - if str is like this -> "1,3-7,4-9" -> then it is invalid
             *  value < MIN_INDEX_OF_PARSE_STRING will be true if value == INVALID_VALUE_INDICATOR
             *  this essentially means that no new number was found between previous hipen/comma and this one
             *  example cases --> ",", "-,", "1-,", "1,3,,", etc
             */

            if (value <= index_so_far || value < MIN_INDEX_OF_PARSE_STRING || value > MAX_INDEX_OF_PARSE_STRING)
            {
                is_invalid = true;
                break;
            }

            if (from_index == INVALID_VALUE_INDICATOR)
                from_index = value;

            to_index = value;

            /*
             *  this is invalid --> "1,5-2"
             */

            if (to_index < from_index)
            {
                is_invalid = true;
                break;
            }

            /*
             *  index_so_far store the maximum index value reached so far to check overlap
             *  checking is done and so, resetting value, from_index, and to_index
             */

            index_so_far = to_index;
            value = INVALID_VALUE_INDICATOR;
            from_index = INVALID_VALUE_INDICATOR;
            to_index = INVALID_VALUE_INDICATOR;
        }
        else
        {
            /*
             *  if value == -1, it becomes 0
             */

            value = FIND_MAX(value, 0);
            value = value * 10 + (str[ui_iter] - '0');
        }

        ui_iter++;
    }

    if (is_invalid)
        ui_menu_message = INVALID_INPUT_MESSAGE;
    else
    {
        new = bitmap_parse_str(str);

        if (new == NULL)
            ui_menu_message = utils_check_message;
        else
        {
            /*
             *  always stores in ui_bitmap_1
             */

            free(ui_bitmap_1);
            ui_bitmap_1 = NULL;
            ui_bitmap_1 = new;
            ui_menu_message = OPERATION_SUCCESSFUL_MESSAGE;
        }
    }

    return;
}

void destroy_bitmap()
{
    i32 which = 0;

    printf("** DESTROY A BITMAP **\n");
    which = get_which_bitmap();

    if (which == 1)
    {
        bitmap_destroy(ui_bitmap_1);
        ui_bitmap_1 = NULL;
    }
    else if (which == 2)
    {
        bitmap_destroy(ui_bitmap_2);
        ui_bitmap_2 = NULL;
    }
    else
        ;

    return;
}

void exit_process(bool *is_exit)
{
    i32 user_choice = 0;

    printf("** EXIT **\n");
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
