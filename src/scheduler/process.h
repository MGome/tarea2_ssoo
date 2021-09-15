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
    int pid;
    char* name;
    int fabric;
    Estado status;
    int qty_burst;
    Process* next;
    int* bursts;
    int* waits;
    int time_init;
    int waits_id;
    int bursts_id;

    // Quizas es necesario incluir los parametros de output
    // dentro del struct. Tambien considerar los Ai Bi, que
    // corresponden a los bursts.
};

// Metodos
Process* process_init(int pid, char* name, int fabric, int qty_burst, int time_init);
void print_process(Process* process);