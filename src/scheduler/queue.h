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
void list_append(Queue* list, Process* process);
void list_print(Queue* list);
Process* list_process_exchange(Queue* list);
void list_destroy(Queue* list);
void list_sort(Queue* list, Process* process);
Process* list_pop_comeback(Queue* list);
void list_pop_finish(Queue* list);