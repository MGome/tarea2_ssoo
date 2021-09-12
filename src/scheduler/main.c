#include <stdio.h>
#include <stdlib.h>

#include "../file_manager/manager.h"
#include "../process/process.h"
#include "../queue/queue.h"

// Se declaran las variables globales que almacenan
// la cantidad de procesos por fabrica
int procesos_fabrica1;
int procesos_fabrica2;
int procesos_fabrica3;
int procesos_fabrica4;

// Flujo principal
int main(int argc, char **argv)
{
  printf("Hello T2!\n");

  //printf(argv[0]);
  //printf(argv[1]);
  //printf(argv[3]);

  InputFile *file = read_file("input.txt");

  printf("Reading file of length %i:\n", file->len);
  for (int i = 0; i < file->len; i++)
  {
    char **line = file->lines[i];
    printf(
        "\tProcess %s from factory %s has init time of %s and %s bursts.\n",
        line[0], line[2], line[1], line[3]);
  };

  Process* proceso = process_init();

}
