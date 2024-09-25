#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#elif __linux__
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include "../h_files/packet_analysis.h"

const char *exported_packets_file = "../txt_files/exported_packets.txt";
u32 hash_table_size = 0;
hash_node_t **hash_table = NULL;

bool create_hash_table(hash_node_t ***hash_table, u32 *hash_table_size, u32 size);
u32 hash_function(const u8 *ip_addresses, u16 src_port, u16 dest_port);
hash_node_t *create_hash_node(u32 key, u8 *src_and_dest_ip, u16 src_port, u16 dest_port);
bool insert_entry_in_hash_table(u32 key, u8 *src_and_dest_ip, u16 src_port, u16 dest_port, u32 *elements_in_hash_table);
bool construct_hash_table_from_byte_array(u8 *byte_array, u32 num_of_frame);
u32 load_frames_from_file_and_prepare_hash_table();
void print_hash_table();
void free_all_pointers();

bool create_hash_table(hash_node_t ***hash_table, u32 *hash_table_size, u32 size)
{
    hash_node_t **tmp = NULL;
    bool is_prime_found = false;
    u32 prime = 0;
    i32 iter = 0;

    /*
     *  lets say the argument 'size' is 100, nearest prime number after that is 101
     *  so, the hash table size will be 101
     */

    iter = size;

    while (iter < UINT_FAST32_MAX)
    {
        if (is_prime(iter))
        {
            is_prime_found = true;
            break;
        }

        iter++;
    }

    *hash_table_size = is_prime_found ? iter : size;
    tmp = (hash_node_t **)get_memory(1, *hash_table_size * sizeof(hash_node_t), __FUNCTION__);

    if (tmp == NULL)
        return false;

    *hash_table = tmp;
    tmp = NULL;

    return true;
}

u32 hash_function(const u8 *ip_addresses, u16 src_port, u16 dest_port)
{
    u32 prime1 = 0;
    u32 prime2 = 0;
    u32 prime3 = 0;
    u32 hash = 0;
    u32 src = 0;
    u32 dest = 0;
    u32 ports = 0;

    src = (ip_addresses[0] << 24) | (ip_addresses[1] << 16) | (ip_addresses[2] << 8) | ip_addresses[3];
    dest = (ip_addresses[4] << 24) | (ip_addresses[5] << 16) | (ip_addresses[6] << 8) | ip_addresses[7];
    ports = (src_port << 16) | dest_port;

    prime1 = 0x9e3779b1;
    prime2 = 0x85ebca6b;
    prime3 = 0xc2b2ae35;

    hash = src;
    hash ^= dest + prime1 + (hash << 6) + (hash >> 2);
    hash ^= ports + prime2 + (hash << 12) + (hash >> 4);
    hash *= prime3;

    return hash;
}

hash_node_t *create_hash_node(u32 key, u8 *src_and_dest_ip, u16 src_port, u16 dest_port)
{
    hash_node_t *new = NULL;

    new = (hash_node_t *)get_memory(1, sizeof(hash_node_t), __FUNCTION__);

    if (new == NULL)
        return NULL;

    new->key = key;
    memcpy(&(new->src_ip), src_and_dest_ip, IPV4_ADDRESS_BYTES_LENGTH);
    memcpy(&(new->dest_ip), &(src_and_dest_ip[4]), IPV4_ADDRESS_BYTES_LENGTH);
    new->src_port = src_port;
    new->dest_port = dest_port;
    new->counter = 1;
    new->next = NULL;

    return new;
}

void rehash(u32 current_elements_count)
{
    hash_node_t **new_hash_table = NULL;
    hash_node_t *curr = NULL, *old_next = NULL;
    bool is_success = false;
    i32 iter = 0;
    u32 new_hash_table_size = 0;
    u32 new_index = 0;

    /*
     *  let's say the current hash table has size 11 and there are 12 elements in the hash table
     *  so, this rehash function will increase the hash table size to decrease collison
     *  so, it will find out the next prime number after the value (12 * 2) i.e. 24
     *  current_elements_count * 2 is used to reduce the number of rehashing
     */

    is_success = create_hash_table(&new_hash_table, &new_hash_table_size, current_elements_count * 2);

    if (!is_success)
        return;

    /*
     *  recalculating the indices for each element in the hash table
     */

    for (iter = 0; iter < hash_table_size; iter++)
    {
        if (hash_table[iter] == NULL)
            continue;

        curr = hash_table[iter];

        while (curr != NULL)
        {
            new_index = curr->key % new_hash_table_size;
            old_next = curr->next;

            if (new_hash_table[new_index] == NULL)
            {
                new_hash_table[new_index] = curr;
                curr->next = NULL;
            }
            else
            {
                curr->next = new_hash_table[new_index];
                new_hash_table[new_index] = curr;
            }

            curr = old_next;
        }
    }

    free(hash_table);
    hash_table = NULL;
    hash_table = new_hash_table;
    hash_table_size = new_hash_table_size;

    return;
}

bool insert_entry_in_hash_table(u32 key, u8 *src_and_dest_ip, u16 src_port, u16 dest_port, u32 *elements_in_hash_table)
{
    u32 index = 0;
    hash_node_t *new = NULL;
    hash_node_t *curr = NULL;
    u32 iter = 0;
    bool is_same = false;

    index = key % hash_table_size;

    if (hash_table[index] == NULL)
    {
        /* means there is no entry in this index of hash table yet */

        new = create_hash_node(key, src_and_dest_ip, src_port, dest_port);

        if (new == NULL)
            return false;

        (*elements_in_hash_table)++;
        hash_table[index] = new;
    }
    else
    {
        /*
         *  if there is entry in this index, find out the one with same key and update counter
         *  if not found, create a new node in the hash table
         */

        curr = hash_table[index];

        while (curr != NULL)
        {
            if (curr->key == key)
            {
                /*
                 *  for the rare case of having two different entities with same key, this extra precaution is taken
                 *  even though it is rare, it is still possible to have two different entities with same hash key
                 *  this extra checking is done so that such abnormality doesn't hamper the hash table
                 */

                is_same = true;

                for (iter = 0; iter < IPV4_ADDRESS_BYTES_LENGTH; iter++)
                {
                    if (curr->src_ip[iter] != src_and_dest_ip[iter])
                    {
                        is_same = false;
                        break;
                    }
                }

                if (!is_same)
                    goto out;

                for (iter = 0; iter < IPV4_ADDRESS_BYTES_LENGTH; iter++)
                {
                    if (curr->dest_ip[iter] != src_and_dest_ip[IPV4_ADDRESS_BYTES_LENGTH + iter])
                    {
                        is_same = false;
                        break;
                    }
                }

                if (!is_same)
                    goto out;

                if (curr->src_port != src_port || curr->dest_port != dest_port)
                    is_same = false;

                if (is_same)
                    break;
            }

        out:
            curr = curr->next;
        }

        if (curr == NULL)
        {
            new = create_hash_node(key, src_and_dest_ip, src_port, dest_port);

            if (new == NULL)
                return false;

            (*elements_in_hash_table)++;
            new->next = hash_table[index];
            hash_table[index] = new;
        }
        else
            curr->counter++;
    }

    return true;
}

u8 *read_frames_from_wireshark_output_file_and_construct_byte_array(u32 *num_of_frame_holder)
{
    FILE *fp = NULL;
    char buffer[MAXIMUM_READ_BUFFER_SIZE] = {0};
    char *ptr = NULL;
    u8 byte = 0;
    u32 index = 0;
    u8 *tmp = NULL;
    u32 current_packet_buffer_length = 0;
    u8 *byte_array = NULL;
    u32 frame_count = 0;
    u32 packet_start_position_in_each_line_in_file = 6;
    u32 packet_end_position_in_each_line_in_file = 52;
    u8 maximum_bytes_in_one_line = 16;
    u8 bytes_found_in_this_line_so_far = 0;

    /*
     *  byte array will only have the individual bytes
     *  this function will also count the number of ethernet frame in the byte array
     *  and then save the value in the memory pointed by num_of_frame_holder
     */

    fp = fopen(exported_packets_file, "r");

    if (fp == NULL)
    {
        printf("file: %s, function: %s, line: %d: '%s' not opened\n", __FILE__, __FUNCTION__, __LINE__, exported_packets_file);

        return NULL;
    }

    current_packet_buffer_length = MINIMUM_BYTE_ARRAY_SIZE;
    byte_array = (u8 *)get_memory(1, MINIMUM_BYTE_ARRAY_SIZE, __FUNCTION__); /* initial allocation */

    if (byte_array == NULL)
    {
        fclose(fp);

        return NULL;
    }

    while (fgets(buffer, 100, fp))
    {
        if (strlen(buffer) < packet_start_position_in_each_line_in_file)
        {
            frame_count++;
            continue;
        }

        buffer[packet_end_position_in_each_line_in_file] = '\0';
        ptr = &buffer[packet_start_position_in_each_line_in_file];
        bytes_found_in_this_line_so_far = 0;

        while (bytes_found_in_this_line_so_far < maximum_bytes_in_one_line && sscanf(ptr, "%hhx", &byte) == 1)
        {
            bytes_found_in_this_line_so_far++;

            if (index >= current_packet_buffer_length)
            {
                current_packet_buffer_length += MINIMUM_BYTE_ARRAY_SIZE;

                if (current_packet_buffer_length > MAXIMUM_BYTE_ARRAY_SIZE)
                    goto out_of_loop;

                tmp = (u8 *)realloc(byte_array, current_packet_buffer_length);
                byte_array = NULL;
                byte_array = tmp;
                tmp = NULL;
            }

            byte_array[index] = byte;
            index++;

            while (*ptr != ' ' && *ptr != '\n' && *ptr != '\0')
                ptr++;

            while (*ptr == ' ' || *ptr == '\n')
                ptr++;
        }
    }

    frame_count++;

out_of_loop:
    *num_of_frame_holder = frame_count;
    fclose(fp);

    return byte_array;
}

bool construct_hash_table_from_byte_array(u8 *byte_array, u32 num_of_frame)
{
    u32 index = 0;
    u32 item = 0;
    u32 curr_key = 0;
    u32 ip_position = 0;
    u32 port_position = 0;
    u16 to_move_forward = 0;
    u16 src_port = 0;
    u16 dest_port = 0;
    u8 *tmp_ptr = NULL;
    bool is_success = false;
    u32 elements_in_hash_table = 0;
    ipv4_first_octet_t *first_octet = NULL;

    /*
     *  lets say the number of frame is 100, nearest prime number after that is 101
     *  so, the hash table size will be 101
     */

    is_success = create_hash_table(&hash_table, &hash_table_size, INITIAL_HASH_TABLE_SIZE);

    if (!is_success)
        return false;

    is_success = false;
    first_octet = (ipv4_first_octet_t *)get_memory(1, sizeof(ipv4_first_octet_t), __FUNCTION__);

    if (first_octet == NULL)
        return false;

    while (item < num_of_frame)
    {
        /* ip position holds where the ip address bytes start in byte array for current packet*/
        ip_position = index + ETHERNET_HEADER_BYTES_LENGTH + SRC_IP_OFFSET_FROM_IPV4_HEADER_START;
        is_success = false;

        /* copying the first octet from the ipv4 address to find out header length */
        memcpy(first_octet, &byte_array[index + ETHERNET_HEADER_BYTES_LENGTH], 1);

        /*
         *  port position holds where the port address bytes start from in byte array for current packet
         *  ipv4 header may contain options and may have length more than 20 bytes
         *  the header_length field in ipv4 address contains the size of the ipv4 header
         */
        port_position = index + ETHERNET_HEADER_BYTES_LENGTH + first_octet->header_len * 4;
        src_port = (byte_array[port_position] << 8) | byte_array[port_position + 1];
        src_port = ntohs(src_port);
        dest_port = (byte_array[port_position + 2] << 8) | byte_array[port_position + 3];
        dest_port = ntohs(dest_port);

        curr_key = hash_function(&byte_array[ip_position], src_port, dest_port);
        is_success = insert_entry_in_hash_table(curr_key, &byte_array[ip_position], src_port, dest_port, &elements_in_hash_table);

        /*
         *  let's say the current hash table has size 11 and there are 12 elements in the hash table
         *  so, this rehash function will increase the hash table size to decrease collison
         */

        if (elements_in_hash_table > hash_table_size)
            rehash(elements_in_hash_table);

        if (!is_success)
            break;

        /*
         *  to go to the next frame, we need to find out the end of the current frame
         *  the total_length field in ipv4 packet contains the number of bytes in total ipv4 packet
         *  so, reading from that value, to_move_forward skips the rest of the frame body
         *  and go to the next frame start byte position
         */

        tmp_ptr = (u8 *)&to_move_forward;
        *tmp_ptr = byte_array[index + ETHERNET_HEADER_BYTES_LENGTH + TOTAL_LENGTH_OFFSET_FROM_IPV4_HEADER_START];
        *(tmp_ptr + 1) = byte_array[index + ETHERNET_HEADER_BYTES_LENGTH + TOTAL_LENGTH_OFFSET_FROM_IPV4_HEADER_START + 1];
        to_move_forward = ntohs(to_move_forward);

        index += (ETHERNET_HEADER_BYTES_LENGTH + to_move_forward);
        item++;
    }

    /*
     *  only if the byte_array is successfully processed, this array will be cleared
     *  there's no use of this array after successfuly processing it, so its getting freed
     */

    if (is_success)
    {
        free(byte_array);
        byte_array = NULL;
    }

    free(first_octet);
    first_octet = NULL;

    return true;
}

u32 load_frames_from_file_and_prepare_hash_table()
{
    u32 num_of_frame = 0;
    u8 *byte_array = NULL;
    bool is_success = false;

    byte_array = read_frames_from_wireshark_output_file_and_construct_byte_array(&num_of_frame);

    if (byte_array == NULL)
        return DATA_NOT_LOADED_ERROR_CODE;

    is_success = construct_hash_table_from_byte_array(byte_array, num_of_frame);

    if (!is_success)
        return HASH_TABLE_NOT_CONSTRUCTED_ERROR_CODE;

    byte_array = NULL;

    return NO_ERROR_CODE;
}

void print_hash_table()
{
    hash_node_t *curr = NULL;
    i32 iter1 = 0;
    i32 iter2 = 0;
    bool keep_spaces_in_left = false;
    u32 total = 0;

    printf("Hash Table:\n");

    for (iter1 = 0; iter1 < HASH_TABLE_WIDTH; iter1++)
        printf("-");

    printf("\n");
    printf("|%14s |%14s |%19s |%9s |%19s |%9s |%10s |", "HASH INDEX", "KEY", "SRC_IP", "SRC_PORT", "DST_IP", "DST_PORT", "COUNTER");
    printf("\n");

    for (iter1 = 0; iter1 < HASH_TABLE_WIDTH; iter1++)
        printf("-");

    printf("\n");

    for (iter1 = 0; iter1 < hash_table_size; iter1++)
    {
        printf("|%14d |", iter1);
        curr = hash_table[iter1];
        keep_spaces_in_left = false;

        if (hash_table[iter1] == NULL)
            printf("%*s\n", HASH_TABLE_WIDTH - HASH_TABLE_EMPTY_ROW_PRINT_OFFSET, "|");

        while (curr != NULL)
        {
            if (keep_spaces_in_left)
                printf("|%14s |", "");

            printf("%14u |", curr->key);
            printf("%7hhu.%3hhu.%3hhu.%3hhu |", curr->src_ip[0], curr->src_ip[1], curr->src_ip[2], curr->src_ip[3]);
            printf("%9hu |", curr->src_port);
            printf("%7hhu.%3hhu.%3hhu.%3hhu |", curr->dest_ip[0], curr->dest_ip[1], curr->dest_ip[2], curr->dest_ip[3]);
            printf("%9hu |", curr->dest_port);
            printf("%10u |\n", curr->counter);
            total += curr->counter;
            curr = curr->next;
            keep_spaces_in_left = true;

            if (curr == NULL)
                continue;

            if (keep_spaces_in_left)
                printf("|%14s ", "");

            for (iter2 = HASH_TABLE_EMPTY_ROW_PRINT_OFFSET; iter2 < HASH_TABLE_WIDTH; iter2++)
                printf("-");

            printf("\n");
        }

        for (iter2 = 0; iter2 < HASH_TABLE_WIDTH; iter2++)
            printf("-");

        printf("\n");
    }

    printf("|%*s |%10u |\n", HASH_TABLE_WIDTH - HASH_TABLE_TOTAL_ROW_PRINT_OFFSET, "TOTAL", total);

    for (iter2 = 0; iter2 < HASH_TABLE_WIDTH; iter2++)
        printf("-");

    printf("\n");

    return;
}

void free_all_pointers()
{
    hash_node_t *h_curr = NULL, *h_prev = NULL;
    u32 iter = 0;

    for (iter = 0; iter < hash_table_size; iter++)
    {
        h_curr = hash_table[iter];
        h_prev = hash_table[iter];

        while (h_prev != NULL)
        {
            h_prev->next = NULL;
            h_prev = h_curr;

            if (h_curr != NULL)
                h_curr = h_curr->next;
        }
    }

    free(hash_table);
    hash_table = NULL;

    return;
}
