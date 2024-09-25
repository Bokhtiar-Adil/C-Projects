#ifndef __DEPARTMENT_H__
#define __DEPARTMENT_H__

extern department_node_t *d_head;
extern department_node_t *d_tail;
extern int32_t maximum_dept_id;
extern department_node_t **dept_hash_table;
extern uint32_t dept_hash_table_size;

department_t *create_department(int32_t id, string_t name, bool_t is_from_file);
department_node_t *create_department_node(department_t *department);
void insert_department_at_the_end(department_node_t *department);
void write_departments_in_file();
bool_t insert_new_department_in_the_linked_list(int32_t id, string_t name);
bool_t check_department_id_value_validity(int32_t id);
bool_t check_department_id_is_unique(int32_t id);
bool_t add_new_department_handler(int32_t id, string_t name);
bool_t update_department_name(int32_t id, string_t new_name);

#endif /* __DEPARTMENT_H__ */