#ifndef __UI_H__
#define __UI_H__

typedef void (*fptr_print_function)(struct bitmap *);

extern const char *ui_menu_message;
extern fptr_print_function bitmap_printer;

void main_menu();
void create_bitmap();
void print_bitmap();
void add_bit_in_bitmap();
void delete_bit_in_bitmap();
void and_bitmaps();
void or_bitmaps();
void not_bitmap();
void clone_bitmap();
void parse_string();
void destroy_bitmap();
void exit_process(bool *is_exit);

#endif /* __UI_H__ */