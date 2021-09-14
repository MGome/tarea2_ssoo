#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
      list_append(new_processes, new_process);
      current = Procesos -> head;
    } else {
      break;
    }
  }

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
  while (Procesos -> len > 0) //&& Cola -> len > 0)
  {
    printf("TIME: %i \n", time);
    Queue* incoming = incoming_process(Procesos, time);
    list_print(incoming);

    if (Cola -> len == 0)
    {
      // Cuando no hay procesos en la Cola, todos los procesos entrantes ingresan al final de la cola.
      // TODO: desempate entre procesos.
      if (incoming -> len > 0)
      {
        while (incoming -> len > 0)
        {
        Process* new_process = list_process_exchange(incoming);
        printf("[t = %i] El proceso %s ha sido creado.", time, new_process-> name);
        list_append(Cola, new_process);
        }
      } else {
        printf("[t = %i] No hay ningún proceso ejecutando en la CPU.", time);
      }

      
    }

    // Proceso se ejecuta

    if (!executing_process) // No hay proceso en ejecución
    {
      int quantum = calculate_quantum(Cola);  // se calcula su quantum
      Process* executing_process = list_process_exchange(incoming); // se extrae el proceso en la cabeza
    }
    
    

    quantum--;
    time++;
  }

}
