#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"
#include "../h_files/department.h"
#include "../h_files/student.h"
#include "../h_files/grade.h"
#include "../h_files/inter_relations.h"

bool_t check_id(void *id, char entity, bool_t test_type);
bool_t delete_department_from_the_list(int32_t id);
bool_t delete_student_from_the_list(string_t id);
bool_t update_student_department_by_id(string_t id, int32_t new_dept);
bool_t update_grade_handler(string_t id, int32_t which_to_update, int32_t updated_value);
void count_males_n_females_in_each_department(int32_t *males, int32_t *females);
void free_all_data_pointers();
void load_department_list_from_file();
void load_student_list_from_file();
void load_grade_list_from_file();

/*
 *  This function checks the validity of student id, department id, and grade id
 *  enitity = 's' means student, entity = 'd' means deparment, and entity = 'g' means grade
 *  test_type = true means checking whether the given id exists in the linked list or not
 *  test_type = false means checking the syntax of the given id
 */

bool_t check_id(void *id, char entity, bool_t test_type)
{
    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "begin");

    if (entity == 's')
    {
        if (test_type)
            return check_student_id_is_unique((string_t)id);
        else
            return check_student_id_syntax_validity((string_t)id);
    }
    else if (entity == 'd')
    {
        if (test_type)
            return check_department_id_is_unique(*(int32_t *)id);
        else
            return check_department_id_value_validity(*(int32_t *)id);
    }
    else
        return check_is_grade_already_exist((string_t)id);

    return false;
}

bool_t delete_department_from_the_list(int32_t id)
{
    department_node_t *d_curr = NULL;
    student_node_t *s_curr = NULL;

    s_curr = s_head;

    /*
     *  Setting the dept_id of the students in the deleted departments to None i.e. 0
     */

    while (s_curr != NULL)
    {
        if (s_curr->student->department_id == id)
            s_curr->student->department_id = 0;

        s_curr = s_curr->next;
    }

    d_curr = dept_hash_table[id];

    if (d_curr->prev != NULL)
        d_curr->prev->next = d_curr->next;

    if (d_curr->next != NULL)
        d_curr->next->prev = d_curr->prev;

    if (d_curr == d_tail)
        d_tail = (d_curr->next != NULL) ? d_curr->next : d_curr->prev;

    if (d_curr == d_head)
        d_head = (d_curr->next != NULL) ? d_curr->next : NULL;

    dept_hash_table[id] = NULL;
    free(d_curr->department->name);
    d_curr->department->name = NULL;
    free(d_curr->department);
    d_curr->department = NULL;
    d_curr->next = NULL;
    d_curr->prev = NULL;

    return true;
}

bool_t delete_student_from_the_list(string_t id)
{
    student_node_t *s_curr = NULL;

    s_curr = stud_hash_table[get_student_id_in_type_int(id)];
    delete_grade_from_the_list(s_curr->student->id);

    if (s_curr->prev != NULL)
        s_curr->prev->next = s_curr->next;

    if (s_curr->next != NULL)
        s_curr->next->prev = s_curr->prev;

    if (s_curr == s_tail)
        s_tail = (s_curr->next != NULL) ? s_curr->next : s_curr->prev;

    if (s_curr == s_head)
        s_head = (s_curr->next != NULL) ? s_curr->next : NULL;

    total_students--;

    stud_hash_table[get_student_id_in_type_int(id)] = NULL;
    free(s_curr->student->id);
    s_curr->student->id = NULL;
    free(s_curr->student->name);
    s_curr->student->name = NULL;
    free(s_curr->student);
    s_curr->student = NULL;
    s_curr->prev = NULL;
    s_curr->next = NULL;

    return true;
}

bool_t update_student_department_by_id(string_t id, int32_t new_dept)
{
    student_node_t *curr = s_head;

    /*
     *  Checking whether the new department_id is valid or not
     */

    if (!check_id((void *)&new_dept, 'd', false) || check_id((void *)&new_dept, 'd', true))
        return false;

    while (curr != NULL)
    {
        if (!strncmp(curr->student->id, id, MAX_STUDENT_ID_LENGTH - 1))
            break;

        curr = curr->next;
    }

    curr->student->department_id = new_dept;

    return true;
}

bool_t update_grade_handler(string_t id, int32_t which_to_update, int32_t updated_value)
{
    grade_node_t *g_curr = NULL;

    g_curr = g_head;
    check_message = "";

    while (g_curr != NULL)
    {
        if (!strncmp(g_curr->grade->student_id, id, MAX_STUDENT_ID_LENGTH - 1))
            break;

        g_curr = g_curr->next;
    }

    if (g_curr == NULL)
    {
        check_message = G_ID_NOT_EXISTS_MSG;

        return false;
    }

    switch (which_to_update)
    {
    case 1:
        g_curr->grade->english = updated_value;
        break;
    case 2:
        g_curr->grade->math = updated_value;
        break;
    case 3:
        g_curr->grade->history = updated_value;
        break;
    default:
        check_message = INVALID_INPUT_MSG;

        return false;
    }

    return true;
}

void count_males_n_females_in_each_department(int32_t *males, int32_t *females)
{
    student_node_t *curr = s_head;

    while (curr != NULL)
    {
        if (curr->student->gender == MALE)
            males[curr->student->department_id]++;
        else
            females[curr->student->department_id]++;

        curr = curr->next;
    }

    return;
}

void free_all_data_pointers()
{
    student_node_t *s_curr = s_head, *s_prev = s_head;
    department_node_t *d_curr = d_head, *d_prev = d_head;
    grade_node_t *g_curr = g_head, *g_prev = g_head;

    while (s_curr != NULL)
    {
        free(s_curr->student->id);
        s_curr->student->id = NULL;
        free(s_curr->student->name);
        s_curr->student->name = NULL;
        free(s_curr->student);
        s_curr->student = NULL;
        s_curr->prev = NULL;
        s_prev = s_curr;
        s_curr = s_curr->next;
        s_prev->next = NULL;
        s_prev = NULL;
    }

    while (d_curr != NULL)
    {
        free(d_curr->department->name);
        d_curr->department->name = NULL;
        free(d_curr->department);
        d_curr->department = NULL;
        d_curr->prev = NULL;
        d_prev = d_curr;
        d_curr = d_curr->next;
        d_prev->next = NULL;
        d_prev = NULL;
    }

    while (g_curr != NULL)
    {
        g_curr->grade->student_id = NULL;
        free(g_curr->grade);
        g_curr->grade = NULL;
        g_curr->prev = NULL;
        g_prev = g_curr;
        g_curr = g_curr->next;
        g_prev->next = NULL;
        g_prev = NULL;
    }

    free(dept_hash_table);
    free(stud_hash_table);
    free(grade_hash_table);

    return;
}

void load_department_list_from_file()
{
    char buffer[MAX_BUFFER_SIZE_USED_FOR_READING_FROM_FILE] = {0};
    char tmp_name[MAX_DEPARTMENT_NAME_LENGTH] = {0};
    int32_t tmp_id = 0;
    department_t *tmp_department = NULL;
    department_node_t *new_department_node = NULL;
    department_node_t *curr = NULL;
    FILE *fp = NULL;
    int32_t iter = 0;
    int32_t iter2 = 0;
    int32_t len = 0;
    bool_t id_done = false;
    bool_t name_done = false;
    bool_t is_invalid_character_found = false;

    fp = fopen(DEPARTMENTS_FILE_NAME, "r");

    if (fp == NULL)
    {
        print_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_OPEN_MSG);

        return;
    }

    maximum_dept_id = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        /*
         *  iter is used for iterating the buffer
         *  iter2 is used for other tmp_*** strings
         */

        iter = 0;
        iter2 = 0;
        len = strlen(buffer);
        tmp_id = 0;
        id_done = false;
        is_invalid_character_found = false;

        /*
         *  in each operation the following loop will check one character from the buffer to find
         *  any irregularity in the database
         */

        while (iter < len)
        {
            if (!id_done)
            {
                if (buffer[iter] == ',')
                {
                    id_done = true;
                    iter++;
                    continue;
                }
                else if (buffer[iter] < '0' || buffer[iter] > '9')
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    ;

                /*
                 *  if dept_id = 10
                 *  first iteration ==> tmp_id = 0*10 + '1' - '0' = 1;
                 *  second interation ==> *tmp_id = 1*10 + '0'-'0' = 10;
                 *  then it finds comma
                 */

                tmp_id = tmp_id * 10 + (buffer[iter] - '0');
            }
            else
            {
                if (buffer[iter] == '\n')
                {
                    name_done = true;
                    tmp_name[iter2] = '\0';
                    iter++;
                    break;
                }
                else if (!((buffer[iter] >= 'a' && buffer[iter] <= 'z') || (buffer[iter] >= 'A' && buffer[iter] <= 'Z') || buffer[iter] == ' ' || buffer[iter] == '_'))
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    ;
                tmp_name[iter2] = buffer[iter];
                iter2++;
            }

            iter++;
        }

        /*
         *  performing further checking on the data
         */

        is_database_tampered = !id_done || !name_done || is_invalid_character_found || !check_id((void *)&tmp_id, 'd', false) || !check_id((void *)&tmp_id, 'd', true);

        if (is_database_tampered)
            break;

        tmp_department = create_department(tmp_id, tmp_name, true);
        new_department_node = create_department_node(tmp_department);
        insert_department_at_the_end(new_department_node);

        if (tmp_id > maximum_dept_id)
            maximum_dept_id = tmp_id;
    }

    if (fclose(fp) != 0)
        print_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_CLOSE_MSG);

    fp = NULL;

    dept_hash_table_size = maximum_dept_id * 2;
    dept_hash_table = (department_node_t **)get_memory(dept_hash_table_size * sizeof(department_node_t *));
    curr = d_head;

    while (curr != NULL)
    {
        dept_hash_table[curr->department->id] = curr;
        curr = curr->next;
    }

    return;
}

void load_student_list_from_file()
{
    char buffer[100] = {0};
    char tmp_id[MAX_STUDENT_ID_LENGTH] = {0};
    char tmp_name[MAX_STUDENT_NAME_LENGTH] = {0};
    char tmp_gender[MAX_STUDENT_GENDER_STRING_LENGTH] = {0};
    int32_t tmp_dept_id = 0;
    student_t *tmp_student = NULL;
    student_node_t *new_student_node = NULL;
    student_node_t *s_curr = NULL;
    FILE *fp = NULL;
    int32_t iter = 0;
    int32_t iter2 = 0;
    int32_t len = 0;
    bool_t id_done = false;
    bool_t name_done = false;
    bool_t gender_done = false;
    bool_t dept_done = false;
    bool_t is_invalid_character_found = false;

    fp = fopen(STUDENTS_FILE_NAME, "r");

    if (fp == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_OPEN_MSG);

        return;
    }

    total_students = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        /*
         *  iter is used for iterating the buffer
         *  iter2 is used for other tmp_*** strings
         */

        iter = 0;
        iter2 = 0;
        len = strlen(buffer);
        tmp_dept_id = 0;
        id_done = false;
        name_done = false;
        gender_done = false;
        dept_done = false;
        is_invalid_character_found = false;

        /*
         *  in each operation the following loop will check one character from the buffer to find
         *  any irregularity in the database
         */

        while (iter < len)
        {
            if (!id_done)
            {
                if (buffer[iter] == ',')
                {
                    id_done = true;
                    tmp_id[iter2] = '\0';
                    iter2 = 0;
                }
                else if (!((buffer[iter] >= 'A' && buffer[iter] <= 'Z') || (buffer[iter] >= 'a' && buffer[iter] <= 'z') || (buffer[iter] >= '0' && buffer[iter] <= '9')))
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                {
                    tmp_id[iter2] = buffer[iter];
                    iter2++;
                }
            }
            else if (!name_done)
            {
                if (buffer[iter] == ',')
                {
                    name_done = true;
                    tmp_name[iter2] = '\0';
                }
                else if (!((buffer[iter] >= 'a' && buffer[iter] <= 'z') || (buffer[iter] >= 'A' && buffer[iter] <= 'Z') || buffer[iter] == ' ' || buffer[iter] == '_'))
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                {
                    tmp_name[iter2] = buffer[iter];
                    iter2++;
                }
            }
            else if (!gender_done)
            {
                if (buffer[iter] == 'M' && buffer[iter + 1] == 'A' && buffer[iter + 2] == 'L' && buffer[iter + 3] == 'E' && buffer[iter + 4] == ',')
                {
                    gender_done = true;
                    iter += 4;
                    tmp_gender[0] = 'M';
                    tmp_gender[1] = 'A';
                    tmp_gender[2] = 'L';
                    tmp_gender[3] = 'E';
                    tmp_gender[4] = '\0';
                }
                else if (buffer[iter] == 'F' && buffer[iter + 1] == 'E' && buffer[iter + 2] == 'M' && buffer[iter + 3] == 'A' && buffer[iter + 4] == 'L' && buffer[iter + 5] == 'E' && buffer[iter + 6] == ',')
                {
                    gender_done = true;
                    iter += 6;
                    tmp_gender[0] = 'F';
                    tmp_gender[1] = 'E';
                    tmp_gender[2] = 'M';
                    tmp_gender[3] = 'A';
                    tmp_gender[4] = 'L';
                    tmp_gender[5] = 'E';
                    tmp_gender[6] = '\0';
                }
                else
                {
                    is_invalid_character_found = true;
                    break;
                }
            }
            else
            {
                if (buffer[iter] == '\n')
                    dept_done = true;
                else if (buffer[iter] < '0' || buffer[iter] > '9')
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    tmp_dept_id = tmp_dept_id * 10 + (buffer[iter] - '0');

                /*
                 *  if dept_id = 10
                 *  first iteration ==> tmp_dept_id = 0*10 + '1' - '0' = 1;
                 *  second interation ==> *tmp_dept_id = 1*10 + '0'-'0' = 10;
                 *  then it finds comma
                 */
            }

            iter++;
        }

        /*
         *  performing further checking on the data
         */

        is_database_tampered = !id_done || !name_done || !gender_done || !dept_done || is_invalid_character_found || !check_id((void *)&tmp_id, 's', false) || !check_id((void *)&tmp_id, 's', true) || !check_id((void *)&tmp_dept_id, 'd', false) || check_id((void *)&tmp_dept_id, 'd', true) || !check_student_name_validity(tmp_name);

        if (is_database_tampered)
            break;

        tmp_student = create_student(tmp_id, tmp_name, tmp_gender, tmp_dept_id);
        new_student_node = create_student_node(tmp_student);
        insert_student_at_the_end(new_student_node);
        total_students++;
    }

    if (fclose(fp) != 0)
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_CLOSE_MSG);

    fp = NULL;

    stud_and_grade_hash_table_size = get_student_id_in_type_int(s_tail->student->id) * 2;
    stud_hash_table = (student_node_t **)get_memory(stud_and_grade_hash_table_size * sizeof(student_node_t *));
    s_curr = s_head;

    while (s_curr != NULL)
    {
        stud_hash_table[get_student_id_in_type_int(s_curr->student->id)] = s_curr;
        s_curr = s_curr->next;
    }

    return;
}

void load_grade_list_from_file()
{
    char buffer[100] = {0};
    char tmp_id[MAX_STUDENT_ID_LENGTH] = {0};
    int32_t tmp_english = 0;
    int32_t tmp_math = 0;
    int32_t tmp_history = 0;
    int32_t iter = 0;
    int32_t iter2 = 0;
    int32_t len = 0;
    grade_t *tmp_grade = NULL;
    grade_node_t *new_grade_node;
    grade_node_t *g_curr = NULL;
    bool_t id_done = false;
    bool_t english_done = false;
    bool_t math_done = false;
    bool_t history_done = false;
    bool_t is_invalid_character_found = false;
    FILE *fp = NULL;

    fp = fopen(GRADES_FILE_NAME, "r");

    if (fp == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_OPEN_MSG);

        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        iter = 0;
        iter2 = 0;
        len = strlen(buffer);
        id_done = false;
        english_done = false;
        math_done = false;
        history_done = false;
        is_invalid_character_found = false;
        tmp_english = 0;
        tmp_math = 0;
        tmp_history = 0;

        while (iter < len)
        {
            if (!id_done)
            {
                if (buffer[iter] == ',')
                {
                    id_done = true;
                    tmp_id[iter2] = '\0';
                    iter2 = 0;
                }
                else if (!((buffer[iter] >= 'A' && buffer[iter] <= 'Z') || (buffer[iter] >= 'a' && buffer[iter] <= 'z') || (buffer[iter] >= '0' && buffer[iter] <= '9')))
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                {
                    tmp_id[iter2] = buffer[iter];
                    iter2++;
                }
            }
            else if (!english_done)
            {
                if (buffer[iter] == ',')
                    english_done = true;
                else if (buffer[iter] < '0' || buffer[iter] > '9')
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    tmp_english = tmp_english * 10 + (buffer[iter] - '0');
            }
            else if (!math_done)
            {
                if (buffer[iter] == ',')
                    math_done = true;
                else if (buffer[iter] < '0' || buffer[iter] > '9')
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    tmp_math = tmp_math * 10 + (buffer[iter] - '0');
            }
            else
            {
                if (buffer[iter] == '\n')
                    history_done = true;
                else if (buffer[iter] < '0' || buffer[iter] > '9')
                {
                    is_invalid_character_found = true;
                    break;
                }
                else
                    tmp_history = tmp_history * 10 + (buffer[iter] - '0');
            }

            iter++;
        }

        /*
         *  checking all possible error on input data
         */

        is_database_tampered = is_invalid_character_found || !id_done || !english_done || !math_done || !history_done || !check_id((void *)tmp_id, 's', false) || check_id((void *)tmp_id, 's', true) || check_id((void *)tmp_id, 'g', 0) || !(tmp_english >= MIN_GRADE_VALUE && tmp_english <= MAX_GRADE_VALUE) || !(tmp_math >= MIN_GRADE_VALUE && tmp_math <= MAX_GRADE_VALUE) || !(tmp_history >= MIN_GRADE_VALUE && tmp_history <= MAX_GRADE_VALUE);

        if (is_database_tampered)
            break;

        tmp_grade = create_grade(tmp_id, tmp_english, tmp_math, tmp_history);
        new_grade_node = create_grade_node(tmp_grade);
        insert_grade_at_the_end(new_grade_node);
    }

    if (fclose(fp) != 0)
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, FILE_NOT_CLOSE_MSG);

    fp = NULL;

    grade_hash_table = (grade_node_t **)get_memory(stud_and_grade_hash_table_size * sizeof(grade_node_t *));
    g_curr = g_head;

    while (g_curr != NULL)
    {
        grade_hash_table[get_student_id_in_type_int(g_curr->grade->student_id)] = g_curr;
        g_curr = g_curr->next;
    }

    return;
}