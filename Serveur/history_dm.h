#ifndef HISTORY_DM_H
#define HISTORY_DM_H
#define MAX_CLIENTS     100
#define BUF_SIZE    1024

typedef struct
{
    char name1[BUF_SIZE];
    char name2[BUF_SIZE];
    char message_1_to_2[BUF_SIZE];
    char message_2_to_1[BUF_SIZE];
    char message[BUF_SIZE];
    
}History_DM;

#endif /* guard */