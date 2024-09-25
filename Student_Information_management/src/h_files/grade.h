#ifndef __GRADE_H__
#define __GRADE_H__

extern grade_node_t *g_head;
extern grade_node_t *g_tail;
extern grade_node_t **grade_hash_table;

grade_t *create_grade(string_t id, int32_t english, int32_t math, int32_t history);
grade_node_t *create_grade_node(grade_t *grade);
void insert_grade_at_the_end(grade_node_t *grade);
void write_grades_in_file();
bool_t insert_new_grade_in_the_linked_list(string_t id, int32_t english, int32_t math, int32_t history);
bool_t check_is_grade_already_exist(string_t stud_id);
bool_t delete_grade_from_the_list(string_t stud_id);

#endif /* __GRADE_H__ */