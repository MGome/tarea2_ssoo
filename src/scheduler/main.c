#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "./../file_manager/manager.h"
#include "process.h"
#include "queue.h"

// Se declaran las variables globales que almacenan
// la cantidad de procesos por fabrica
int procesos_fabrica1;
int procesos_fabrica2;
int procesos_fabrica3;
int procesos_fabrica4;
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
  // TODO: Función Piso
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

        if (current -> waits[id] == -1)
        {
          current -> status = READY;
          current -> waits_id += 1;

          printf("[t = %i] El proceso %s ha pasado a estado READY\n", time, current -> name);
        }
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

    switch (Proceso -> fabric)
    {
      case 1:
        procesos_fabrica1++;
        break;
      case 2:
        procesos_fabrica2++;
        break;
      case 3:
        procesos_fabrica3++;
        break;
      case 4:
        procesos_fabrica4++;
        break;
    }

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
  while (qty_process != process_finished)
  {
    // printf("Time: %i\n", time);
    // printf("TIME: %i \n", time);
    Queue* incoming = incoming_process(Procesos, time);

    // Cuando no hay procesos en la Cola, todos los procesos entrantes ingresan al final de la cola.
    // TODO: desempate entre procesos.
    if (Cola -> len == 0)
    {
      if (incoming -> len > 0)
      { 
        while (incoming -> len > 0)
        {
          Process* new_process = list_process_exchange(incoming);
          if (incoming -> len >= 1)
          {
            Process* new_process2 = list_process_exchange(incoming);
            if (new_process -> fabric == new_process2 -> fabric)
            {
              int result = strcmp(new_process -> name, new_process2 -> name);
              if (result <= 0)
              {
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
                list_append(Cola, new_process);
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process2-> name);
                list_append(Cola, new_process2);
              } else if (result > 0)
              {
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process2-> name);
                list_append(Cola, new_process2);
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
                list_append(Cola, new_process);
              }
            }
          } else 
          {
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);
          }      
        }
      } else if (Cola -> len == 0 && !executing_process) {
        printf("[t = %i] No hay ningún proceso ejecutando en la CPU.\n", time);
        time++;
        continue;
      }
    }

    // Proceso se ejecuta

    if (!executing_process) // No hay proceso en ejecución
    { 
      // quantum = calculate_quantum(Cola);  // se calcula su quantum
      quantum = 4;
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

            if (current -> waits[id] == -1)
            {
              current -> status = READY;
              current -> waits_id += 1;
              printf("[t = %i] El proceso %s ha pasado a estado READY\n", time, current -> name);
              time--;
            }
          }
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
        printf("[t = %i] El proceso %s ha pasado a estado READY\n", time, executing_process -> name);
        list_append(Cola, executing_process);
        executing_process = NULL;
        continue;

      } 
    } else if (quantum != 0)
    {
    
      if (executing_process -> status == READY)
      {
        executing_process -> status = RUNNING;
        printf("[t = %i] El proceso %s ha pasado a estado RUNNING\n", time, executing_process -> name);
      }

      if (executing_process -> bursts[executing_process -> bursts_id] == 0)
      {
        // Se le acabo burst
        if (executing_process -> bursts_id == executing_process -> qty_burst - 1)
        {
          executing_process -> status = FINISHED;
          printf("[t = %i] El proceso %s ha pasado a estado FINISHED\n", time, executing_process -> name);
          executing_process = NULL;
          process_finished++;
          continue;
        }
        executing_process -> bursts_id += 1;
        executing_process -> status = WAITING;
        printf("[t = %i] El proceso %s ha pasado a estado WAITING\n", time, executing_process -> name);
        list_append(Cola, executing_process);
        executing_process = NULL;
        continue;
      }
        
    } else if (quantum == 0 && executing_process -> bursts[executing_process -> bursts_id] == 0) 
    {
      if (executing_process -> bursts_id == executing_process -> qty_burst - 1)
      {
        executing_process -> status = FINISHED;
        printf("[t = %i] El proceso %s ha pasado a estado FINISHED\n", time, executing_process -> name);
        executing_process = NULL;
        process_finished++;
        continue;
      }
      executing_process -> bursts_id += 1;
      executing_process -> status = WAITING;
      printf("[t = %i] El proceso %s ha pasado a estado WAITING\n", time, executing_process -> name);
      list_append(Cola, executing_process);
      executing_process = NULL;
      continue;
    }

    wait_sum(Cola);
    int id = executing_process -> bursts_id;
    executing_process -> bursts[id] -= 1;
    quantum--;

    if (incoming -> len > 0)
      { 
        while (incoming -> len > 0)
        {
          Process* new_process = list_process_exchange(incoming);
          if (incoming -> len >= 1)
          {
            Process* new_process2 = list_process_exchange(incoming);
            if (new_process -> fabric == new_process2 -> fabric)
            {
              int result = strcmp(new_process -> name, new_process2 -> name);
              if (result <= 0)
              {
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
                list_append(Cola, new_process);
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process2-> name);
                list_append(Cola, new_process2);
              } else if (result > 0)
              {
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process2-> name);
                list_append(Cola, new_process2);
                printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
                list_append(Cola, new_process);
              }
            }
          } else 
          {
            printf("[t = %i] El proceso %s ha sido creado.\n", time, new_process-> name);
            list_append(Cola, new_process);
          }      
        }
      } else if (Cola -> len == 0 && !executing_process) {
        printf("[t = %i] No hay ningún proceso ejecutando en la CPU.\n", time);
        time++;
        continue;
      }

    time++;
  }

}
