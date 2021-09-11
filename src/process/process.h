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

typedef struct process
{
    int pid;
    char* name;
    int fabric;
    Estado status;
    int burst;

    // Quizas es necesario incluir los parametros de output
    // dentro del struct. Tambien considerar los Ai Bi, que
    // corresponden a los bursts.
} Process;

// Metodos
Process* process_init(int pid, char* name, int fabric, int bursts);