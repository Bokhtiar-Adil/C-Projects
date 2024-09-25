#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../h_files/utils.h"
#include "../h_files/bit_management.h"

struct bitmap *bitMap_create(u16 capacity);
void bitmap_destroy(struct bitmap *bm);
static bool bitmap_check(struct bitmap *bm);
bool bitmap_add_value(struct bitmap *bm, u16 value);
bool bitmap_del_value(struct bitmap *bm, u16 value);
void bitmap_print(struct bitmap *bm);
struct bitmap *bitmap_clone(struct bitmap *bm);
bool bitmap_not(struct bitmap *bm);
bool bitmap_or(struct bitmap *bm_store, struct bitmap *bm);
bool bitmap_and(struct bitmap *bm_store, struct bitmap *bm);
struct bitmap *bitmap_parse_str(u8 *str);
void bitmap_update_first_and_last_value_and_numbers(struct bitmap *bm);
void bitmap_print_details(struct bitmap *bm);

static i32 bit_management_iter = 0;

struct bitmap *bitMap_create(u16 capacity)
{
    struct bitmap *new = NULL;
    u32 extra_bits = 0;

    extra_bits = (capacity % MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS == 0) ? capacity : ((capacity / MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS) + 1) * MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS;

    new = (struct bitmap *)calloc(1, sizeof(struct bitmap) + extra_bits);

    if (new == NULL)
    {
        utils_check_message = NULL_CALLOC_RETURNED_MESSAGE;

        return NULL;
    }

    new->max_value = capacity;
    new->first_value = 0;
    new->last_value = 0;
    new->numbers = 0;
    new->buf_len = extra_bits / MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS;
    new->bm_self = new;

    return new;
}

void bitmap_destroy(struct bitmap *bm)
{
    if (bm == NULL)
        return;

    bm->bm_self = NULL;
    free(bm);
    bm = NULL;

    return;
}

bool bitmap_check(struct bitmap *bm)
{
    if (bm == NULL)
    {
        utils_check_message = NULL_POINTER_TO_BITMAP_MESSAGE;

        return false;
    }

    return true;
}

void bitmap_print(struct bitmap *bm)
{
    u32 mask = 0;
    u32 curr = 0;
    u32 index = 0;
    i32 from_index = 0;
    i32 to_index = 0;
    u32 one_found_so_far = 0;
    i32 iter2 = 0;
    u32 tmp = 0;

    if (bitmap_check(bm) == false)
    {
        utils_check_message = NULL_POINTER_TO_BITMAP_MESSAGE;
        printf("%s : ERROR : %s\n", __FUNCTION__, utils_check_message);

        return;
    }

    printf("buf : ");

    from_index = INVALID_VALUE_INDICATOR;
    to_index = INVALID_VALUE_INDICATOR;
    index = 1;
    one_found_so_far = 0;

    /*
     *  in the bitmap 1st bit is the rightmost bit so buf contains every bit from right to left
     *  so, the iteration starts from right
     *  example --> if bit field is "1-33", buf[0] = 0x1, buf[1] = 0xffffffff
     */

    for (bit_management_iter = bm->buf_len - 1; bit_management_iter >= 0; bit_management_iter--)
    {
        curr = bm->buf[bit_management_iter];
        mask = 1;

        while (mask != 0)
        {
            tmp = curr & mask;

            if (tmp != 0)
            {
                if (from_index == INVALID_VALUE_INDICATOR)
                    from_index = index;

                to_index = index;
                one_found_so_far++;
            }
            else
            {
                if (from_index != INVALID_VALUE_INDICATOR)
                {
                    if (from_index == to_index)
                        printf("%d", from_index);
                    else
                        printf("%d-%d", from_index, to_index);

                    if (one_found_so_far != bm->numbers)
                        printf(",");
                }

                from_index = INVALID_VALUE_INDICATOR;
                to_index = INVALID_VALUE_INDICATOR;
            }

            mask <<= 1;
            index++;
        }
    }

    printf("\n");

    return;
}

void bitmap_print_details(struct bitmap *bm)
{
    u32 mask = 0;
    u32 curr = 0;
    u32 index = 0;
    i32 from_index = 0;
    i32 to_index = 0;
    u32 one_found_so_far = 0;
    i32 iter2 = 0;
    u32 tmp = 0;

    if (bitmap_check(bm) == false)
    {
        printf("%s : ERROR : %s\n", __FUNCTION__, utils_check_message);

        return;
    }

    printf("max_value   : %hu\n", bm->max_value);
    printf("first_value : %hu\n", bm->first_value);
    printf("last_value  : %hu\n", bm->last_value);
    printf("numbers     : %hu\n", bm->numbers);
    printf("buf_len     : %hu\n", bm->buf_len);
    printf("bitmap      : ");

    for (bit_management_iter = 0; bit_management_iter < bm->buf_len; bit_management_iter++)
    {
        printf("%032b", bm->buf[bit_management_iter]);

        if (bit_management_iter < bm->buf_len - 1)
            printf(", ");
    }

    printf("\n");
    printf("buf         : ");

    from_index = INVALID_VALUE_INDICATOR;
    to_index = INVALID_VALUE_INDICATOR;
    index = 1;
    one_found_so_far = 0;

    /*
     *  in the bitmap 1st bit is the rightmost bit so buf contains every bit from right to left
     *  so, the iteration starts from right
     *  example --> if bit field is "1-33", buf[0] = 0x1, buf[1] = 0xffffffff
     */

    for (bit_management_iter = bm->buf_len - 1; bit_management_iter >= 0; bit_management_iter--)
    {
        curr = bm->buf[bit_management_iter];
        mask = 1;

        while (mask != 0)
        {
            tmp = curr & mask;

            if (tmp != 0)
            {
                if (from_index == INVALID_VALUE_INDICATOR)
                    from_index = index;

                to_index = index;
                one_found_so_far++;
            }
            else
            {
                if (from_index != INVALID_VALUE_INDICATOR)
                {
                    if (from_index == to_index)
                        printf("%d", from_index);
                    else
                        printf("%d-%d", from_index, to_index);

                    if (one_found_so_far != bm->numbers)
                        printf(",");
                }

                from_index = INVALID_VALUE_INDICATOR;
                to_index = INVALID_VALUE_INDICATOR;
            }

            mask <<= 1;
            index++;
        }
    }

    printf("\n");

    return;
}

bool bitmap_add_value(struct bitmap *bm, u16 value)
{
    u16 buf_index = 0;
    u16 bit_index = 0;
    u32 mask = 0;

    if (bitmap_check(bm) == false)
        return false;

    if (value > bm->max_value)
    {
        printf("%s : ERROR : argument 'value' should be less than %hu\n", __FUNCTION__, bm->max_value);

        return false;
    }

    if (value == 0)
    {
        printf("%s : ERROR : argument 'value' can't be zero\n", __FUNCTION__);

        return false;
    }

    /*
     *  using value - 1 because if value = MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS, it creates unwanted problem
     */

    buf_index = bm->buf_len - 1 - ((value - 1) / MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS);
    bit_index = (value - 1) % MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS;
    mask = 1 << bit_index;
    bm->buf[buf_index] |= mask;
    bm->numbers++;

    if (bm->first_value == 0 || value < bm->first_value)
        bm->first_value = value;

    if (bm->last_value == 0 || value > bm->last_value)
        bm->last_value = value;

    return true;
}

bool bitmap_del_value(struct bitmap *bm, u16 value)
{
    u16 buf_index = 0;
    u16 bit_index = 0;
    u32 mask = 0;
    bool is_first_one_found = false;
    u32 index = 0;

    if (bitmap_check(bm) == false)
    {
        printf("%s : ERROR : %s\n", __FUNCTION__, utils_check_message);

        return false;
    }

    if (value > bm->max_value)
    {
        printf("%s : ERROR : argument 'value' should be less than %hu\n", __FUNCTION__, bm->max_value);

        return false;
    }

    if (value == 0)
    {
        printf("%s : ERROR : argument 'value' can't be zero\n", __FUNCTION__);

        return false;
    }

    buf_index = bm->buf_len - 1 - ((value - 1) / MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS);
    bit_index = (value - 1) % MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS;
    mask = 1 << bit_index;
    bm->buf[buf_index] &= ~mask;

    /*
     *  there were no 1 before first_value and after last_value in the bitmap
     *  so, deleting 1 means doing operation within this boundary [first_value, last_value]
     *  if the index of the deleted 1 is not equal to first_value or last_value, their values remain unchanged
     *  if the value equals to either first_value or last_value, their values need to be changed
     */

    if (value == bm->first_value || value == bm->last_value)
        bitmap_update_first_and_last_value_and_numbers(bm);

    bit_management_iter = 0;

    return true;
}

struct bitmap *bitmap_clone(struct bitmap *bm)
{
    struct bitmap *new = NULL;

    if (bitmap_check(bm) == false)
        return NULL;

    new = bitMap_create(bm->max_value);

    if (new == NULL)
        return NULL;

    new->max_value = bm->max_value;
    new->first_value = bm->first_value;
    new->last_value = bm->last_value;
    new->numbers = bm->numbers;
    new->buf_len = bm->buf_len;

    bit_management_iter = 0;

    while (bit_management_iter < bm->buf_len)
    {
        new->buf[bit_management_iter] = bm->buf[bit_management_iter];
        bit_management_iter++;
    }

    bit_management_iter = 0;

    return new;
}

bool bitmap_not(struct bitmap *bm)
{
    if (bitmap_check(bm) == false)
    {
        printf("%s : ERROR : %s\n", __FUNCTION__, utils_check_message);

        return false;
    }

    for (bit_management_iter = 0; bit_management_iter < bm->buf_len; bit_management_iter++)
        bm->buf[bit_management_iter] = ~bm->buf[bit_management_iter];

    bitmap_update_first_and_last_value_and_numbers(bm);

    return true;
}

bool bitmap_or(struct bitmap *bm_store, struct bitmap *bm)
{
    i32 bm_store_buf = 0;
    i32 bm_buf = 0;

    if (bitmap_check(bm_store) == false)
    {
        printf("%s : ERROR : %s : bm_store\n", __FUNCTION__, utils_check_message);

        return false;
    }

    if (bitmap_check(bm) == false)
    {
        printf("%s : ERROR : %s : bm\n", __FUNCTION__, utils_check_message);

        return false;
    }

    bm_store_buf = bm_store->buf_len - 1;
    bm_buf = bm->buf_len - 1;

    /*
     *  if the bm_store_buf is larger than bm_buff, that means bm_store has larger bitmap than bm
     *  for example - bm_store => 11100 and bm => 11
     *  so, the or will be -> 11100 | 00011 => 11111
     */

    while (bm_store_buf >= 0 && bm_buf >= 0)
    {
        bm_store->buf[bm_store_buf] = bm_store->buf[bm_store_buf] | bm->buf[bm_buf];
        bm_store_buf--;
        bm_buf--;
    }

    bitmap_update_first_and_last_value_and_numbers(bm_store);

    return true;
}

bool bitmap_and(struct bitmap *bm_store, struct bitmap *bm)
{
    i32 bm_store_buf = 0;
    i32 bm_buf = 0;

    if (bitmap_check(bm_store) == false)
        return false;

    if (bitmap_check(bm) == false)
        return false;

    bm_store_buf = bm_store->buf_len - 1;
    bm_buf = bm->buf_len - 1;

    /*
     *  if the bm_store_buf is larger than bm_buff, that means bm_store has larger bitmap than bm
     *  for example - bm_store => 11111 and bm => 11
     *  so, the and will be -> 11111 & 00011 => 00011
     */

    while (bm_store_buf >= 0 && bm_buf >= 0)
    {
        bm_store->buf[bm_store_buf] = bm_store->buf[bm_store_buf] & bm->buf[bm_buf];
        bm_store_buf--;
        bm_buf--;
    }

    while (bm_store_buf >= 0)
    {
        bm_store->buf[bm_store_buf] = 0;
        bm_store_buf--;
    }

    bitmap_update_first_and_last_value_and_numbers(bm_store);

    return true;
}

struct bitmap *bitmap_parse_str(u8 *str)
{
    i32 len = 0;
    i32 index = 0;
    i32 from_index = 0;
    i32 to_index = 0;
    i32 value = 0;
    i32 from_value = 0;
    i32 to_value = 0;
    i32 factor = 0;
    char *tmp_bitmap = NULL;
    struct bitmap *new = NULL;

    len = strlen(str);
    from_value = INVALID_VALUE_INDICATOR;
    to_value = INVALID_VALUE_INDICATOR;
    bit_management_iter = len - 1;
    factor = 1;

    while (bit_management_iter >= 0)
    {
        if (str[bit_management_iter] == '-' || str[bit_management_iter] == ',')
            break;

        value = (str[bit_management_iter] - '0') * factor + value;
        bit_management_iter--;
        factor *= 10;
    }

    new = bitMap_create((u16)value);

    if (new == NULL)
        return NULL;

    from_index = INVALID_VALUE_INDICATOR;
    to_index = INVALID_VALUE_INDICATOR;
    index = 1;
    bit_management_iter = 0;

    while (bit_management_iter < len)
    {
        value = 0;

        while (str[bit_management_iter] >= '0' && str[bit_management_iter] <= '9')
        {
            value = value * 10 + (str[bit_management_iter] - '0');
            bit_management_iter++;
        }

        if (from_index == INVALID_VALUE_INDICATOR)
            from_index = value;

        to_index = value;

        if (str[bit_management_iter] != '-')
        {
            for (index = from_index; index <= to_index; index++)
            {
                if (!bitmap_add_value(new, (u16)index))
                    return NULL;
            }

            from_index = INVALID_VALUE_INDICATOR;
            to_index = INVALID_VALUE_INDICATOR;
        }

        bit_management_iter++;
    }

    return new;
}

void bitmap_update_first_and_last_value_and_numbers(struct bitmap *bm)
{
    bool is_first_one_found = false;
    u32 current = 0;
    u32 index = 0;

    bm->numbers = 0;

    /*
     *  in the bitmap 1st bit is the rightmost bit so buf contains every bit from right to left
     *  example --> if bit field is "1-33", buf[0] = 0x1, buf[1] = 0xffffffff
     */

    for (bit_management_iter = bm->buf_len - 1; bit_management_iter >= 0; bit_management_iter--)
    {
        current = bm->buf[bit_management_iter];
        index = 1;

        while (index <= MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS)
        {
            if (current & 1 == 1)
            {
                /*
                 *  first_value is only updated when the first one is found
                 */

                if (is_first_one_found == false)
                    bm->first_value = MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS * (bm->buf_len - 1 - bit_management_iter) + index;

                bm->last_value = MAX_NUMBER_OF_BITS_IN_BUF_ELEMENTS * (bm->buf_len - 1 - bit_management_iter) + index;
                is_first_one_found = true;
                bm->numbers++;
            }

            index++;
            current >>= 1;
        }
    }

    return;
}