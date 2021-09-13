#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "process.h"

Queue* queue_init()
{
    Queue* cola = malloc(sizeof(Queue));
    int* procesosxfabrica = calloc(4, sizeof(int)); //pueden haber hasta 4 fabricas activas
    cola -> head = NULL;
    cola -> tail = NULL;
    cola -> len = 0;
    cola -> procesosXfabrica = procesosxfabrica;

    return cola;
}

void list_append(Queue* list, Process* process)
{
  if(list -> head == NULL)
  {
    list -> head = process;
  }
  else
  {
    list -> tail -> next = process;
  }
  list -> tail = process;
  list -> len += 1;
  list -> procesosXfabrica[process -> fabric - 1] += 1;
}

Process* list_pop_comeback(Queue* list)
{
    Process* head = list -> head;
    //list -> head = head -> next;
    //list -> tail -> next = head;
    list -> tail = head;

    return head;
} 

int list_finish_pop(Queue* list)
{
  Process* head = list -> head;
  int pid = head -> pid;
  list -> head = list -> head -> next;
  list -> len -= 1;
  free(head);

  return pid;
}

void list_print(Queue* list)
{
  if(list -> head == NULL)
  {
      return;
  }
  for(Process* current = list -> head; current; current = current -> next)
  {
    printf("%i\n", current -> time_init);
  }
}

void list_destroy(Queue* list)
{
  if(list -> head)
  {
    Process* curr = list -> head -> next;
    Process* prev = list -> head;

    while(curr)
    {
      free(prev);
      prev = curr;
      curr = curr -> next;
    }
    free(prev);
  }
  free(list);
}

void list_sort(Queue* list, Process* process)
{
    Process* previous = NULL;
    if (list -> head != NULL)
    {
        for (Process* current = list -> head; current; current = current -> next)
        {

            if (previous == NULL ){
                if (process -> time_init <= current->time_init)
                {
                    process -> next = current;
                    list -> head = process;
                    break;
                } else if (process -> time_init > current->time_init && list -> len == 1) {
                    current -> next = process;
                    list -> tail = process;
                    break;
                }
            }

            if (process -> time_init <= current -> time_init)
            {
                previous -> next = process;
                process -> next = current;
                break;

            } else if (current -> next == NULL)
            {
              if (process -> time_init < current -> time_init)
              {
                  previous -> next = process;
                  process -> next = current;
                  list -> tail = current;
                  break;
              }  else {
                  current -> next = process;
                  list -> tail = process;
                  break;
              }
            }

            previous = current;
        }
    } else {
        list -> head = process;
        list -> tail = process;
    }
    list -> len += 1;
}
