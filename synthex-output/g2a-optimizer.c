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
        temp_pointer = *(long *)(data_pointer + (long)index * 8);
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
    long temp_pointer;
    long result_pointer = 0;
    long current_pointer;

    if (data_pointer == 0) {
        return 0; // No valid pointer
    }

    current_pointer = *(long *)(data_pointer + 0x10);
    int valid_count = 0;

    while (current_pointer != 0) {
        temp_pointer = *(long *)current_pointer;
        if (temp_pointer != 0) {
            if (compare_strings(temp_pointer, param)) {
                valid_count++;
                result_pointer = current_pointer;
            }
        }
        current_pointer = *(long *)(current_pointer + 0x10);
    }

    return (valid_count == 0) ? 0 : result_pointer; // Return 0 if no valid pointer
}

/* Function: compare_strings */
/* This function compares two strings and returns 1 if they are equal, 0 otherwise. */
int compare_strings(long str1, long str2) {
    int index = 0;

    while (*(char *)(str1 + index) != '\0') {
        if (*(char *)(str1 + index) != *(char *)(str2 + index)) {
            return 0; // Strings are not equal
        }
        index++;
    }
    return (*(char *)(str2 + index) == '\0') ? 1 : 0; // Return 1 if strings are equal
}