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
    list_sort(Cola, Proceso);
    // TO DO: SUMA INDICE FABRICA
  };
  //printf("%i", Cola -> head -> time_init);
  list_print(Cola);

  // int bursts [3] = {2, 3, 2};
  // proceso -> bursts = bursts;

  // for (int i=0; i<3; i++) 
  // {
  //   printf("%i", proceso->bursts[i]);
  // };

  // printf("%s", proceso->name);

}
