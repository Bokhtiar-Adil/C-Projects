#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../h_files/utils.h"

static i32 utils_iter = 0;
const char *utils_check_message = "";

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