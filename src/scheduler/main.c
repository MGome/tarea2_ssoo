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

  Queue* Cola = queue_init();

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    // printf(
    //     "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
    //     line[0], line[2], line[1], line[3]);
    Process* Proceso = process_init(process_id, line[0], atoi(line[2]), atoi(line[3]) , atoi(line[1]));
    //printf("%i\n", Proceso->time_init);
    process_id++;

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
    int* bursts = calloc(cantidad_burst, sizeof(int));
    int* io_burst = calloc(cantidad_burst-1, sizeof(int));
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
        bursts[indice_burst] = atoi(line[indice_recorrido]);
        indice_burst++;
      } else
      {
        // Es un io burst
        io_burst[indice_io] = atoi(line[indice_recorrido]);
        indice_io++;
      }
    }
    Proceso -> bursts = bursts;
    Proceso -> waits = io_burst;

    // for(int i=0; i < cantidad_burst; i++){
    //   printf("%i\n", Proceso->bursts[i]);
    // }
    // printf("--------------\n");

    list_sort(Cola, Proceso);
  };
  
  list_print(Cola);

  for (int i =0; i < Cola->head->qty_burst; i++)
  {
    printf("BURSTS = %i \n", Cola->head->bursts[i]);
  }
  for (int i =0; i < Cola->head->qty_burst-1; i++)
  {
    printf("WAIT = %i \n", Cola->head->waits[i]);
  }
  // printf("Fabrica 1: %i\n", procesos_fabrica1);
  // printf("Fabrica 3: %i\n", procesos_fabrica3);

}
