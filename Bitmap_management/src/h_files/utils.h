#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdbool.h>

#define MAX_MENU_WIDTH 50
#define APP_NAME "BITMAP MANAGEMENT"
#define USER_INPUT_OPTION_ONE 1
#define USER_INPUT_OPTION_TWO 2
#define USER_INPUT_OPTION_THREE 3
#define USER_INPUT_OPTION_FOUR 4
#define USER_INPUT_OPTION_FIVE 5
#define USER_INPUT_OPTION_SIX 6
#define USER_INPUT_OPTION_SEVEN 7
#define USER_INPUT_OPTION_EIGHT 8
#define USER_INPUT_OPTION_NINE 9
#define USER_INPUT_OPTION_TEN 10
#define USER_INPUT_OPTION_ELEVEN 11
#define MAX_PARSE_STRING_LENGTH 100
#define MIN_INDEX_OF_PARSE_STRING 1
#define MAX_INDEX_OF_PARSE_STRING 65535
#define MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS 32
#define MAX_NUMBER_OF_BITS_IN_INDEX_OF_BITFIELD 16
#define INVALID_VALUE_INDICATOR -1
#define INVALID_INPUT_MESSAGE "ERROR: invalid input"
#define OPERATION_SUCCESSFUL_MESSAGE "Operation successful"
#define NULL_CALLOC_RETURNED_MESSAGE "ERROR: Null pointer returned by calloc"
#define NULL_POINTER_TO_BITMAP_MESSAGE "ERROR: Null pointer to bitmap encountered"
#define NOT_YET_CREATED_MESSAGE "INFO: This bitmap hasn't been created yet"
#define FIND_MAX(x,y) (((x) >= (y)) ? (x) : (y))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t i32;

extern const char *utils_check_message;

i32 get_integer_from_user_input();

#endif /* __UTILS_H__ */