#include <sys/socket.h>

#ifndef _NETWORK_H
#define _NETWORK_H

#define MAX_PAYLOAD_LENGTH 10

enum {DATA=0, LAST_DATA, ACK, NACK, TERM};

typedef struct _PACKET{
  int type;
  int checksum;
  int payload_length;
  char payload[MAX_PAYLOAD_LENGTH];
} PACKET;


int net_connect(char *host, int port);
int net_disconnect(int connection);
int net_send_packet(int connection, PACKET *packet);
int net_recv_packet(int connection, PACKET *packet);

#endif
