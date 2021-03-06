#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

void queue_init(QUEUE *queue){
  queue->size = 0;
  queue->elements = NULL;
}

void queue_add(QUEUE *queue, void *element){
  void **temp;
  queue->size++;
  temp = realloc((void*)(queue->elements),sizeof(void*) * queue->size);
  if (temp == NULL){
    fprintf(stderr,"Out of memory\n");
    exit (EXIT_FAILURE);
  }
  queue->elements = temp;
  queue->elements[queue->size-1] = element;
}

void *queue_extract(QUEUE *queue){
  int i;
  void *element;

  if (queue->elements == NULL)
    return NULL;

  element = queue->elements[0];
  if (queue->size == 0)
    return NULL;
  for (i=0; i<queue->size-1; i++){
    queue->elements[i] = queue->elements[i+1];
  }
  queue->size--;
  return element;
}

int queue_size(QUEUE *queue){
  return queue->size;
}
