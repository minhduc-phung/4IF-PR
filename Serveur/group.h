#ifndef GROUP_H
#define GROUP_H

#include "server2.h"
#include "client2.h"

typedef struct
{
    char* name;
    Client members[MAX_CLIENTS];
    char message[BUF_SIZE];
}Group;

#endif /* guard */
