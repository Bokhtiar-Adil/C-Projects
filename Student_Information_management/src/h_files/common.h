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
extern const char *debug_file_name;
extern const char *departments_file_name;
extern const char *students_file_name;
extern const char *grades_file_name;
extern const char *check_message;
extern const char *student_id_format;
extern const char *database_tampered_msg;
extern const char *null_calloc_msg;
extern const char *create_succees_msg;
extern const char *null_root_msg;
extern const char *null_node_msg;
extern const char *invalid_position_msg;
extern const char *insert_success_msg;
extern const char *value_not_found_msg;
extern const char *add_success_msg;
extern const char *delete_success_msg;
extern const char *update_success_msg;
extern const char *debug_file_not_open_msg;
extern const char *file_not_open_msg;
extern const char *debug_file_not_close_msg;
extern const char *file_not_close_msg;
extern const char *invalid_input_msg;
extern const char *valid_input_msg;
extern const char *d_invalid_id_msg;
extern const char *d_id_exists_msg;
extern const char *d_name_exists_msg;
extern const char *d_id_not_exist_msg;
extern const char *d_invalid_name_length_msg;
extern const char *d_invalid_name_syntax_msg;
extern const char *s_invalid_id_msg;
extern const char *s_id_exists_msg;
extern const char *s_id_not_exist_msg;
extern const char *s_invalid_name_length_msg;
extern const char *s_invalid_name_syntax_msg;
extern const char *s_invalid_gender_msg;
extern const char *g_id_exists_msg;
extern const char *g_id_not_exist_msg;
extern const char *g_english_invalid_marks_msg;
extern const char *g_math_invalid_marks_msg;
extern const char *g_history_invalid_marks_msg;

void print_message(const char *file, const char *function, int32_t line, const char *message);
void open_debug_file(string_t mode);
void close_debug_file();
void print_debug_message(const char *file, const char *function, int32_t line, const char *message);
void *get_memory(int32_t size);
int32_t get_integer_from_user_input();
int32_t get_student_id_in_type_int(string_t id);
string_t int_to_string(int32_t number);

#endif /* __COMMON_H__ */