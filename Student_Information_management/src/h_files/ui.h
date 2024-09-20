#ifndef __UI_H__
#define __UI_H__

#include "common.h"

extern bool_t is_exit;
extern const char *app_name;
extern const char *menu_message;
extern const char *add_success_msg;

void load_all_data();
void ui_main_menu();
void ui_department_management();
void ui_student_management();
void ui_grade_management();
void save_process();
void exit_process();
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

#endif /* __UI_H__ */