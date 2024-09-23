#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"
#include "../h_files/ui.h"
#include "../h_files/department.h"
#include "../h_files/student.h"
#include "../h_files/grade.h"
#include "../h_files/inter_relations.h"

void clear_menu();
void print_middle_aligned(const char *text, int32_t given_width);
void print_header();
void load_all_data();
void get_integer_from_user_until_valid_value_is_given(int32_t *value, int32_t min_value, int32_t max_value, const char *message);
void ui_message_and_user_input_area();
void ui_main_menu();
void ui_department_management();
void ui_student_management();
void ui_grade_management();
void save_process();
void exit_process();
void get_student_id_user_input(string_t id_buffer);
void add_new_department_process();
void add_new_student_process();
void add_new_grade_process();
void update_department_process();
void update_student_process(int32_t what_to_update);
void update_grade_process(int what_to_update);
void display_departments();
void display_students();
void display_grades();
void display_table(fptr_table_displayer displayer);
void delete_department_process();
void delete_student_process();
void delete_grade_process();

/*
 *   'iter' will be frequently used to iterate loops in various functions
 */
static int32_t iter = 0;
static int32_t input_character_count = 0;
const char *app_name = "STUDENT INFORMATION MANAGEMENT";
/*
 *   'menu message' will be frequently used to show info and error messages on console
 */
const char *menu_message = "";
/*
 *   'is_exit' will be true if exit is selected by the users
 */
bool_t is_exit = false;

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

void print_middle_aligned(const char *text, int32_t given_width)
{
    for (iter = 0; iter < (given_width - strlen(text)) / 2; iter++)
        printf(" ");

    printf("%s", text);
    iter += strlen(text);

    for (; iter <= given_width; iter++)
        printf(" ");

    return;
}

void print_header()
{
    clear_menu();

    for (iter = 0; iter <= MAX_MENU_WIDTH; iter++)
        printf("*");

    printf("\n");
    print_middle_aligned(app_name, MAX_MENU_WIDTH);
    printf("\n");

    for (iter = 0; iter <= MAX_MENU_WIDTH; iter++)
        printf("*");

    printf("\n");

    return;
}

void load_all_data()
{
    load_department_list_from_file();

    if (is_database_tampered == true)
    {
        menu_message = database_tampered_msg;
        return;
    }

    load_student_list_from_file();

    if (is_database_tampered == true)
    {
        menu_message = database_tampered_msg;
        return;
    }

    load_grade_list_from_file();

    if (is_database_tampered == true)
        menu_message = database_tampered_msg;

    return;
}

/*
 *  This function keeps taking user input until the given value is within the valid range
 *  The range is set by 'min_value' and 'max_value'
 *  The 'message' is what to be shown if an invalid input is given
 */

void get_integer_from_user_until_valid_value_is_given(int32_t *value, int32_t min_value, int32_t max_value, const char *message)
{
    input_character_count = 0;

    do
    {
        if (input_character_count != 0)
            printf("%s\n>> ", message);

        *value = get_integer_from_user_input();
        input_character_count++;

    } while (*value < min_value || *value > max_value);

    input_character_count = 0;

    return;
}

void ui_message_and_user_input_area()
{
    for (iter = 0; iter <= MAX_MENU_WIDTH; iter++)
        printf("*");

    printf("\n");

    if (strlen(menu_message) != 0)
        printf("%s\n", menu_message);

    printf("Insert your command here: ");
    printf("\n");

    return;
}

void ui_main_menu()
{
    print_header();
    print_middle_aligned("Main Menu", MAX_MENU_WIDTH);
    printf("\n");
    printf("\n");
    printf("1. Department Management\n");
    printf("2. Student Management\n");
    printf("3. Grade Management\n");
    printf("4. Save Data\n");
    printf("5. Exit\n");

    ui_message_and_user_input_area();

    return;
}

void ui_department_management()
{
    print_header();
    print_middle_aligned("Department Management", MAX_MENU_WIDTH);
    printf("\n");
    printf("\n");
    printf("1. Add New Department\n");
    printf("2. Delete Department\n");
    printf("3. Update Department\n");
    printf("4. Display All Departments\n");
    printf("5. Main Menu\n");
    printf("6. Exit\n");

    ui_message_and_user_input_area();

    return;
}

void ui_student_management()
{
    print_header();
    print_middle_aligned("Student Management", MAX_MENU_WIDTH);
    printf("\n");
    printf("\n");
    printf("1. Add New Student\n");
    printf("2. Delete Student\n");
    printf("3. Update Student Name Using Student Id\n");
    printf("4. Update Student Gender Using Student Id\n");
    printf("5. Update Student Department Using Student Id\n");
    printf("6. Display All Students\n");
    printf("7. Main Menu\n");
    printf("8. Exit\n");

    ui_message_and_user_input_area();

    return;
}

void ui_grade_management()
{
    print_header();
    print_middle_aligned("Grade Management", MAX_MENU_WIDTH);
    printf("\n");
    printf("\n");
    printf("1. Add New Grade\n");
    printf("2. Delete Grade\n");
    printf("3. Update English Grade Using Student Id\n");
    printf("4. Update Math Grade Using Student Id\n");
    printf("5. Update History Grade Using Student Id\n");
    printf("6. Display All Grades\n");
    printf("7. Main Menu\n");
    printf("8. Exit\n");

    ui_message_and_user_input_area();

    return;
}

void save_process()
{
    int32_t user_decision = 0;

    printf("Are you sure to save the data in the file?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&user_decision, 1, 2, invalid_input_msg);

    if (user_decision == 1)
    {
        write_departments_in_file();
        write_students_in_file();
        write_grades_in_file();
    }

    return;
}

void exit_process()
{
    int32_t user_decision = 0;

    printf("Are you sure to exit?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&user_decision, 1, 2, invalid_input_msg);

    if (user_decision == 1)
    {
        free_all_data_pointers();
        is_exit = true;
    }

    return;
}

void get_student_id_user_input(string_t id_buffer)
{
    char tmp = 0;

    iter = 0;
    input_character_count = 0;

    /*
     *   This loop will keep taking inputs until 'enter' button is pressed
     *   wiil save the first 8 characters in the buffer as the student id is 8 character-long
     */

    while (tmp != '\n')
    {
        scanf("%c", &tmp);

        if (tmp != '\n')
            input_character_count++;

        if (iter < MAX_STUDENT_ID_LENGTH - 1 && tmp != '\n')
        {
            id_buffer[iter] = tmp;
            iter++;
        }
    }

    /*
     *   if the number of input character is longer than 8, it is invalid input
     */

    if (input_character_count >= MAX_STUDENT_ID_LENGTH)
    {
        menu_message = s_invalid_id_msg;

        return;
    }

    id_buffer[iter] = '\0';
    iter = 0;
    input_character_count = 0;

    return;
}

void add_new_department_process()
{
    char buffer[MAX_DEPARTMENT_NAME_LENGTH];
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_added = false;

    print_middle_aligned("** Add New Department **", 50);
    printf("\n");
    printf("Insert the department name:\n");
    printf("* Max length: 20\n");
    printf("* Allowed characters: A-Z, a-z, underscore, and space\n");
    printf("* Extra space and characters will be discarded\n");
    printf(">> ");
    iter = 0;

    while (tmp != '\n')
    {
        scanf("%c", &tmp);

        /*
         *   multiple consecutive spaces will be truncated into one space in the buffer
         */

        if (tmp != ' ')
            space_count = 0;

        /*
         *  Will only insert the first 20 valid characters in the buffer
         */

        if (iter < MAX_DEPARTMENT_NAME_LENGTH - 1 && tmp != '\n' && space_count == 0)
        {
            buffer[iter] = tmp;
            iter++;
            if (tmp == ' ')
                space_count++;
            else
                space_count = 0;
        }
    }

    buffer[iter] = '\0';
    is_added = add_new_department_handler(0, buffer);

    if (is_added)
        menu_message = add_success_msg;
    else
        menu_message = check_message;

    return;
}

void add_new_student_process()
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    char name_buffer[MAX_STUDENT_NAME_LENGTH] = {0};
    char dept_id_buffer[MAX_DEPARTMENT_ID_CHARACTER_LENGTH] = {0};
    string_t gender_string;
    int gender = 0;
    int dept_id = 0;
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_valid = false;
    bool_t is_added = false;

    print_middle_aligned("** Add New Student **", 50);
    printf("\n");
    printf("Insert the student id:\n");
    printf("* Must start with 'BDCOM' followed by three digits\n");
    printf("* No space is allowed\n");
    printf(">> ");
    get_student_id_user_input(id_buffer);
    is_valid = check_id((void *)id_buffer, 's', false) && check_id((void *)id_buffer, 's', true);

    if (is_valid == false)
    {
        menu_message = check_message;

        return;
    }

    is_valid = false;
    input_character_count = 0;
    printf("\n");
    printf("Insert the student name:\n");
    printf("* Max length: 20\n");
    printf("* Allowed characters: A-Z, a-z, underscore, and space\n");
    printf("* Extra space and characters will be discarded\n");
    printf(">> ");
    iter = 0;
    tmp = 0;

    while (tmp != '\n')
    {
        scanf("%c", &tmp);

        if (tmp != '\n')
            input_character_count++;

        /*
         *   multiple consecutive spaces will be truncated into one space in the buffer
         */

        if (tmp != ' ')
            space_count = 0;

        /*
         *  Will only insert the first 20 valid characters in the buffer
         */

        if (iter < MAX_STUDENT_NAME_LENGTH - 1 && tmp != '\n' && space_count == 0)
        {
            name_buffer[iter] = tmp;
            iter++;
            if (tmp == ' ')
                space_count++;
            else
                space_count = 0;
        }
    }

    if (input_character_count >= MAX_STUDENT_NAME_LENGTH)
    {
        menu_message = d_invalid_name_length_msg;

        return;
    }

    name_buffer[iter] = '\0';
    is_valid = check_student_name_validity(name_buffer);

    if (is_valid == false)
    {
        menu_message = check_message;

        return;
    }

    printf("\n");
    printf("Insert the gender: \n");
    printf("1. Male\n");
    printf("2. Female\n");
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&gender, 1, 2, invalid_input_msg);

    if (gender == 1)
        gender_string = "MALE";
    else if (gender == 2)
        gender_string = "FEMALE";
    else
        ;

    is_valid = false;
    printf("\n");
    printf("Insert the department id: \n");
    printf("* Must be within %d to %d\n", MIN_DEPARTMENT_ID_VALUE, MAX_DEPARTMENT_ID_VALUE);
    printf(">> ");
    get_integer_from_user_until_valid_value_is_given(&dept_id, MIN_DEPARTMENT_ID_VALUE, MAX_DEPARTMENT_ID_VALUE, invalid_input_msg);
    is_valid = check_id((void *)&dept_id, 'd', false) && !check_id((void *)&dept_id, 'd', true);

    if (is_valid == false)
    {
        menu_message = check_message;

        return;
    }

    is_added = insert_new_student_in_the_linked_list(id_buffer, name_buffer, gender_string, dept_id);

    if (is_added)
        menu_message = add_success_msg;
    else
        menu_message = check_message;

    input_character_count = 0;

    return;
}

void add_new_grade_process()
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    int english = -1;
    int math = -1;
    int history = -1;
    char tmp = 0;
    bool_t is_valid = false;
    bool_t is_added = false;

    print_middle_aligned("** Add New Grade **", 50);
    printf("\n");
    printf("Insert the student id:\n");
    printf("* Must start with 'BDCOM' followed by three digits\n");
    printf("* No space is allowed\n");
    printf(">> ");
    get_student_id_user_input(id_buffer);
    is_valid = check_id((void *)id_buffer, 's', false) && !check_id((void *)id_buffer, 's', true) && !check_id((void *)id_buffer, 'g', 0);

    if (is_valid == false)
    {
        menu_message = check_message;

        return;
    }

    is_valid = false;
    input_character_count = 0;

    printf("\n");
    printf("* Marks must be within %d to %d\n", MIN_GRADE_VALUE, MAX_GRADE_VALUE);
    printf("Insert the marks of English >> ");
    get_integer_from_user_until_valid_value_is_given(&english, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_english_invalid_marks_msg);

    printf("Insert the marks of Math >> ");
    get_integer_from_user_until_valid_value_is_given(&math, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_math_invalid_marks_msg);

    printf("Insert the marks of History >> ");
    get_integer_from_user_until_valid_value_is_given(&history, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_history_invalid_marks_msg);

    is_added = insert_new_grade_in_the_linked_list(id_buffer, english, math, history);

    if (is_added)
        menu_message = add_success_msg;
    else
        menu_message = check_message;

    input_character_count = 0;

    return;
}

void update_department_process()
{
    int32_t id = 0;
    char buffer[MAX_DEPARTMENT_NAME_LENGTH];
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_updated = false;

    print_middle_aligned("** Update Department **", 50);
    printf("\n");
    printf("Insert department id >> ");
    get_integer_from_user_until_valid_value_is_given(&id, MIN_DEPARTMENT_ID_VALUE, MAX_DEPARTMENT_ID_VALUE, invalid_input_msg);

    if (!check_id((void *)&id, 'd', false) || check_id((void *)&id, 'd', true))
    {
        menu_message = check_message;

        return;
    }

    printf("Insert new name >> ");
    iter = 0;

    while (tmp != '\n')
    {
        scanf("%c", &tmp);

        /*
         *   multiple consecutive spaces will be truncated into one space in the buffer
         */

        if (tmp != ' ')
            space_count = 0;

        /*
         *  Will only insert the first 20 valid characters in the buffer
         */

        if (iter < MAX_DEPARTMENT_NAME_LENGTH - 1 && tmp != '\n' && space_count == 0)
        {
            buffer[iter] = tmp;
            iter++;
            if (tmp == ' ')
                space_count++;
            else
                space_count = 0;
        }
    }

    buffer[iter] = '\0';
    is_updated = update_department_name(id, buffer);

    if (is_updated == true)
        menu_message = update_success_msg;
    else
        menu_message = check_message;

    return;
}

/*
 *   what_to_update = 1 means updating student name, 2 means updating gender and 3 means updating dept_id
 */

void update_student_process(int32_t what_to_update)
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    char name_buffer[MAX_STUDENT_NAME_LENGTH] = {0};
    char dept_id_buffer[MAX_DEPARTMENT_ID_CHARACTER_LENGTH] = {0};
    int gender = 0;
    int dept_id = 0;
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_invalid = false, is_updated = false;

    print_middle_aligned("** Update Student **", 50);
    printf("\n");
    printf("Insert student id >> ");
    get_student_id_user_input(id_buffer);

    if (!check_id(id_buffer, 's', false) || check_id(id_buffer, 's', true))
    {
        menu_message = check_message;

        return;
    }

    if (what_to_update == 1)
    {
        printf("Insert new name >> ");
        iter = 0;
        input_character_count = 0;
        tmp = 0;

        while (tmp != '\n')
        {
            scanf("%c", &tmp);

            if (tmp != '\n')
                input_character_count++;

            if (tmp != ' ')
                space_count = 0;

            if (iter < MAX_STUDENT_NAME_LENGTH - 1 && tmp != '\n' && space_count == 0)
            {
                name_buffer[iter] = tmp;
                iter++;
                if (tmp == ' ')
                    space_count++;
                else
                    space_count = 0;
            }
        }

        if (input_character_count >= MAX_STUDENT_NAME_LENGTH)
        {
            menu_message = d_invalid_name_length_msg;

            return;
        }

        name_buffer[iter] = '\0';
        is_updated = update_student_name_by_id(id_buffer, name_buffer);
    }
    else if (what_to_update == 2)
    {
        printf("Insert new gender:\n");
        printf("1. Male\n");
        printf("2. Female\n");
        printf(">> ");
        get_integer_from_user_until_valid_value_is_given(&gender, 1, 2, invalid_input_msg);
        is_updated = update_student_gender_by_id(id_buffer, gender);
    }
    else if (what_to_update == 3)
    {
        printf("Insert new department id >> ");
        get_integer_from_user_until_valid_value_is_given(&dept_id, MIN_DEPARTMENT_ID_VALUE, MAX_DEPARTMENT_ID_VALUE, invalid_input_msg);
        is_updated = update_student_department_by_id(id_buffer, dept_id);
    }
    else
        ;

    if (is_updated == true)
        menu_message = update_success_msg;
    else
        menu_message = check_message;

    return;
}

void update_grade_process(int what_to_update)
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    char tmp = 0;
    int32_t space_count = 0;
    int32_t new_mark = 0;
    bool_t is_updated = false;

    print_middle_aligned("** Update Grade **", 50);
    printf("\n");
    printf("Insert student id >> ");
    get_student_id_user_input(id_buffer);

    if (!check_id(id_buffer, 's', false) || check_id(id_buffer, 's', true))
    {
        menu_message = check_message;

        return;
    }

    if (what_to_update == 1)
    {
        printf("Insert new English grade >> ");
        get_integer_from_user_until_valid_value_is_given(&new_mark, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_english_invalid_marks_msg);
    }
    else if (what_to_update == 2)
    {
        printf("Insert new Math grade >> ");
        get_integer_from_user_until_valid_value_is_given(&new_mark, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_math_invalid_marks_msg);
    }
    else if (what_to_update == 3)
    {
        printf("Insert new History grade >> ");
        get_integer_from_user_until_valid_value_is_given(&new_mark, MIN_GRADE_VALUE, MAX_GRADE_VALUE, g_history_invalid_marks_msg);
    }
    else
        ;

    is_updated = update_grade_handler(id_buffer, what_to_update, new_mark);

    if (is_updated == true)
        menu_message = update_success_msg;
    else
        menu_message = check_message;
}

void display_departments()
{
    department_node_t *d_curr = d_head;
    int32_t *males = NULL;
    int32_t *females = NULL;
    int32_t iter = 0;
    string_t int_to_str = NULL;

    males = (int32_t *)calloc(maximum_dept_id + 1, sizeof(int32_t));
    females = (int32_t *)calloc(maximum_dept_id + 1, sizeof(int32_t));

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("Department Table", MAX_MENU_WIDTH);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("ID", MAX_MENU_WIDTH / 4);
    printf("|");
    print_middle_aligned("NAME", MAX_MENU_WIDTH / 4);
    printf("|");
    print_middle_aligned("MALES", MAX_MENU_WIDTH / 4);
    printf("|");
    print_middle_aligned("FEMALES", MAX_MENU_WIDTH / 4);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    count_males_n_females_in_each_department(males, females);

    while (d_curr != NULL)
    {
        int_to_str = int_to_string(d_curr->department->id);
        print_middle_aligned(int_to_str, MAX_MENU_WIDTH / 4);
        free(int_to_str);
        printf("|");

        print_middle_aligned(d_curr->department->name, MAX_MENU_WIDTH / 4);
        printf("|");

        int_to_str = int_to_string(males[d_curr->department->id]);
        print_middle_aligned(int_to_str, MAX_MENU_WIDTH / 4);
        free(int_to_str);
        printf("|");

        int_to_str = int_to_string(females[d_curr->department->id]);
        print_middle_aligned(int_to_str, MAX_MENU_WIDTH / 4);
        free(int_to_str);
        printf("\n");

        d_curr = d_curr->next;
    }

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");

    free(males);
    males = NULL;
    free(females);
    females = NULL;
    int_to_str = NULL;

    return;
}

void display_students()
{
    student_node_t *s_curr = NULL;
    department_node_t *d_curr = NULL;
    grade_node_t *g_curr = NULL;
    string_t *dept_names = NULL;
    string_t int_to_str = NULL;

    s_curr = s_head;
    d_curr = d_head;
    g_curr = g_head;
    dept_names = (string_t *)calloc(maximum_dept_id, MAX_DEPARTMENT_NAME_LENGTH);

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("Student Table", MAX_MENU_WIDTH);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("ID", 15);
    printf("|");
    print_middle_aligned("NAME", 25);
    printf("|");
    print_middle_aligned("GENDER", 10);
    printf("|");
    print_middle_aligned("DEPARTMENT NAME", 25);
    printf("|");
    print_middle_aligned("ENGLISH", 11);
    printf("|");
    print_middle_aligned("MATH", 11);
    printf("|");
    print_middle_aligned("HISTORY", 11);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    iter = 0;

    /*
     *  Lists the dept_names for each dept_id in an array
     *  dept[0] contains the name of the department with id 1
     */

    while (d_curr != NULL)
    {
        dept_names[d_curr->department->id - 1] = d_curr->department->name;
        d_curr = d_curr->next;
        iter++;
    }

    while (s_curr != NULL)
    {
        print_middle_aligned(s_curr->student->id, 15);
        printf("|");
        print_middle_aligned(s_curr->student->name, 25);
        printf("|");
        print_middle_aligned((s_curr->student->gender == MALE) ? "MALE" : "FEMALE", 10);
        printf("|");
        print_middle_aligned((s_curr->student->department_id != 0) ? dept_names[s_curr->student->department_id - 1] : "None", 25);
        printf("|");

        while (g_curr != NULL)
        {
            if (!strncmp(g_curr->grade->student_id, s_curr->student->id, MAX_STUDENT_ID_LENGTH))
                break;

            g_curr = g_curr->next;
        }

        if (g_curr == NULL)
        {
            print_middle_aligned("-", 11);
            printf("|");
            print_middle_aligned("-", 11);
            printf("|");
            print_middle_aligned("-", 11);
            printf("\n");
        }
        else
        {
            int_to_str = int_to_string(g_curr->grade->english);
            print_middle_aligned(int_to_str, 11);
            free(int_to_str);
            printf("|");

            int_to_str = int_to_string(g_curr->grade->math);
            print_middle_aligned(int_to_str, 11);
            free(int_to_str);
            printf("|");

            int_to_str = int_to_string(g_curr->grade->history);
            print_middle_aligned(int_to_str, 11);
            free(int_to_str);
            printf("\n");
        }

        g_curr = g_head;
        s_curr = s_curr->next;
    }

    printf("\n");

    free(dept_names);
    dept_names = NULL;

    return;
}

void display_grades()
{
    grade_node_t *g_curr = g_head;
    student_node_t *s_curr = s_head;
    string_t *stud_names = NULL;
    string_t int_to_str = NULL;

    stud_names = (string_t *)calloc(999, MAX_STUDENT_NAME_LENGTH);

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("Grade Table", MAX_MENU_WIDTH);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");
    print_middle_aligned("STUDENT NAME", 30);
    printf("|");
    print_middle_aligned("ENGLISH", 30);
    printf("|");
    print_middle_aligned("MATH", 30);
    printf("|");
    print_middle_aligned("HISTORY", 30);
    printf("\n");

    for (iter = 0; iter < MAX_MENU_WIDTH; iter++)
        printf("-");

    printf("\n");

    /*
     *  Lists the student names for each student_id in an array
     *  stud_names[0] contains the name of the student with id 'BDCOM001'
     */

    while (s_curr != NULL)
    {
        stud_names[get_student_id_in_type_int(s_curr->student->id) - 1] = s_curr->student->name;
        s_curr = s_curr->next;
    }

    while (g_curr != NULL)
    {
        print_middle_aligned(stud_names[get_student_id_in_type_int(g_curr->grade->student_id) - 1], 30);
        printf("|");

        int_to_str = int_to_string(g_curr->grade->english);
        print_middle_aligned(int_to_str, 30);
        free(int_to_str);
        printf("|");

        int_to_str = int_to_string(g_curr->grade->math);
        print_middle_aligned(int_to_str, 30);
        free(int_to_str);
        printf("|");

        int_to_str = int_to_string(g_curr->grade->history);
        print_middle_aligned(int_to_str, 30);
        free(int_to_str);
        printf("\n");

        g_curr = g_curr->next;
    }

    printf("\n");

    free(stud_names);
    stud_names = NULL;

    return;
}

/*
 *  displayer can be display_departments, display_students or display_grades
 */

void display_table(fptr_table_displayer displayer)
{
    int32_t pressed_key = 0;

    print_header();
    displayer();
    printf("\n");
    printf("Press '1' and 'Enter' to go back.\n");

    ui_message_and_user_input_area();

    return;
}

void delete_department_process()
{
    int32_t id = 0, iter = 0;
    bool_t success = false, is_invalid = false;
    char input = 0;
    char buffer[MAX_DEPARTMENT_ID_CHARACTER_LENGTH + 1] = {0};

    print_middle_aligned("** Delete Department **", 50);
    printf("\n");
    printf("Insert department id >> ");
    get_integer_from_user_until_valid_value_is_given(&id, MIN_DEPARTMENT_ID_VALUE, MAX_DEPARTMENT_ID_VALUE, invalid_input_msg);

    if (!check_id((void *)&id, 'd', false) || check_id((void *)&id, 'd', true))
    {
        menu_message = check_message;

        return;
    }

    success = delete_department_from_the_list(id);

    if (success)
        menu_message = delete_success_msg;
    else
        menu_message = check_message;

    return;
}

void delete_student_process()
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_deleted = false;

    print_middle_aligned("** Delete Student **", 50);
    printf("\n");
    printf("Insert student id >> ");
    get_student_id_user_input(id_buffer);

    if (!check_id(id_buffer, 's', false) || check_id(id_buffer, 's', true))
    {
        menu_message = check_message;

        return;
    }

    is_deleted = delete_student_from_the_list(id_buffer);

    if (is_deleted)
        menu_message = delete_success_msg;
    else
        menu_message = check_message;

    return;
}

void delete_grade_process()
{
    char id_buffer[MAX_STUDENT_ID_LENGTH] = {0};
    char tmp = 0;
    int32_t space_count = 0;
    bool_t is_deleted = false;

    print_middle_aligned("** Delete Grade **", 50);
    printf("\n");
    printf("Insert student id >> ");
    get_student_id_user_input(id_buffer);

    if (!check_id(id_buffer, 's', false) || check_id(id_buffer, 's', true))
    {
        menu_message = check_message;

        return;
    }

    is_deleted = delete_grade_from_the_list(id_buffer);

    if (is_deleted)
        menu_message = delete_success_msg;
    else
        menu_message = check_message;

    return;
}