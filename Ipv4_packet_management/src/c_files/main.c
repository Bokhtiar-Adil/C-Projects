#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../h_files/packet_analysis.h"

int main(void)
{
    u32 is_success = 0;

    printf("**********************************\n");
    printf("***** IPV4 Packet Management *****\n");
    printf("**********************************\n");
    printf("\n");
    
    is_success = load_frames_from_file_and_prepare_hash_table();

    if (is_success == DATA_NOT_LOADED_ERROR_CODE)
    {
        printf("ERROR: Data could not be loaded from file.\n");

        return 0;
    }
    else if (is_success == LIST_OR_HASH_TABLE_NOT_CONSTRUCTED_ERROR_CODE)
    {
        printf("ERROR: List or hash table could not be created.\n");

        return 0;
    }
    else
        printf("Data successfully loaded.\n"
               "List and hash table successfully created.\n");

    print_list();
    print_hash_table();
    free_all_pointers();

    return 0;
}