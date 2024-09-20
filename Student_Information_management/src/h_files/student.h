#ifndef __STUDENT_H__
#define __STUDENT_H__

extern student_node_t *s_head;
extern student_node_t *s_tail;
extern int32_t total_students;

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

#endif /* __STUDENT_H__ */