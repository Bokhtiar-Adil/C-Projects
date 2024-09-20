#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utils.h"

static i32 utils_iter = 0;
const char *utils_check_message = "";

u32 power_of_two(u32 value)
{
    u32 res = 1;

    while (value > 0)
    {
        res *= 2;
        value--;
    }

    return res;
}

void check_string(char *str)
{
    i32 len = 0;
    i32 number_of_ones = 0;
    i32 first_one_index = 0;
    i32 last_one_index = 0;
    bool is_one_found = false;

    len = strlen(str);
    utils_iter = 0;

    while (utils_iter < len)
    {
        if (str[utils_iter] == '1')
        {
            number_of_ones++;

            if (!is_one_found)
                last_one_index = len - 1 - utils_iter;

            first_one_index = len - 1 - utils_iter;
            is_one_found = true;
        }

        utils_iter++;
    }

    printf("** %s **\n", __FUNCTION__);
    printf("given_string        : %s\n", str);
    printf("length              : %d\n", len);
    printf("number_of_ones      : %d\n", number_of_ones);
    printf("first_one_index     : %d\n", first_one_index);
    printf("last_one_index      : %d\n", last_one_index);
    printf("\n");

    return;
}

i32 get_integer_from_user_input()
{
    i32 value = 0;
    char input = 0;
    char buffer[10] = {0};
    bool valid_input = true;
    utils_iter = 0;

    do
    {
        scanf("%c", &input);

        if ((input != '\n') && (input < '0' || input > '9'))
            valid_input = false;

        /*
         *   Only if the input contains valid character, it can be inserted into buffer
         *   However, the first 9 numbers will be stored, rest will be discarded
         */

        if (valid_input == true && input != '\n' && utils_iter < 10)
        {
            buffer[utils_iter] = input;
            utils_iter++;
        }

    } while (input != '\n');

    buffer[utils_iter] = '\0';

    /*
     *   If the user input contains an invalid character, it returns -1
     */

    if (valid_input == false)
        return -1;

    for (utils_iter = 0; buffer[utils_iter] != '\0'; utils_iter++)
        value = (value * 10) + (buffer[utils_iter] - '0');

    return value;
}