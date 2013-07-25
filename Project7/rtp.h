#include <pthread.h>
#include "queue.h"

#ifndef _RTP_H
#define _RTP_H

typedef struct _RTP_CONNECTION{
  int net_connection_handle;
  int alive;

  pthread_t recv_thread;
  pthread_t send_thread;

  /* FIX ME */
  pthread_cond_t ack_cond;
  pthread_mutex_t ack_mutex;

  pthread_cond_t recv_cond;
  pthread_mutex_t recv_mutex;
  QUEUE recv_queue;

  pthread_cond_t send_cond;
  pthread_mutex_t send_mutex;
  QUEUE send_queue;

} RTP_CONNECTION;

RTP_CONNECTION *rtp_connect(char *host, int port);
int rtp_disconnect(RTP_CONNECTION *connection);
int rtp_recv_message(RTP_CONNECTION *connection, char **buffer, int *length);
int rtp_send_message(RTP_CONNECTION *connection, char *buffer, int length);

#endif
