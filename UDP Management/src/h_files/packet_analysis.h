#ifndef __PACKET_ANALYSIS_H__
#define __PACKET_ANALYSIS_H__

#include "utils.h"

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || defined(_BIG_ENDIAN)
    #define IS_BIG_ENDIAN 1
#else
    #define IS_LITTLE_ENDIAN 1
#endif

#define ETHERNET_HEADER_BYTES_LENGTH 14
#define IPV4_ADDRESS_BYTES_LENGTH 4
#define MAXIMUM_READ_BUFFER_SIZE 1024
#define MINIMUM_BYTE_ARRAY_SIZE 100
#define MAXIMUM_BYTE_ARRAY_SIZE 100000000
#define INITIAL_HASH_TABLE_SIZE 11
#define SRC_IP_OFFSET_FROM_IPV4_HEADER_START 12
#define DEST_IP_OFFSET_FROM_IPV4_HEADER_START 16
#define TOTAL_LENGTH_OFFSET_FROM_IPV4_HEADER_START 2
#define HASH_TABLE_WIDTH 109
#define DATA_NOT_LOADED_ERROR_CODE 1
#define HASH_TABLE_NOT_CONSTRUCTED_ERROR_CODE 2
#define NO_ERROR_CODE 0
#define HASH_TABLE_TOTAL_ROW_PRINT_OFFSET 15
#define HASH_TABLE_EMPTY_ROW_PRINT_OFFSET 17

#pragma pack(1)
typedef struct ipv4_first_octet
{
#ifdef IS_BIG_ENDIAN
    u8 version : 4;
    u8 header_len : 4;
#endif

#ifdef IS_LITTLE_ENDIAN
    u8 header_len : 4;
    u8 version : 4;
#endif
} ipv4_first_octet_t;

#pragma pack(1)
typedef struct hash_node
{
    u32 key;
    u8 src_ip[4];
    u8 dest_ip[4];
    u16 src_port;
    u16 dest_port;
    u32 counter;
    struct hash_node *next;
} hash_node_t;

u32 load_frames_from_file_and_prepare_hash_table();
void print_hash_table();
void free_all_pointers();

#endif /* __PACKET_ANALYSIS_H__ */