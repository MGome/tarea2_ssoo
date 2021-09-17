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
    proceso -> bursts = calloc(qty_burst, sizeof(int));;
    proceso -> waits = calloc(qty_burst-1, sizeof(int));;

    proceso -> next = NULL;
    proceso -> waits_id = 0;
    proceso -> bursts_id = 0;
    proceso -> first_tag = -1;
    proceso -> n_chosen= 0;
    proceso -> n_interrupt= 0;
    proceso -> response_time= 0;
    proceso -> waiting_time= 0;
    proceso -> llego_cola = 0;
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