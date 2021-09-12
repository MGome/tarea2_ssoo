#include "process.h"
#include <stdio.h>
#include <stdlib.h>

Process* process_init(int pid, char* name, int fabric, int qty_burst, int time_init)
{
    Process* proceso = malloc(sizeof(Process));
    proceso -> pid = pid;
    proceso -> name = name;
    proceso -> fabric = fabric;
    proceso -> qty_burst = qty_burst;
    proceso -> status = READY;
    proceso -> time_init = time_init;

    proceso -> next = NULL;
    // // int array [bursts]
    return proceso;
}

// void eliminate_procces(Process* process)
// {

// }