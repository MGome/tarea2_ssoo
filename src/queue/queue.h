#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef struct queue
{
    Process** procesos;
} Queue;

// Metodos
Queue* queue_init()