#ifndef GROUP_H
#define GROUP_H
#define MAX_CLIENTS     100
#include "client2.h"


typedef struct
{
    char* name;
    Client members[MAX_CLIENTS];
    char message[BUF_SIZE];
}Group;

#endif /* guard */
