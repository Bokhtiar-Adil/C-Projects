#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include <stdbool.h>

#define MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS 32
#define MAX_NUMBER_OF_BITS_IN_INDEX_OF_BITFIELD 16
#define NULL_CALLOC_RETURNED_MESSAGE "ERROR: Null pointer returned by calloc"
#define NULL_POINTER_TO_BITMAP_MESSAGE "ERROR: Null pointer to bitmap encountered"
#define NOT_YET_CREATED_MESSAGE "INFO: This bitmap hasn't been created yet"

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int32_t i32;

extern const char *utils_check_message;

u32 power_of_two(u32 value);
void check_string(char *str);
i32 get_integer_from_user_input();

#endif /* __UTILS_H__ */