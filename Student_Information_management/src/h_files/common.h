#ifndef __COMMON_H__
#define __COMMON_H__

#define MAX_STUDENT_ID_LENGTH 9
#define MAX_STUDENT_NAME_LENGTH 21
#define MAX_STUDENT_GENDER_STRING_LENGTH 7
#define MIN_DEPARTMENT_ID_VALUE 1
#define MAX_DEPARTMENT_ID_VALUE 99
#define MAX_DEPARTMENT_ID_CHARACTER_LENGTH 3
#define MAX_DEPARTMENT_NAME_LENGTH 21
#define MIN_GRADE_VALUE 0
#define MAX_GRADE_VALUE 100
#define MAX_MENU_WIDTH 120
#define MAX_BUFFER_SIZE_USED_FOR_READING_FROM_FILE 100
#define USER_INPUT_OPTION_ONE 1
#define USER_INPUT_OPTION_TWO 2
#define USER_INPUT_OPTION_THREE 3
#define USER_INPUT_OPTION_FOUR 4
#define USER_INPUT_OPTION_FIVE 5
#define USER_INPUT_OPTION_SIX 6
#define USER_INPUT_OPTION_SEVEN 7
#define USER_INPUT_OPTION_EIGHT 8
#define DEBUG_FILE_NAME "../txt_files/debug_messages.txt"
#define DEPARTMENTS_FILE_NAME "../txt_files/departments.txt"
#define STUDENTS_FILE_NAME "../txt_files/students.txt"
#define GRADES_FILE_NAME "../txt_files/grades.txt"
#define DATABASE_TAMPERED_MSG "INFO: Some data was not loaded due to tampered database"
#define NULL_CALLOC_MSG "INFO: calloc returns null pointer"
#define CREATE_SUCCESS_MSG "INFO: List root created successfully"
#define NULL_ROOT_MSG "INFO: null root"
#define NULL_NODE_MSG "INFO: null node"
#define INVALID_POSITION_MSG "INFO: Invalid position"
#define INSERT_SUCCESS_MSG "INFO: INFO:Data inserted successfully"
#define VALUE_NOT_FOUND_MSG "INFO: could not find the given value"
#define ADD_SUCCESS_MSG "INFO: Data added successfully"
#define DELETE_SUCCESS_MSG "INFO: Data deleted successfully"
#define UPDATE_SUCCESS_MSG "INFO: Data updated successfully"
#define DEBUG_FILE_NOT_OPEN_MSG "INFO: Debug file did not open"
#define FILE_NOT_OPEN_MSG "INFO:  File did not open"
#define DEBUG_FILE_NOT_CLOSE_MSG "INFO: Debug file did not close"
#define FILE_NOT_CLOSE_MSG "INFO: File did not close"
#define INVALID_INPUT_MSG "INFO: invalid input"
#define VALID_INPUT_MSG "INFO: Valid input"
#define D_INVALID_ID_MSG "INFO: Department id must be within 1 to 99"
#define D_ID_EXISTS_MSG "INFO: Department id already exists"
#define D_NAME_EXISTS_MSG "INFO: Department name already exists"
#define D_ID_NOT_EXISTS_MSG "INFO: Department id does not exist"
#define D_INVALID_NAME_LENGTH_MSG "INFO: Department name must be within 20 characters"
#define D_INVALID_NAME_SYNTAX_MSG "INFO: Department name can only contain A-Z, a-z, space, and underscores"
#define S_INVALID_ID_MSG "INFO: Student id must start with 'BDCOM' followed by three digits"
#define S_ID_EXISTS_MSG "INFO: Student id already exists"
#define S_ID_NOT_EXISTS_MSG "INFO: Student id does not exists"
#define S_INVALID_NAME_LENGTH_MSG "INFO: Student name must be within 20 characters"
#define S_INVALID_NAME_SYNTAX_MSG "INFO: Student name can only contain A-Z, a-z, space, and underscores"
#define S_INVALID_GENDER_MSG "INFO: Gender must be either 'MALE' or 'FEMALE'"
#define G_ID_EXISTS_MSG "INFO: Grade for this student id already exists"
#define G_ID_NOT_EXISTS_MSG "INFO: Grade for this student id does not exist"
#define G_ENGLISH_INVALID_MARKS_MSG "INFO: English grade must be within 0 to 100"
#define G_MATH_INVALID_MARKS_MSG "INFO: Math grade must be within 0 to 100"
#define G_HISTORY_INVALID_MARKS_MSG "INFO: History grade must be within 0 to 100"

typedef char *string_t;

typedef enum bool
{
    false = 0,
    true
} bool_t;

typedef enum gender
{
    MALE = 0,
    FEMALE
} gender_t;

typedef void (*fptr_ui_loader)();
typedef void (*fptr_table_displayer)();

typedef struct student
{
    string_t id;
    string_t name;
    gender_t gender;
    int32_t department_id;
} student_t;

typedef struct department
{
    int32_t id;
    string_t name;
} department_t;

typedef struct grade
{
    string_t student_id;
    int32_t english;
    int32_t math;
    int32_t history;
} grade_t;

typedef struct student_node
{
    student_t *student;
    struct student_node *prev;
    struct student_node *next;
} student_node_t;

typedef struct department_node
{
    department_t *department;
    struct department_node *prev;
    struct department_node *next;
} department_node_t;

typedef struct grade_node
{
    grade_t *grade;
    struct grade_node *prev;
    struct grade_node *next;
} grade_node_t;

extern FILE *debug_file;
extern bool_t is_null_alloc_returned;
extern bool_t is_database_tampered;
extern bool_t is_data_being_loaded_from_file;
extern const char *check_message;
extern const char *student_id_format;

void print_message(const char *file, const char *function, int32_t line, const char *message);
void open_debug_file(string_t mode);
void close_debug_file();
void print_debug_message(const char *file, const char *function, int32_t line, const char *message);
void *get_memory(int32_t size);
int32_t get_integer_from_user_input();
int32_t get_student_id_in_type_int(string_t id);
string_t int_to_string(int32_t number);

#endif /* __COMMON_H__ */