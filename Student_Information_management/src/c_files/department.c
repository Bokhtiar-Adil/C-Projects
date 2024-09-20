#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"
#include "../h_files/department.h"

department_t *create_department(int32_t id, string_t name, bool_t is_from_file);
department_node_t *create_department_node(department_t *department);
void insert_department_at_the_end(department_node_t *department);
void write_departments_in_file();
bool_t insert_new_department_in_the_linked_list(int32_t id, string_t name);
bool_t check_department_id_value_validity(int32_t id);
bool_t check_department_id_is_unique(int32_t id);
bool_t check_department_name_validity(string_t name);
bool_t check_department_name_is_unique(string_t name);
bool_t add_new_department_handler(int32_t id, string_t name);
bool_t update_department_name(int32_t id, string_t new_name);

department_node_t *d_head = NULL;
department_node_t *d_tail = NULL;
/* store the maximum department id to use for auto-increment */
int32_t maximum_dept_id;
/*
 *   This variable will be frequently used to iterate loops in various functions
 */
static int32_t iter = 0;

department_t *create_department(int32_t id, string_t name, bool_t is_from_file)
{
    department_t *new_dept = NULL;

    new_dept = (department_t *)get_memory((int32_t)sizeof(department_t));

    if (new_dept == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    /*
     *   If it is loading from file, the id in file will be used
     *   If a new department is being added by user, automatic increase will be used
     */
    if (is_from_file)
        new_dept->id = id;
    else
        new_dept->id = maximum_dept_id + 1;

    new_dept->name = (string_t)get_memory(MAX_DEPARTMENT_NAME_LENGTH);

    if (new_dept->name == NULL)
    {
        is_null_alloc_returned = true;
        free(new_dept);
        new_dept = NULL;

        return NULL;
    }

    memcpy(new_dept->name, name, strlen(name) + 1);
    is_null_alloc_returned = false;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new_dept;
}

department_node_t *create_department_node(department_t *department)
{
    department_node_t *new = NULL;

    new = (department_node_t *)get_memory((int32_t)sizeof(department_node_t));

    if (new == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    new->department = department;
    new->prev = NULL;
    new->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new;
}

void insert_department_at_the_end(department_node_t *department)
{
    if (department == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, null_node_msg);

        return;
    }

    if (d_head == NULL)
    {
        d_head = department;
        d_tail = department;
        department->prev = NULL;
    }
    else
    {
        d_tail->next = department;
        department->prev = d_tail;
        d_tail = department;
    }
    department->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

void write_departments_in_file()
{
    department_node_t *curr = NULL;
    FILE *fp = NULL;

    curr = d_head;
    fp = fopen(departments_file_name, "w");

    if (fp == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_open_msg);

        return;
    }

    while (curr != NULL)
    {
        fprintf(fp, "%d,%s\n", curr->department->id, curr->department->name);
        curr = curr->next;
    }

    if (fclose(fp) != 0)
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_close_msg);

    fp = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

bool_t check_department_id_value_validity(int32_t id)
{
    if (id < MIN_DEPARTMENT_ID_VALUE || id > MAX_DEPARTMENT_ID_VALUE)
    {
        check_message = d_invalid_id_msg;

        return false;
    }

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

/*
 *  returns true if it is unique, if not, returns false
 */

bool_t check_department_id_is_unique(int32_t id)
{
    department_node_t *d_curr = NULL;

    d_curr = d_head;

    while (d_curr != NULL)
    {
        if (d_curr->department->id == id)
        {
            check_message = d_id_exists_msg;
            return false;
        }

        d_curr = d_curr->next;
    }

    check_message = d_id_not_exist_msg;
    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

/*
 *  A valid department name will only contain a-z, A-Z, underscores and spaces
 */

bool_t check_department_name_validity(string_t name)
{
    int32_t len = 0;
    char tmp = 0;

    len = strlen(name);

    if (len >= MAX_DEPARTMENT_NAME_LENGTH)
    {
        check_message = d_invalid_name_length_msg;
        return false;
    }

    for (iter = 0; iter < len; iter++)
    {
        tmp = name[iter];
        if (!((tmp >= 'a' && tmp <= 'z') || (tmp >= 'A' && tmp <= 'Z') || (tmp == '_') || (tmp == ' ')))
        {
            check_message = d_invalid_name_syntax_msg;
            return false;
        }
    }

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

/*
 *  This fucntion returns 'true' if the new dept_name does not exist in the table, 'false' if it does
 */

bool_t check_department_name_is_unique(string_t name)
{
    department_node_t *curr = NULL;

    curr = d_head;

    while (curr != NULL)
    {
        if (!strncmp(curr->department->name, name, MAX_DEPARTMENT_NAME_LENGTH))
        {
            check_message = d_id_exists_msg;
            return false;
        }

        curr = curr->next;
    }

    check_message = d_id_not_exist_msg;
    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t insert_new_department_in_the_linked_list(int32_t id, string_t name)
{
    department_t *dept = NULL;
    department_node_t *dept_node = NULL;

    dept = create_department(id, name, false);

    if (dept == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        return false;
    }

    dept_node = create_department_node(dept);

    if (dept_node == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        free(dept);
        dept = NULL;

        return false;
    }

    insert_department_at_the_end(dept_node);
    maximum_dept_id = dept_node->department->id;
    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t add_new_department_handler(int32_t id, string_t name)
{
    /*
     *  The validity of id is already checked in the caller function
     *  So, this function only checks the given name is valid or not
     */

    if (!check_department_name_validity(name))
        return false;
    else if (!check_department_name_is_unique(name))
        return false;
    else
        return insert_new_department_in_the_linked_list(id, name);

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t update_department_name(int32_t id, string_t new_name)
{
    department_node_t *curr = NULL;

    curr = d_head;

    if (!check_department_name_validity(new_name) || !check_department_name_is_unique(new_name))
        return false;

    while (curr != NULL)
    {
        if (curr->department->id == id)
            break;

        curr = curr->next;
    }

    memset(curr->department->name, 0, MAX_DEPARTMENT_NAME_LENGTH);
    memcpy(curr->department->name, new_name, strlen(new_name) + 1);

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}