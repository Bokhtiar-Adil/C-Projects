#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../h_files/utils.h"

void *get_memory(u32 amount, u32 size, const char *caller_function_name)
{
    void *pointer = NULL;

    pointer = calloc(amount, size);

    if (pointer == NULL)
        printf("ERROR: NULL calloc returned in %s\n", caller_function_name);

    return pointer;
}

bool is_prime(u32 number)
{
    /* checks whether number is prime or not */

    i32 iter = 0;

    if (number < 2)
        return false;

    if (number == 2)
        return true;

    for (iter = 2; iter < (i32)sqrt((double)number); iter++)
    {
        if (number % iter == 0)
            return false;
    }

    return true;
}