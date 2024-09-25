#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"

void print_message(const char *file, const char *function, int32_t line, const char *message);
void open_debug_file(string_t mode);
void close_debug_file();
void print_debug_message(const char *file, const char *function, int32_t line, const char *message);
void *get_memory(int32_t size);
int32_t get_integer_from_user_input();
int32_t get_student_id_in_type_int(string_t id);
string_t int_to_string(int32_t number);

/*
 *   'iter' will be frequently used to iterate loops in various functions
 */
FILE *debug_file = NULL;
bool_t is_null_alloc_returned = false;
bool_t is_database_tampered = false;
bool_t is_data_being_loaded_from_file = false;
const char *check_message = "Okay";
const char *student_id_format = "BDCOM***";

void print_message(const char *file, const char *function, int32_t line, const char *message)
{
    printf("%s, %s, %d, %s\n", file, function, line, message);

    return;
}

void open_debug_file(string_t mode)
{
    debug_file = fopen(DEBUG_FILE_NAME, mode);

    if (debug_file == NULL)
    {
        print_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_OPEN_MSG);
        fflush(stdout);

        return;
    }

    return;
}

void close_debug_file()
{
    fclose(debug_file);

    if (debug_file != NULL)
    {
        /*print_debug_message(__FILE__, __FUNCTION__, __LINE__, debug_FILE_NOT_CLOSE_MSG);*/

        return;
    }

    debug_file = NULL;

    return;
}

/*
 *   This function opens a debug file and writes the debug messages from various functions
 */

void print_debug_message(const char *file, const char *function, int32_t line, const char *message)
{
    open_debug_file("a");
    fprintf(debug_file, "%s, %s, %d, %s\n", file, function, line, message);
    fflush(stdout);
    close_debug_file();

    return;
}

/*
 *   This function allocates a heap memory of size 'size'
 */

void *get_memory(int32_t size)
{
    void *new = calloc(1, size);

    if (new == NULL)
    {
        print_message(__FILE__, __FUNCTION__, __LINE__, NULL_CALLOC_MSG);

        return NULL;
    }

    return new;
}

/*
 *   Takes characters as user input and constructs a number from it
 */

int32_t get_integer_from_user_input()
{
    int32_t value = 0;
    char input = 0;
    char buffer[10] = {0};
    bool_t valid_input = true;
    int32_t iter = 0;

    do
    {
        scanf("%c", &input);

        if ((input != '\n') && (input < '0' || input > '9'))
            valid_input = false;

        /*
         *   Only if the input contains valid character, it can be inserted into buffer
         *   However, the first 9 numbers will be stored, rest will be discarded
         */

        if (valid_input == true && input != '\n' && iter < 10)
        {
            buffer[iter] = input;
            iter++;
        }

    } while (input != '\n');

    buffer[iter] = '\0';

    /*
     *   If the user input contains an invalid character, it returns -1
     */

    if (valid_input == false)
        return -1;

    for (iter = 0; buffer[iter] != '\0'; iter++)
        value = (value * 10) + (buffer[iter] - '0');

    return value;
}

string_t int_to_string(int32_t number)
{
    int32_t num_of_digits = 0;
    int32_t iter = 0;
    int32_t iter2 = 0;
    char digits[10];
    string_t num_str;

    /*
     *   If the number is 0, only 2-character long string is enough
     */

    if (number == 0)
    {
        num_str = (string_t)calloc(1, 2);
        memcpy(num_str, "0", 2);

        return num_str;
    }

    /*
     *   After the following block, if number = 123, digits[] = {3,2,1}
     */

    for (iter = 0; iter < 10 && number > 0; iter++)
    {
        digits[iter] = '0' + (number % 10);
        number /= 10;
        num_of_digits++;
    }
    digits[iter] = '\0';

    /*
     *   After the following block, if number = 123, digits[] = {1,2,3,'\0'}
     */

    for (iter = 0, iter2 = num_of_digits - 1; iter < iter2; iter++, iter2--)
    {
        char tmp = digits[iter];
        digits[iter] = digits[iter2];
        digits[iter2] = tmp;
    }

    num_str = (string_t)calloc(1, strlen(digits) + 1);
    memcpy(num_str, digits, num_of_digits + 1);

    return num_str;
}

/*
 *   If student id is 'BDCOM123', it extracts the integer 123 from it
 */

int32_t get_student_id_in_type_int(string_t id)
{
    int32_t iter = 0;
    int32_t int_id = 0;

    for (iter = 5; iter < MAX_STUDENT_ID_LENGTH - 1; iter++)
        int_id = (int_id * 10) + (id[iter] - '0');

    return int_id;
}