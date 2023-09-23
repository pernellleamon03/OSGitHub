// list/list.c
// Implementation for linked list.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

list_t *list_alloc() { 
    list_t* mylist = (list_t *) malloc(sizeof(list_t));
    mylist->head = NULL;
    return mylist;
}

void list_free(list_t *l) {
    // Free all nodes in the list
    node_t* temp;
    while (l->head != NULL) {
        temp = l->head;
        l->head = l->head->next;
        free(temp);
    }
    // Free the list structure itself
    free(l);
}

void list_print(list_t *l) {
    node_t* curr = l->head;
    while (curr != NULL) {
        printf("%d->", curr->value);
        curr = curr->next;
    }
    printf("NULL\n");
}

char *listToString(list_t *l) {
    char* buf = (char *) malloc(sizeof(char) * 1024);
    char tbuf[20];

    buf[0] = '\0'; 

    node_t* curr = l->head;
    while (curr != NULL) {
        sprintf(tbuf, "%d->", curr->value);
        strcat(buf, tbuf);
        curr = curr->next;
    }
    strcat(buf, "NULL");
    return buf;
}

int list_length(list_t *l) {
    int length = 0;
    node_t* curr = l->head;
    while (curr != NULL) {
        length++;
        curr = curr->next;
    }
    return length;
}

void list_add_to_back(list_t *l, elem value) {
    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;

    if (l->head == NULL) {
        l->head = new_node;
    } else {
        node_t* curr = l->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }
}

void list_add_to_front(list_t *l, elem value) {
    node_t* new_node = (node_t *) malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = l->head;
    l->head = new_node;
}

void list_add_at_index(list_t *l, elem value, int index) {
    int i; // Declare 'i' outside the loop

    if (index < 0 || index > list_length(l)) {
        // Invalid index
        return;
    }

    if (index == 0) {
        list_add_to_front(l, value);
    } else {
        node_t* new_node = (node_t *) malloc(sizeof(node_t));
        new_node->value = value;
        new_node->next = NULL;

        node_t* curr = l->head;
        for (i = 0; i < index - 1; i++) {
            curr = curr->next;
        }

        new_node->next = curr->next;
        curr->next = new_node;
    }
}

elem list_remove_from_back(list_t *l) {
    if (l->head == NULL) {
        // List is empty
        return -1; 
    }

    if (l->head->next == NULL) {
        // Only one element in the list
        elem value = l->head->value;
        free(l->head);
        l->head = NULL;
        return value;
    }

    // Traverse to the second-to-last node
    node_t* curr = l->head;
    while (curr->next->next != NULL) {
        curr = curr->next;
    }

    elem value = curr->next->value;
    free(curr->next);
    curr->next = NULL;
    return value;
}

elem list_remove_from_front(list_t *l) {
    if (l->head == NULL) {
        // List is empty
        return -1; 
    }

    node_t* temp = l->head;
    elem value = temp->value;
    l->head = l->head->next;
    free(temp);
    return value;
}

elem list_remove_at_index(list_t *l, int index) {
    int i; // Declare 'i' outside the loop

    if (index < 0 || index >= list_length(l) || l->head == NULL) {
        // Invalid index or list is empty
        return -1; 
    }

    if (index == 0) {
        return list_remove_from_front(l);
    }

    node_t* curr = l->head;
    for (i = 0; i < index - 1; i++) {
        curr = curr->next;
    }

    node_t* temp = curr->next;
    elem value = temp->value;
    curr->next = temp->next;
    free(temp);
    return value;
}

bool list_is_in(list_t *l, elem value) {
    node_t* curr = l->head;
    while (curr != NULL) {
        if (curr->value == value) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

elem list_get_elem_at(list_t *l, int index) {
    int i;
    if (index < 0 || index >= list_length(l) || l->head == NULL) {
        // Invalid index or list is empty
        return -1; 
    }

    node_t* curr = l->head;
    for (i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->value;
}

int list_get_index_of(list_t *l, elem value) {
    node_t* curr = l->head;
    int index = 0;
    while (curr != NULL) {
        if (curr->value == value) {
            return index;
        }
        curr = curr->next;
        index++;
    }
    return -1; // Value not found
}

