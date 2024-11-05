#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../h_files/ui.h"
#include "../h_files/common.h"

int main(void)
{
    open_debug_file("w");
    close_debug_file();

    int32_t selected_menu_item = 0;
    bool_t is_displaying_table = false;
    fptr_ui_loader ui_loader = ui_main_menu;
    fptr_table_displayer table_displayer = NULL;

    menu_message = "";
    load_all_data();

    while (!is_exit)
    {
        if (is_displaying_table)
            display_table(table_displayer);
        else
            ui_loader();

        printf(">> ");
        selected_menu_item = get_integer_from_user_input();

        if (selected_menu_item == -1)
        {
            menu_message = INVALID_INPUT_MSG;
            continue;
        }

        if (is_displaying_table)
        {
            switch (selected_menu_item)
            {
                case USER_INPUT_OPTION_ONE:
                    is_displaying_table = false;
                    menu_message = "";
                    break;

                default:
                    menu_message = INVALID_INPUT_MSG;
                    break;
            }
        }

        else if (ui_loader == ui_main_menu)
        {
            switch (selected_menu_item)
            {
                case USER_INPUT_OPTION_ONE:
                    menu_message = "";
                    ui_loader = ui_department_management;
                    break;
                case USER_INPUT_OPTION_TWO:
                    menu_message = "";
                    ui_loader = ui_student_management;
                    break;
                case USER_INPUT_OPTION_THREE:
                    menu_message = "";
                    ui_loader = ui_grade_management;
                    break;
                case USER_INPUT_OPTION_FOUR:
                    menu_message = "";
                    ui_loader();
                    save_process();
                    break;
                case USER_INPUT_OPTION_FIVE:
                    menu_message = "";
                    ui_loader();
                    exit_process();
                    break;
                default:
                    menu_message = INVALID_INPUT_MSG;
                    break;
            }
        }
        else if (ui_loader == ui_department_management)
        {
            switch (selected_menu_item)
            {
                case USER_INPUT_OPTION_ONE:
                    menu_message = "";
                    ui_loader();
                    add_new_department_process();
                    break;
                case USER_INPUT_OPTION_TWO:
                    menu_message = "";
                    ui_loader();
                    delete_department_process();
                    break;
                case USER_INPUT_OPTION_THREE:
                    menu_message = "";
                    ui_loader();
                    update_department_process();
                    break;
                case USER_INPUT_OPTION_FOUR:
                    menu_message = "";
                    is_displaying_table = true;
                    table_displayer = display_departments;
                    break;
                case USER_INPUT_OPTION_FIVE:
                    menu_message = "";
                    ui_loader = ui_main_menu;
                    break;
                case USER_INPUT_OPTION_SIX:
                    menu_message = "";
                    ui_loader();
                    exit_process();
                    break;
                default:
                    menu_message = INVALID_INPUT_MSG;
                    break;
            }
        }
        else if (ui_loader == ui_student_management)
        {
            switch (selected_menu_item)
            {
                case USER_INPUT_OPTION_ONE:
                    menu_message = "";
                    ui_loader();
                    add_new_student_process();
                    break;
                case USER_INPUT_OPTION_TWO:
                    menu_message = "";
                    ui_loader();
                    delete_student_process();
                    break;
                case USER_INPUT_OPTION_THREE:
                    menu_message = "";
                    ui_loader();
                    update_student_process(1);
                    break;
                case USER_INPUT_OPTION_FOUR:
                    menu_message = "";
                    ui_loader();
                    update_student_process(2);
                    break;
                case USER_INPUT_OPTION_FIVE:
                    menu_message = "";
                    ui_loader();
                    update_student_process(3);
                    break;
                case USER_INPUT_OPTION_SIX:
                    menu_message = "";
                    is_displaying_table = true;
                    table_displayer = display_students;
                    break;
                case USER_INPUT_OPTION_SEVEN:
                    menu_message = "";
                    ui_loader = ui_main_menu;
                    break;
                case USER_INPUT_OPTION_EIGHT:
                    menu_message = "";
                    ui_loader();
                    exit_process();
                    break;
                default:
                    menu_message = INVALID_INPUT_MSG;
                    break;
            }
        }
        else if (ui_loader == ui_grade_management)
        {
            switch (selected_menu_item)
            {
                case USER_INPUT_OPTION_ONE:
                    menu_message = "";
                    ui_loader();
                    add_new_grade_process();
                    break;
                case USER_INPUT_OPTION_TWO:
                    menu_message = "";
                    ui_loader();
                    delete_grade_process();
                    break;
                case USER_INPUT_OPTION_THREE:
                    menu_message = "";
                    ui_loader();
                    update_grade_process(1);
                    break;
                case USER_INPUT_OPTION_FOUR:
                    menu_message = "";
                    ui_loader();
                    update_grade_process(2);
                    break;
                case USER_INPUT_OPTION_FIVE:
                    menu_message = "";
                    ui_loader();
                    update_grade_process(3);
                    break;
                case USER_INPUT_OPTION_SIX:
                    menu_message = "";
                    is_displaying_table = true;
                    table_displayer = display_grades;
                    break;
                case USER_INPUT_OPTION_SEVEN:
                    menu_message = "";
                    ui_loader = ui_main_menu;
                    break;
                case USER_INPUT_OPTION_EIGHT:
                    menu_message = "";
                    ui_loader();
                    exit_process();
                    break;
                default:
                    menu_message = INVALID_INPUT_MSG;
                    break;
            }
        }
    }

    return 0;
}