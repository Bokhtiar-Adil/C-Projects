#ifndef __INTER_RELATIONS_H__
#define __INTER_RELATIONS_H__

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

#endif /* __INTER_RELATIONS_H__ */