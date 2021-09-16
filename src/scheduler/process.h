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

    // Para el output
    int n_chosen;
    int n_interrupt;
    int response_time;
    int waiting_time;
    int first_tag;
    int llego_cola;
    // int turnaround_time; Este se puede implementar dentro de la linea finished

};

// Metodos
Process* process_init(int pid, char* name, int fabric, int qty_burst, int time_init);
void print_process(Process* process);