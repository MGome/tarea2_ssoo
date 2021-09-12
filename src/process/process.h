#include <stdio.h>
#include <stdlib.h>
#pragma once

typedef enum estado
{
    READY,
    RUNNING,
    WAITING,
    FINISHED,
    
} Estado;

struct process;
typedef struct process Process;
struct process
{
    // int pid;
    // char* name;
    // int fabric;
    // //Estado status;
    // int qty_burst;
    // Process* next;
    // int* bursts;

    // Quizas es necesario incluir los parametros de output
    // dentro del struct. Tambien considerar los Ai Bi, que
    // corresponden a los bursts.
};

// Metodos
Process* process_init();