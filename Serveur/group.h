#ifndef GROUP_H
#define GROUP_H
#define MAX_CLIENTS     100
#define BUF_SIZE    1024


typedef struct
{
    char* name;
    int nbMembers;
    char members_name[MAX_CLIENTS][BUF_SIZE];
    char message[BUF_SIZE];
    
}Group;

#endif /* guard */
