#pragma once

#include "process.h"

typedef struct queue Queue;

struct queue
{
    Process* head;
    Process* tail;
    int len;
    int* procesosXfabrica; // procesos por fabrica
};

// // Metodos
Queue* queue_init();
void list_append(Queue* list, Process* node);
void list_print(Queue* list);
int list_finish_pop(Queue* list);
void list_destroy(Queue* list);
void list_sort(Queue* list, Process* process);
Process* list_pop_comeback(Queue* list);