#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/common.h"
#include "../h_files/grade.h"

grade_t *create_grade(string_t id, int32_t english, int32_t math, int32_t history);
grade_node_t *create_grade_node(grade_t *grade);
void insert_grade_at_the_end(grade_node_t *grade);
void write_grades_in_file();
bool_t insert_new_grade_in_the_linked_list(string_t id, int32_t english, int32_t math, int32_t history);
bool_t check_is_grade_already_exist(string_t stud_id);
bool_t delete_grade_from_the_list(string_t stud_id);

grade_node_t *g_head, *g_tail;
int32_t total_grades;
/*
 *   This variable will be frequently used to iterate loops in various functions
 */
static int32_t iter = 0;

grade_t *create_grade(string_t id, int32_t english, int32_t math, int32_t history)
{
    grade_t *new_grade = NULL;

    new_grade = (grade_t *)get_memory((int32_t)sizeof(grade_t));

    if (new_grade == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    new_grade->student_id = (string_t)get_memory(MAX_STUDENT_ID_LENGTH);

    if (new_grade->student_id == NULL)
    {
        is_null_alloc_returned = true;
        free(new_grade);
        new_grade = NULL;

        return NULL;
    }

    memcpy(new_grade->student_id, id, strlen(id) + 1);
    new_grade->english = english;
    new_grade->math = math;
    new_grade->history = history;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new_grade;
}

grade_node_t *create_grade_node(grade_t *grade)
{
    grade_node_t *new = NULL;

    new = (grade_node_t *)get_memory((int32_t)sizeof(grade_node_t));

    if (new == NULL)
    {
        is_null_alloc_returned = true;

        return NULL;
    }

    new->grade = grade;
    new->prev = NULL;
    new->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return new;
}

void insert_grade_at_the_end(grade_node_t *grade)
{
    if (grade == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, null_node_msg);

        return;
    }

    if (g_head == NULL)
    {
        g_head = grade;
        g_tail = grade;
        grade->prev = NULL;
    }
    else
    {
        g_tail->next = grade;
        grade->prev = g_tail;
        g_tail = grade;
    }

    grade->next = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

void write_grades_in_file()
{
    grade_node_t *curr = g_head;
    FILE *fp = fopen(grades_file_name, "w");

    if (fp == NULL)
    {
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_open_msg);

        return;
    }

    while (curr != NULL)
    {
        fprintf(fp, "%s,%d,%d,%d\n", curr->grade->student_id, curr->grade->english, curr->grade->math, curr->grade->history);
        curr = curr->next;
    }

    if (fclose(fp) != 0)
        print_debug_message(__FILE__, __FUNCTION__, __LINE__, file_not_close_msg);

    fp = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return;
}

bool_t insert_new_grade_in_the_linked_list(string_t id, int32_t english, int32_t math, int32_t history)
{
    grade_t *grade = NULL;
    grade_node_t *grade_node = NULL;
    grade_node_t *g_curr = NULL;
    int32_t int_id = 0;

    grade = create_grade(id, english, math, history);

    if (grade == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        return false;
    }

    grade_node = create_grade_node(grade);

    if (grade_node == NULL)
    {
        if (is_null_alloc_returned == true)
            check_message = null_calloc_msg;

        free(grade);
        grade = NULL;

        return false;
    }

    g_curr = g_head;
    int_id = get_student_id_in_type_int(id);
    total_grades++;

    if (g_head == NULL)
    {
        g_head = grade_node;
        g_tail = grade_node;

        return true;
    }

    /*
     *   Inserts grade in a sorted way
     */

    while (g_curr != NULL)
    {
        if (get_student_id_in_type_int(g_curr->grade->student_id) >= int_id)
            break;

        g_curr = g_curr->next;
    }

    if (g_curr == NULL)
    {
        g_tail->next = grade_node;
        grade_node->prev = g_tail;
        grade_node->next = NULL;
        g_tail = grade_node;

        return true;
    }

    grade_node->prev = g_curr->prev;
    g_curr->prev = grade_node;

    if (g_curr == g_head)
    {
        g_head = grade_node;
        grade_node->prev = NULL;
    }
    else
        grade_node->prev->next = grade_node;

    grade_node->next = g_curr;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}

bool_t check_is_grade_already_exist(string_t stud_id)
{
    grade_node_t *g_curr = NULL;

    g_curr = g_head;

    while (g_curr != NULL)
    {
        if (!strncmp((g_curr)->grade->student_id, stud_id, MAX_STUDENT_ID_LENGTH))
        {
            check_message = g_id_exists_msg;
            return true;
        }

        g_curr = g_curr->next;
    }

    check_message = g_id_not_exist_msg;

    return false;
}

bool_t delete_grade_from_the_list(string_t stud_id)
{
    grade_node_t *g_curr = NULL;

    g_curr = g_head;

    while (g_curr != NULL)
    {
        if (!strncmp(g_curr->grade->student_id, stud_id, MAX_STUDENT_ID_LENGTH))
            break;

        g_curr = g_curr->next;
    }

    if (g_curr == NULL)
    {
        check_message = g_id_not_exist_msg;

        return false;
    }
    else
    {
        if (g_curr->prev != NULL)
            g_curr->prev->next = g_curr->next;

        if (g_curr->next != NULL)
            g_curr->next->prev = g_curr->prev;

        if (g_curr == g_tail)
            g_tail = (g_curr->next != NULL) ? g_curr->next : g_curr->prev;

        if (g_curr == g_head)
            g_head = (g_curr->next != NULL) ? g_curr->next : NULL;
    }

    g_curr->next = NULL;
    g_curr->prev = NULL;
    free(g_curr);
    g_curr = NULL;

    print_debug_message(__FILE__, __FUNCTION__, __LINE__, "end");

    return true;
}