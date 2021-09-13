#include <stdio.h>
#include <stdlib.h>

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

    //list_sort(Cola, Proceso);
    list_append(Procesos, Proceso);
  };

  Queue* Cola = queue_init();

  while (Procesos -> len > 0)
  {
    //Procesos nuevos
    int qty_new_process = 0;
    Queue* new_processes = queue_init();
    Process* current = Procesos -> head;
    while (current)
    {
      Process* new_process = NULL;
      if (current -> time_init == time)
      {
        list_append(new_processes, current);
        list_finish_pop(Procesos);
      } else {
        break;
      }
    }
    


    for (Process* current = Procesos -> head; current; current = current -> next)
    {

    }



    time++;
  }

}
