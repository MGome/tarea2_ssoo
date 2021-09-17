#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./../file_manager/manager.h"
#include "process.h"
#include "queue.h"

// Se declaran las variables globales que almacenan
// la cantidad de procesos por fabrica
int process_id = 0;
int Q = 100;
int time = 0;

Queue* incoming_process(Queue* Procesos, int time)
{
  // Procesos que ingresan a la cola en el tiempo :time:
  Queue* new_processes = queue_init();
  Process* current = Procesos -> head;
  while (Procesos -> len > 0)
  {
    if (current -> time_init == time)
    {
      Process* new_process = list_process_exchange(Procesos);
      new_process ->llego_cola = time;
      list_sort(new_processes, new_process);
      current = Procesos -> head;
    } else {
      break;
    }
  }
  // printf("REVISANDO %i\n", new_processes->head->next->pid);
  return new_processes;
}

int calculate_quantum(Queue* Cola)
{
  int f_i = Cola -> head -> fabric - 1; // id de la fabrica
  int n_i = Cola -> procesosXfabrica[f_i];  // n_i

  int f = 0;
  for (int i = 0; i < 4; i++){
    if (Cola -> procesosXfabrica[i] > 0)
    {
      f++;
    }
  }
  return Q/ (n_i * f);
}

void wait_sum(Queue* Cola)
{
  for(Process* current = Cola -> head; current; current = current -> next)
    {
      if (current -> status == WAITING)
      {
        int id = current -> waits_id;
        current -> waits[id] -= 1;
        current -> waiting_time++;

        if (current -> waits[id] == -1)
        {
          current -> status = READY;
          current -> waits_id += 1;
          current -> waiting_time++;

          printf("[t = %i] El proceso %s ha pasado a estado READY por haber terminado su IO Burst\n", time, current -> name);
        }
      } else if (current -> status == READY)
      {
        current -> waiting_time++;
      }
    }
}

// Flujo principal
int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  InputFile *file = read_file(argv[1]);
  // TODO: HACER OUTPUT
  
  if (argc == 4) {
    Q = atoi(argv[3]);
  }

  printf("%i\n", file -> len);

  Queue* Procesos = queue_init();

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];

    Process* Proceso = process_init(i, line[0], atoi(line[2]), atoi(line[3]) , atoi(line[1]));

    // Se crean los arrelos para separar los CPU Bursts y IO Burst
    int cantidad_burst = atoi(line[3]);
    int iteraciones_arreglos = 2*cantidad_burst-1;

    int indice_burst = 0;
    int indice_io = 0;

    // Desde line[4] comienzan los argumentos de los bursts y wait
    for (int i=0; i < iteraciones_arreglos; i++)
    {
      int indice_recorrido = 4+i;
      if (i%2 == 0)
      {
        // Es un cpu burst
        // int entero = atoi(line[indice_recorrido]);
        Proceso -> bursts[indice_burst] = atoi(line[indice_recorrido]);
        indice_burst++;
      } else
      {
        // Es un io burst
        Proceso -> waits[indice_io] = atoi(line[indice_recorrido]);
        indice_io++;
      }
    }
    list_append(Procesos, Proceso);
    
  };

  Queue* Cola = queue_init();
  int quantum = 0;
  Process* executing_process = NULL;
  int qty_process = Procesos -> len;
  int process_finished = 0;
  bool primera_vuelta = true;
  
  while (qty_process != process_finished)
  {

    Queue* incoming = incoming_process(Procesos, time);
    
    // Cuando no hay procesos en la Cola, todos los procesos entrantes ingresan al final de la cola.
    // TODO: desempate entre procesos.

    if (incoming -> len > 0 && primera_vuelta)
    { 
      while (incoming -> len > 0)
      {
        Process* new_process = list_process_exchange(incoming);
        printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
        list_append(Cola, new_process);      
      }
      primera_vuelta = false;
      list_destroy(incoming);

    } else if (Cola -> len == 0 && !executing_process) {
      printf("[t = %i] No hay ningún proceso ejecutando en la CPU.\n", time);
      time++;
      list_destroy(incoming);
      continue;
    }

    // list_destroy(incoming);
    // printf("--------\n");
    // list_print(Cola);
    // printf("--------\n");

    // Proceso se ejecuta

    if (!executing_process) // No hay proceso en ejecución
    { 
      quantum = calculate_quantum(Cola);  // se calcula su quantum

      // quantum = 4;
      executing_process = list_process_exchange(Cola); // se extrae el proceso en la cabeza
      if (executing_process->status == WAITING)
      {
        list_append(Cola, executing_process);
        executing_process = NULL;
        for(Process* current = Cola -> head; current; current = current -> next)
        {
          if (current -> status == WAITING)
          {
            int id = current -> waits_id;
            current -> waits[id] -= 1;
            current -> waiting_time++;

            if (current -> waits[id] == -1)
            {
              current -> status = READY;
              current -> waits_id += 1;
              printf("[t = %i] El proceso %s ha pasado a estado READY por haber terminado su IO Burst\n", time, current -> name);
              time--;
            }
          } else if (current -> status == READY)
          {
            current -> waiting_time++;
          }
        }

        
        if (incoming -> len > 0)
        { 
          while (incoming -> len > 0)
          {
            Process* new_process = list_process_exchange(incoming);
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);      
          }
        list_destroy(incoming);

        } else if (Cola -> len == 0 && !executing_process) {
          printf("[t = %i] No hay ningún proceso ejecutando en la CPU.\n", time);
          time++;
          continue;
        }
        time++;
        continue;
      }
      // printf("Nodo siguiente: %i \n", Cola -> head -> pid);
    }
    
    if (quantum == 0 && executing_process -> bursts[executing_process -> bursts_id] != 0)
    {
      if (executing_process -> status == RUNNING)
      {
        executing_process -> status = READY;
        executing_process -> n_interrupt++;
        printf("[t = %i] El proceso %s ha pasado a estado READY\n", time, executing_process -> name);
        list_append(Cola, executing_process);
        executing_process = NULL;
        if (incoming -> len > 0)
        { 
          while (incoming -> len > 0)
          {
            Process* new_process = list_process_exchange(incoming);
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);      
          }
        

        }
        list_destroy(incoming);
        continue;

      } 
    } else if (quantum != 0)
    {
    
      if (executing_process -> status == READY)
      {
        executing_process -> status = RUNNING;
        executing_process -> n_chosen++;
        if (executing_process -> first_tag == -1)
        {
          executing_process -> response_time = time- executing_process->llego_cola;
          executing_process -> first_tag = 0;
        }
        printf("[t = %i] El proceso %s ha pasado a estado RUNNING\n", time, executing_process -> name);
      }

      if (executing_process -> bursts[executing_process -> bursts_id] == 0)
      {
        // Se le acabo burst
        if (executing_process -> bursts_id == executing_process -> qty_burst - 1)
        {
          executing_process -> status = FINISHED;
          // executing_process -> n_interrupt++;
          int turnaround_time = time - executing_process -> time_init;
          printf("[t = %i] El proceso %s ha pasado a estado FINISHED\n", time, executing_process -> name);
          printf("NUMERO DE VECES QUE FUE EJECUTADO: %i\n", executing_process -> n_chosen);
          printf("NUMERO DE VECES QUE FUE INTERRUMPIDO: %i\n", executing_process -> n_interrupt);
          printf("TURNAROUND TIME: %i\n", turnaround_time);
          printf("RESPONSE TIME: %i\n", executing_process -> response_time);
          printf("WAITING TIME: %i\n", executing_process -> waiting_time);
          free(executing_process->bursts);
          free(executing_process->waits);
          free(executing_process);
          executing_process = NULL;
          process_finished++;
          if (incoming -> len > 0)
          { 
            while (incoming -> len > 0)
            {
              Process* new_process = list_process_exchange(incoming);
              printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
              list_append(Cola, new_process);      
            }
          
          }
          list_destroy(incoming);
          continue;
        }
        executing_process -> bursts_id += 1;
        executing_process -> status = WAITING;
        executing_process -> waiting_time--;
        printf("[t = %i] El proceso %s ha pasado a estado WAITING\n", time, executing_process -> name);
        list_append(Cola, executing_process);
        executing_process = NULL;
        if (incoming -> len > 0)
        { 
          while (incoming -> len > 0)
          {
            Process* new_process = list_process_exchange(incoming);
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);      
          }
        
        }
        list_destroy(incoming);
        continue;
      }
        
    } else if (quantum == 0 && executing_process -> bursts[executing_process -> bursts_id] == 0) 
    {
      if (executing_process -> bursts_id == executing_process -> qty_burst - 1)
      {
        executing_process -> status = FINISHED;
        executing_process -> n_interrupt++;
        int turnaround_time = time - executing_process -> time_init;
        printf("[t = %i] El proceso %s ha pasado a estado FINISHED\n", time, executing_process -> name);
        printf("NUMERO DE VECES QUE FUE EJECUTADO: %i\n", executing_process -> n_chosen);
        printf("NUMERO DE VECES QUE FUE INTERRUMPIDO: %i\n", executing_process -> n_interrupt);
        printf("TURNAROUND TIME: %i\n", turnaround_time);
        printf("RESPONSE TIME: %i\n", executing_process -> response_time);
        printf("WAITING TIME: %i\n", executing_process -> waiting_time);
        // free(executing_process->bursts);
        // free(executing_process->waits);
        // free(executing_process);
        executing_process = NULL;
        process_finished++;
        if (incoming -> len > 0)
        { 
          while (incoming -> len > 0)
          {
            Process* new_process = list_process_exchange(incoming);
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);      
          }
        
        }
        list_destroy(incoming);
        continue;
      }
      executing_process -> bursts_id += 1;
      executing_process -> status = WAITING;
      executing_process -> n_interrupt++;
      executing_process -> waiting_time--;
      printf("[t = %i] El proceso %s ha pasado a estado WAITING\n", time, executing_process -> name);
      list_append(Cola, executing_process);
      executing_process = NULL;
      if (incoming -> len > 0)
      { 
        while (incoming -> len > 0)
        {
          Process* new_process = list_process_exchange(incoming);
          printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
          list_append(Cola, new_process);      
        }
      
      }
      list_destroy(incoming);
      continue;
    }

    if (incoming -> len > 0)
    { 
      while (incoming -> len > 0)
      {
        Process* new_process = list_process_exchange(incoming);
        printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
        list_append(Cola, new_process);      
      }
    
    }
    list_destroy(incoming);

    wait_sum(Cola);
    int id = executing_process -> bursts_id;
    executing_process -> bursts[id] -= 1;
    quantum--;
    time++;

  }
  list_destroy(Procesos);
  list_destroy(Cola);
  input_file_destroy(file);
}
