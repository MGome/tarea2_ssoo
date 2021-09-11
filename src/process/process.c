#include <stdio.h>
#include <stdlib.h>
#include "process.h"

Process* process_init(int pid, char* name, int fabric, int bursts)
{
    Process* proceso = malloc(sizeof(Process))
    proceso -> pid = pid;
    proceso -> name = calloc(255, sizeof(char));
    proceso -> fabric = fabric;
    procso -> burst = burst;

    return proceso;
}