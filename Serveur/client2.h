#ifndef CLIENT_H
#define CLIENT_H

#include "server2.h"

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   SOCKET sock_current_chat_client;
}Client;

#endif /* guard */
