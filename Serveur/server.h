#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF        "\r\n"
#define PORT         1977
#define MAX_CLIENTS     100
#define MAX_GROUPS     20
#define MAX_NB_DMS     50     

#define BUF_SIZE    1024

#include "client.h"
#include "group.h"
#include "history_dm.h"

static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static int remove_sub_and_keep(char string[], const char substr[], const char* keepstr);
static void send_message_to_sender(Client c, const char *buffer);
static void send_message_to_receiver(Client c, const char *buffer);
static void send_message_to_client(Client sender, Client receiver, const char *buffer, History_DM* history_dm, int* actual_nb_dm, const int current_dest_sender, const int current_dest_receiver);
static void send_message_to_all_clients(Client *clients, Client client, int actual, const char *buffer, char* offline_buffer_all, char from_server);
static int find_dm_in_history (const History_DM* history_dm, const char* client_name1, const char* client_name2, int actual_nb_dm);
static void save_history_all(const char history_all[]);
static void save_history_dm(const History_DM history_dm[], const int actual_nb_dm); 
static void restore_history(char history_all[]);
static void restore_history_dm(History_DM history_dm[], int* actual_nb_dm);
static void remove_client(Client *clients, int to_remove, int *actual);
static void remove_group(Group *groups, int to_remove, int *actual);
static void remove_member(Group *groups, int to_remove, int group_index);
static void clear_clients(Client *clients, int actual);

#endif /* guard */
