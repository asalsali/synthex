#include <stdlib.h>

/* Function: process_data */
/* This function processes data based on the given parameters and returns a pointer or error. */
long process_data(long data_pointer, int count) {
    int index;
    long temp_pointer;
    long result_pointer;
    long *data_list;
    long allocated_memory;
    long current_memory;
    long total_size;
    long local_stack;

    local_stack = *(long *)(in_FS_OFFSET + 0x28);
    allocated_memory = data_pointer;
    total_size = count;

    if (count < 1) {
        return -1; // Error case
    }

    data_list = (long *)malloc(0x18);
    if (data_list == NULL) {
        return -1; // Error case
    }

    data_list[0] = 0;
    data_list[1] = 0;
    data_list[2] = 0;
    current_memory = (long)data_list;
    total_size = 0;

    for (index = 0; index < count; index++) {
        temp_pointer = *(long *)(allocated_memory + (long)index * 8);
        if (temp_pointer == 0) {
            continue; // Skip to next iteration
        }
        int size = *(int *)(temp_pointer + 4);
        if (size < 0) {
            continue; // Skip to next iteration
        }

        data_list = (long *)malloc(0x18);
        if (data_list == NULL) {
            free_memory(current_memory);
            return -1; // Error case
        }

        data_list[0] = temp_pointer;
        data_list[1] = size;
        data_list[2] = 0;
        *(long *)(current_memory + 0x10) = (long)data_list;
        current_memory = (long)data_list;
        total_size += (long)size;
    }

    if (total_size == 0) {
        free_memory(current_memory);
        return -1; // Error case
    }

    *(long *)(current_memory + 8) = total_size;
    return current_memory;
}

/* Function: free_memory */
/* This function frees the allocated memory for the given pointer. */
void free_memory(long memory_pointer) {
    long next_pointer;

    while (memory_pointer != 0) {
        next_pointer = *(long *)(memory_pointer + 0x10);
        free((void *)memory_pointer);
        memory_pointer = next_pointer;
    }
}

/* Function: find_valid_pointer */
/* This function finds a valid pointer based on the given parameters. */
long find_valid_pointer(long data_pointer, long param) {
    int valid_count;
    long temp_pointer;
    long result_pointer;
    long current_pointer;
    long local_stack;

    local_stack = *(long *)(in_FS_OFFSET + 0x28);
    if (data_pointer == 0) {
        return 0; // No valid pointer
    }

    current_pointer = *(long *)(data_pointer + 0x10);
    valid_count = 0;
    result_pointer = 0;

    while (current_pointer != 0) {
        temp_pointer = *(long *)current_pointer;
        if (temp_pointer != 0) {
            int is_valid = compare_strings(temp_pointer, param);
            if (is_valid != 0) {
                valid_count++;
                result_pointer = current_pointer;
            }
        }
        current_pointer = *(long *)(current_pointer + 0x10);
    }

    if (valid_count == 0) {
        return 0; // No valid pointer
    }
    return result_pointer;
}

/* Function: compare_strings */
/* This function compares two strings and returns 1 if they are equal, 0 otherwise. */
int compare_strings(long str1, long str2) {
    int index = 0;

    while (*(char *)(str1 + (long)index) != '\0') {
        if (*(char *)(str1 + (long)index) != *(char *)(str2 + (long)index)) {
            return 0; // Strings are not equal
        }
        index++;
    }
    if (*(char *)(str2 + (long)index) != '\0') {
        return 0; // Strings are not equal
    }
    return 1; // Strings are equal
}