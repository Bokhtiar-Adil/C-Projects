#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int32_t i32;

void *get_memory(u32 amount, u32 size, const char *caller_function_name);
bool is_prime(u32 number);

#endif /* __UTILS_H__ */