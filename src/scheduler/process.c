#include "process.h"
#include <stdio.h>
#include <stdlib.h>

Process* process_init(int pid, char* name, int fabric, int qty_burst, int time_init)
{
    Process* proceso = malloc(sizeof(Process));
    int* bursts = calloc(qty_burst, sizeof(int));
    int* io_burst = calloc(qty_burst-1, sizeof(int));

    proceso -> pid = pid;
    proceso -> name = name;
    proceso -> fabric = fabric;
    proceso -> qty_burst = qty_burst;
    proceso -> status = READY;
    proceso -> time_init = time_init;
    proceso -> bursts = bursts;
    proceso -> waits = io_burst;

    proceso -> next = NULL;
    proceso -> waits_id = 0;
    proceso -> bursts_id = 0;
    // // int array [bursts]
    return proceso;
}

void print_process(Process* process)
{
    printf("Pid: %i, fabrix: %i, time_init: %i\n", process->pid, process-> fabric, process-> time_init);
}

// void eliminate_procces(Process* process)
// {

// }