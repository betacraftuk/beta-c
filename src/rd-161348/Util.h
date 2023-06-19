#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#define frand (float) rand() / (float) RAND_MAX

typedef struct Node {
    void* data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    int size;
} LinkedList;

uint64_t util_getTimeInNano();
uint64_t util_getTimeInMs();
LinkedList* util_linkedlist_create();
void util_linkedlist_add(LinkedList* list, void* data);
void util_linkedlist_remove_node(LinkedList* list, Node* node);
void util_linkedlist_free(LinkedList* list);

#endif
