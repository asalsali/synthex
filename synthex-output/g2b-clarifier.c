#include <stdlib.h>

#define NODE_SIZE sizeof(long) // Size of each node in the data list
#define ERROR_RESULT -1 // Error return value
#define STRING_OFFSET 0x10 // Offset for string in node structure
#define LOCAL_STACK_OFFSET 0x28 // Offset for local stack pointer

/* Function: process_data */
/* @brief Processes data based on the given parameters and returns a pointer to the allocated memory or an error code. 
   @param data_pointer Pointer to the data to be processed.
   @param count Number of elements to process.
   @return Pointer to the allocated memory containing processed data or ERROR_RESULT on error. */
long process_data(long data_pointer, int count) {
    int index;
    long temp_pointer;
    long result_pointer;
    long *data_list;
    long allocated_memory = data_pointer;
    long current_memory;
    long total_size = 0;

    // Check for invalid count
    if (count < 1) {
        return ERROR_RESULT; // Error case
    }

    // Allocate memory for the data list
    data_list = (long *)malloc(NODE_SIZE);
    if (data_list == NULL) {
        return ERROR_RESULT; // Error case
    }

    // Initialize the first node
    data_list[0] = 0; // Placeholder for data pointer
    data_list[1] = 0; // Placeholder for size
    data_list[2] = 0; // Placeholder for next node
    current_memory = (long)data_list;

    // Process each element in the data
    for (index = 0; index < count; index++) {
        temp_pointer = *(long *)(allocated_memory + (long)index * sizeof(long));
        if (temp_pointer == 0) {
            continue; // Skip to next iteration if pointer is null
        }
        int size = *(int *)(temp_pointer + 4);
        if (size < 0) {
            continue; // Skip to next iteration if size is invalid
        }

        // Allocate memory for the new node
        data_list = (long *)malloc(NODE_SIZE);
        if (data_list == NULL) {
            free_memory(current_memory);
            return ERROR_RESULT; // Error case
        }

        // Initialize the new node
        data_list[0] = temp_pointer; // Store the data pointer
        data_list[1] = size; // Store the size
        data_list[2] = 0; // Next node is null
        *(long *)(current_memory + STRING_OFFSET) = (long)data_list; // Link the new node
        current_memory = (long)data_list; // Move to the new node
        total_size += (long)size; // Accumulate total size
    }

    // Check if no valid data was processed
    if (total_size == 0) {
        free_memory(current_memory);
        return ERROR_RESULT; // Error case
    }

    // Store the total size in the last node
    *(long *)(current_memory + 8) = total_size;
    return current_memory; // Return pointer to the processed data
}

/* Function: free_memory */
/* @brief Frees the allocated memory for the given pointer. 
   @param memory_pointer Pointer to the memory to be freed. */
void free_memory(long memory_pointer) {
    long next_pointer;

    // Free each node in the linked list
    while (memory_pointer != 0) {
        next_pointer = *(long *)(memory_pointer + STRING_OFFSET);
        free((void *)memory_pointer);
        memory_pointer = next_pointer; // Move to the next node
    }
}

/* Function: find_valid_pointer */
/* @brief Finds a valid pointer based on the given parameters. 
   @param data_pointer Pointer to the data list.
   @param param Pointer to the string to compare against.
   @return Pointer to the last valid node or 0 if none found. */
long find_valid_pointer(long data_pointer, long param) {
    int valid_count = 0;
    long temp_pointer;
    long result_pointer = 0;
    long current_pointer;

    // Check for null data pointer
    if (data_pointer == 0) {
        return 0; // No valid pointer
    }

    current_pointer = *(long *)(data_pointer + STRING_OFFSET);

    // Traverse the linked list to find valid pointers
    while (current_pointer != 0) {
        temp_pointer = *(long *)current_pointer;
        if (temp_pointer != 0) {
            int is_valid = compare_strings(temp_pointer, param);
            if (is_valid != 0) {
                valid_count++;
                result_pointer = current_pointer; // Update result pointer
            }
        }
        current_pointer = *(long *)(current_pointer + STRING_OFFSET); // Move to the next node
    }

    // Return the last valid pointer found or 0 if none
    return (valid_count == 0) ? 0 : result_pointer;
}

/* Function: compare_strings */
/* @brief Compares two strings and returns 1 if they are equal, 0 otherwise. 
   @param str1 Pointer to the first string.
   @param str2 Pointer to the second string.
   @return 1 if strings are equal, 0 otherwise. */
int compare_strings(long str1, long str2) {
    int index = 0;

    // Compare characters of both strings
    while (*(char *)(str1 + (long)index) != '\0') {
        if (*(char *)(str1 + (long)index) != *(char *)(str2 + (long)index)) {
            return 0; // Strings are not equal
        }
        index++;
    }
    // Check if both strings have the same length
    if (*(char *)(str2 + (long)index) != '\0') {
        return 0; // Strings are not equal
    }
    return 1; // Strings are equal
}