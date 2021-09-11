#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

Queue* queue_init()
{
    Queue* cola = malloc(sizeof(Queue));
    cola -> procesos = calloc(8, sizeof(Process));

    return cola;
}