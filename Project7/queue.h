#include <stdlib.h>

#ifndef _QUEUE_H
#define _QUEUE_H

typedef struct _QUEUE{
  int size;
  void **elements;
} QUEUE;

void queue_init(QUEUE *queue);
void queue_add(QUEUE *queue, void *element);
void *queue_extract(QUEUE *queue);
int queue_size(QUEUE *queue);

#endif
