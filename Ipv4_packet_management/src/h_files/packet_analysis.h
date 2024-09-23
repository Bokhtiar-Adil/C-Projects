#ifndef __PACKET_ANALYSIS_H__
#define __PACKET_ANALYSIS_H__

#include "utils.h"

#define MAXIMUM_TRANSMISSION_UNIT 1500
#define MAXIMUM_FRAME_BYTE_LENGTH 1518
#define ETHERNET_HEADER_BYTES_LENGTH 14
#define IPV4_HEADER_BYTES_MIN_LENGTH 20
#define UDP_HEADER_BYTES_LENGTH 16
#define MAC_ADDRESS_BYTES_LENGTH 6
#define IPV4_ADDRESS_BYTES_LENGTH 4
#define MAXIMUM_READ_BUFFER_SIZE 1024
#define MINIMUM_BYTE_ARRAY_SIZE 100
#define MAXIMUM_BYTE_ARRAY_SIZE 100000000
#define INITIAL_HASH_TABLE_SIZE 11
#define SRC_IP_OFFSET_FROM_IPV4_HEADER_START 12
#define DEST_IP_OFFSET_FROM_IPV4_HEADER_START 16
#define TOTAL_LENGTH_OFFSET_FROM_IPV4_HEADER_START 2
#define LIST_TABLE_WIDTH 67
#define HASH_TABLE_WIDTH 87
#define DATA_NOT_LOADED_ERROR_CODE 1
#define LIST_OR_HASH_TABLE_NOT_CONSTRUCTED_ERROR_CODE 2
#define NO_ERROR_CODE 0
#define LIST_TOTAL_ROW_PRINT_OFFSET 14
#define HASH_TABLE_EMPTY_ROW_PRINT_OFFSET 17

#pragma pack(1)
typedef struct list_node
{
    u8 src_ip[4];
    u8 dest_ip[4];
    u32 counter;
    struct list_node *next;
} list_node_t;

#pragma pack(1)
typedef struct hash_node
{
    u32 key;
    list_node_t *data;
    struct hash_node *next;
} hash_node_t;

u32 load_frames_from_file_and_prepare_hash_table();
void print_list();
void print_hash_table();
void free_all_pointers();

#endif /* __PACKET_ANALYSIS_H__ */