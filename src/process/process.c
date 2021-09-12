#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process* process_init()
{
    Process* proceso = malloc(sizeof(Process));
    // proceso -> pid = pid;
    // proceso -> name [255] = name;
    // proceso -> fabric = fabric;
    // proceso -> qty_burst = qty_burst;
    // proceso -> status = READY;
    // proceso -> bursts [qty_burst];

    // proceso -> next = malloc(sizeof(Process));
    // // int array [bursts]
    return proceso;
}

// void eliminate_procces(Process* process)
// {

// }