#include "Util.h"

#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <stddef.h>

uint64_t startTime = 0;

uint64_t getRealTimeInNano() {
    struct timeval time;
	gettimeofday(&time, NULL);
    return 1000 * ((uint64_t)(time.tv_sec % 10000) * 1000000 + (uint64_t)time.tv_usec);
}

uint64_t util_getTimeInNano() {
    uint64_t t = getRealTimeInNano();
    if (startTime == 0) {
        startTime = t;
    }
	return t - startTime;
}

uint64_t util_getTimeInMs() {
    return getRealTimeInNano() / 1000000;
}

LinkedList* util_linkedlist_create() {
    LinkedList* list = malloc(sizeof(LinkedList));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void util_linkedlist_add(LinkedList* list, void* data) {
    list->size++;
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    node->prev = list->tail;

    if (list->head == NULL) {
        list->head = node;
        list->tail = node;
        return;
    }

    list->tail->next = node;
    list->tail = node;
}

void util_linkedlist_remove_node(LinkedList* list, Node* node) {
    Node* trash = node;

    if (node == list->head) {
        if (node->next != NULL) {
            node->next->prev = NULL;
            list->head = node->next;
        } else {
            list->head = NULL;
        }
    } else if (node == list->tail) {
        Node* tailPrev = list->tail->prev;
        tailPrev->next = NULL;
        list->tail = tailPrev;
    } else if (node->next != NULL) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        node = node->next;
    }

    free(trash->data);
    free(trash);
    list->size--;
}

void util_linkedlist_free(LinkedList* list) {
    Node* cur = list->head;

    while (cur != NULL) {
        Node* nextNode = cur->next;
        free(cur->data);
        free(cur);
        cur = nextNode;
    }

    free(list);
}
