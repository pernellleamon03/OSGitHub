#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    // Test list_alloc and list_free functions
    list_t *mylist = list_alloc();
    list_free(mylist);
    mylist = list_alloc();
    list_free(mylist);

    // Test list_add_to_back function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    list_print(mylist);
    list_free(mylist);

    // Test list_add_to_front function
    mylist = list_alloc();
    list_add_to_front(mylist, 10);
    list_add_to_front(mylist, 20);
    list_add_to_front(mylist, 30);
    list_print(mylist);
    list_free(mylist);

    // Test list_add_at_index function
    mylist = list_alloc();
    list_add_at_index(mylist, 10, 0);
    list_add_at_index(mylist, 20, 1);
    list_add_at_index(mylist, 30, 1);
    list_print(mylist);
    list_free(mylist);

    // Test list_remove_from_back function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    elem removed_value = list_remove_from_back(mylist);
    printf("Removed from back: %d\n", removed_value);
    list_print(mylist);
    list_free(mylist);

    // Test list_remove_from_front function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    removed_value = list_remove_from_front(mylist);
    printf("Removed from front: %d\n", removed_value);
    list_print(mylist);
    list_free(mylist);

    // Test list_remove_at_index function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    removed_value = list_remove_at_index(mylist, 1);
    printf("Removed at index 1: %d\n", removed_value);
    list_print(mylist);
    list_free(mylist);

    // Test list_is_in function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    int value_to_check = 20;
    if (list_is_in(mylist, value_to_check)) {
        printf("%d is in the list.\n", value_to_check);
    } else {
        printf("%d is not in the list.\n", value_to_check);
    }
    list_free(mylist);

    // Test list_get_elem_at function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    int index_to_get = 1;
    elem retrieved_value = list_get_elem_at(mylist, index_to_get);
    printf("Value at index %d: %d\n", index_to_get, retrieved_value);
    list_free(mylist);

    // Test list_get_index_of function
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    int value_to_search = 20;
    int index_found = list_get_index_of(mylist, value_to_search);
    printf("Index of %d: %d\n", value_to_search, index_found);
    list_free(mylist);

    // Edge case: Indexing a value not in the list
    mylist = list_alloc();
    list_add_to_back(mylist, 10);
    list_add_to_back(mylist, 20);
    list_add_to_back(mylist, 30);
    index_to_get = 5;
    retrieved_value = list_get_elem_at(mylist, index_to_get);
    if (retrieved_value == -1) {
        printf("Value at index %d not found (edge case handled).\n", index_to_get);
    } else {
        printf("Value at index %d: %d\n", index_to_get, retrieved_value);
    }
    list_free(mylist);

    // Edge case: Removing elements from an empty list
    mylist = list_alloc();
    removed_value = list_remove_from_back(mylist);
    if (removed_value == -1) {
        printf("Removed from back: -1 (list is empty - edge case handled).\n");
    } else {
        printf("Removed from back: %d\n", removed_value);
    }
    list_free(mylist);

    return 0;
}
