#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"
#include "../h_files/student.h"

student_t *create_student(string_t id, string_t name, string_t gender, int32_t department_id);
student_node_t *create_student_node(student_t *student);
void insert_student_at_the_end(student_node_t *student);
void write_students_in_file();
bool_t insert_new_student_in_the_linked_list(string_t id, string_t name, string_t gender, int32_t dept_id);
bool_t check_student_id_syntax_validity(string_t id);
bool_t check_student_id_is_unique(string_t id);
bool_t check_student_name_validity(string_t name);
bool_t update_student_name_by_id(string_t id, string_t new_name);
bool_t update_student_gender_by_id(string_t id, int32_t updated_value);

student_node_t *s_head = NULL;
student_node_t *s_tail = NULL;
int32_t total_students = 0;
static int32_t iter = 0;

student_t *create_student(string_t id, string_t name, string_t gender, int32_t department_id)
{
    student_t *new_stud = NULL;

    new_stud = (student_t *)get_memory((int32_t)sizeof(student_t));

    if (new_stud == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    new_stud->id = get_memory(MAX_STUDENT_ID_LENGTH);

    if (new_stud->id == NULL)
    {
        is_null_alloc_returned = true;
        free(new_stud);
        new_stud = NULL;

        return NULL;
    }

    memcpy(new_stud->id, id, strlen(id) + 1);
    new_stud->name = get_memory(MAX_STUDENT_NAME_LENGTH);

    if (new_stud->name == NULL)
    {
        is_null_alloc_returned = true;
        free(new_stud);
        new_stud = NULL;
        free(new_stud->id);
        new_stud->id = NULL;

        return NULL;
    }

    memcpy(new_stud->name, name, strlen(name) + 1);
    new_stud->gender = (!strncmp(gender, "MALE", strlen("MALE")) ? MALE : FEMALE);
    new_stud->department_id = department_id;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new_stud;
}

student_node_t *create_student_node(student_t *student)
{
    student_node_t *new = NULL;

    new = (student_node_t *)get_memory((int32_t)sizeof(student_node_t));

    if (new == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    new->student = student;
    new->prev = NULL;
    new->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new;
}

void insert_student_at_the_end(student_node_t *student)
{
    if (student == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, null_node_msg);

        return;
    }

    if (s_head == NULL)
    {
        s_head = student;
        s_tail = student;
        student->prev = NULL;
    }
    else
    {
        s_tail->next = student;
        student->prev = s_tail;
        s_tail = student;
    }
    student->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

void write_students_in_file()
{
    FILE *fp = NULL;
    student_node_t *curr = NULL;
    string_t int_to_str = NULL;

    fp = fopen(students_file_name, "w");
    curr = s_head;

    if (fp == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_open_msg);

        return;
    }

    while (curr != NULL)
    {
        fprintf(fp, "%s", curr->student->id);
        fprintf(fp, ",");
        fprintf(fp, "%s", curr->student->name);
        fprintf(fp, ",");
        fprintf(fp, (curr->student->gender == MALE ? "MALE" : "FEMALE"));
        fprintf(fp, ",");
        int_to_str = int_to_string(curr->student->department_id);
        fprintf(fp, "%s", int_to_str);
        free(int_to_str);
        fprintf(fp, "\n");
        curr = curr->next;
    }

    if (fclose(fp) != 0)
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_close_msg);

    fp = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

bool_t check_student_id_syntax_validity(string_t id)
{
    int32_t len = 0;

    len = strlen(id);

    if (len != MAX_STUDENT_ID_LENGTH - 1)
    {
        check_message = s_invalid_id_msg;
        return false;
    }

    /*
     * Comparing the given id with pattern 'BDCOM***'
     */

    for (iter = 0; iter < MAX_STUDENT_ID_LENGTH; iter++)
    {
        if (student_id_format[iter] != '*' && id[iter] != student_id_format[iter])
        {
            check_message = s_invalid_id_msg;
            return false;
        }
        else if (student_id_format[iter] == '*' && (id[iter] < '0' || id[iter] > '9'))
        {
            check_message = s_invalid_id_msg;
            return false;
        }
        else
            ;
    }

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

/*
 *  This function returns 'true' if the new student_id is unique, 'false' otherwise
 */

bool_t check_student_id_is_unique(string_t id)
{
    int32_t int_id = 0;
    int32_t other_id = 0;
    student_node_t *curr = NULL;

    int_id = get_student_id_in_type_int(id);
    curr = s_head;

    while (curr != NULL)
    {
        other_id = get_student_id_in_type_int(curr->student->id);

        if (int_id == other_id)
        {
            check_message = s_id_exists_msg;
            return false;
        }

        curr = curr->next;
    }

    check_message = s_id_not_exist_msg;
    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

/*
 *  A valid student name will only contain a-z, A-Z, underscores and spaces
 */

bool_t check_student_name_validity(string_t name)
{
    int32_t len = 0;

    len = strlen(name);

    if (len >= MAX_STUDENT_NAME_LENGTH)
    {
        check_message = s_invalid_name_length_msg;
        return false;
    }

    for (iter = 0; iter < len; iter++)
    {
        if (name[iter] == ' ')
            continue;
        else if (name[iter] == '_')
            continue;
        else if (name[iter] >= 'A' && name[iter] <= 'Z')
            continue;
        else if (name[iter] >= 'a' && name[iter] <= 'z')
            continue;
        else
        {
            check_message = s_invalid_name_syntax_msg;

            return false;
        }
    }

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t insert_new_student_in_the_linked_list(string_t id, string_t name, string_t gender, int32_t dept_id)
{
    student_t *stud = NULL;
    student_node_t *stud_node = NULL;
    student_node_t *s_curr = NULL;
    int32_t int_id = 0;

    stud = create_student(id, name, gender, dept_id);

    if (stud == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        return false;
    }

    stud_node = create_student_node(stud);

    if (stud_node == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        free(stud);
        stud = NULL;

        return false;
    }

    s_curr = s_head;
    int_id = get_student_id_in_type_int(id);
    total_students++;

    if (s_head == NULL)
    {
        s_head = stud_node;
        s_tail = stud_node;

        return true;
    }

    /*
     *   Inserts student in a sorted way
     */

    while (s_curr != NULL)
    {
        if (get_student_id_in_type_int(s_curr->student->id) >= int_id)
            break;

        s_curr = s_curr->next;
    }

    /*
     *   s_curr = NULL means the new node needs to be added at the last
     */

    if (s_curr == NULL)
    {
        s_tail->next = stud_node;
        stud_node->prev = s_tail;
        stud_node->next = NULL;
        s_tail = stud_node;

        return true;
    }

    stud_node->prev = s_curr->prev;
    s_curr->prev = stud_node;

    if (s_curr == s_head)
    {
        s_head = stud_node;
        stud_node->prev = NULL;
    }
    else
        stud_node->prev->next = stud_node;

    stud_node->next = s_curr;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t update_student_name_by_id(string_t id, string_t new_name)
{
    student_node_t *curr = NULL;

    curr = s_head;

    if (!check_student_name_validity(new_name))
        return false;

    while (curr != NULL)
    {
        if (!strncmp(curr->student->id, id, MAX_STUDENT_ID_LENGTH - 1))
            break;

        curr = curr->next;
    }

    memset(curr->student->name, 0, MAX_STUDENT_NAME_LENGTH);
    memcpy(curr->student->name, new_name, strlen(new_name) + 1);

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t update_student_gender_by_id(string_t id, int32_t updated_value)
{
    student_node_t *curr = NULL;

    curr = s_head;

    while (curr != NULL)
    {
        if (!strncmp(curr->student->id, id, MAX_STUDENT_ID_LENGTH - 1))
            break;

        curr = curr->next;
    }

    curr->student->gender = (updated_value == 1) ? MALE : FEMALE;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}