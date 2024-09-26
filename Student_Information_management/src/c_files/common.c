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
static int32_t iter = 0;
FILE *debug_file = NULL;
bool_t is_null_alloc_returned = false;
bool_t is_database_tampered = false;
const char *debug_file_name = "../txt_files/debug_messages.txt";
const char *departments_file_name = "../txt_files/departments.txt";
const char *students_file_name = "../txt_files/students.txt";
const char *grades_file_name = "../txt_files/grades.txt";
const char *check_message = "Okay";
const char *student_id_format = "BDCOM***";
const char *database_tampered_msg = "INFO: Some data was not loaded due to tampered database";
const char *null_calloc_msg = "INFO: calloc returns null pointer";
const char *create_succees_msg = "INFO: List root created successfully";
const char *null_root_msg = "INFO: null root";
const char *null_node_msg = "INFO: null node";
const char *invalid_position_msg = "INFO: Invalid position";
const char *insert_success_msg = "INFO: INFO:Data inserted successfully";
const char *value_not_found_msg = "INFO: could not find the given value";
const char *add_success_msg = "INFO: Data added successfully";
const char *delete_success_msg = "INFO: Data deleted successfully";
const char *update_success_msg = "INFO: Data updated successfully";
const char *debug_file_not_open_msg = "INFO: Debug file did not open";
const char *file_not_open_msg = "INFO:  File did not open";
const char *debug_file_not_close_msg = "INFO: Debug file did not close";
const char *file_not_close_msg = "INFO: File did not close";
const char *invalid_input_msg = "INFO: invalid input";
const char *valid_input_msg = "INFO: Valid input";
const char *d_invalid_id_msg = "INFO: Department id must be within 1 to 99";
const char *d_id_exists_msg = "INFO: Department id already exists";
const char *d_name_exists_msg = "INFO: Department name already exists";
const char *d_id_not_exist_msg = "INFO: Department id does not exists";
const char *d_invalid_name_length_msg = "INFO: Department name must be within 20 characters";
const char *d_invalid_name_syntax_msg = "INFO: Department name can only contain A-Z, a-z, space, and underscores";
const char *s_invalid_id_msg = "INFO: Student id must start with 'BDCOM' followed by three digits";
const char *s_id_exists_msg = "INFO: Student id already exists";
const char *s_id_not_exist_msg = "INFO: Student id does not exists";
const char *s_invalid_name_length_msg = "INFO: Student name must be within 20 characters";
const char *s_invalid_name_syntax_msg = "INFO: Student name can only contain A-Z, a-z, space, and underscores";
const char *s_invalid_gender_msg = "INFO: Gender must be either 'MALE' or 'FEMALE'";
const char *g_id_exists_msg = "INFO: Grade for this student id already exists";
const char *g_id_not_exist_msg = "INFO: Grade for this student id does not exists";
const char *g_english_invalid_marks_msg = "INFO: English grade must be within 0 to 100";
const char *g_math_invalid_marks_msg = "INFO: Math grade must be within 0 to 100";
const char *g_history_invalid_marks_msg = "INFO: History grade must be within 0 to 100";

void print_message(const char *file, const char *function, int32_t line, const char *message)
{
    printf("%s, %s, %d, %s\n", file, function, line, message);

    return;
}

void open_debug_file(string_t mode)
{
    debug_file = fopen(debug_file_name, mode);

    if (debug_file == NULL)
    {
        print_message(__FILE__, __FUNCTION__, __LINE__, file_not_open_msg);
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
        /*print_debug_message(__FILE__, __FUNCTION__, __LINE__, debug_file_not_close_msg);*/

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
        print_message(__FILE__, __FUNCTION__, __LINE__, null_calloc_msg);

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
    iter = 0;

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
    int32_t num_of_digits = 0, iter2 = 0;
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
    int32_t int_id = 0;

    for (iter = 5; iter < MAX_STUDENT_ID_LENGTH; iter++)
        int_id = (int_id * 10) + (id[iter] - '0');

    return int_id;
}