#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "group.h"
#include "server2.h"
#include "client2.h"
#include "command_handler.h"

static void init(void)
{
#ifdef WIN32
   WSADATA wsa;
   int err = WSAStartup(MAKEWORD(2, 2), &wsa);
   if(err < 0)
   {
      puts("WSAStartup failed !");
      exit(EXIT_FAILURE);
   }
#endif
}

static void end(void)
{
#ifdef WIN32
   WSACleanup();
#endif
}

static void app(void)
{
   char offline_buffer_all[BUF_SIZE];
   SOCKET sock = init_connection();
   char buffer[BUF_SIZE];
   /* the index for the array */
   int actual = 0;
   int actual_group = 0;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   Group groups[MAX_GROUPS];

   fd_set rdfs;

   while(1)
   {
      int i = 0;
      FD_ZERO(&rdfs);

      /* add STDIN_FILENO */
      FD_SET(STDIN_FILENO, &rdfs);

      /* add the connection socket */
      FD_SET(sock, &rdfs);

      /* add socket of each client */
      for(i = 0; i < actual; i++)
      {
         FD_SET(clients[i].sock, &rdfs);
      }

      if(select(max + 1, &rdfs, NULL, NULL, NULL) == -1)
      {
         perror("select()");
         exit(errno);
      }

      /* something from standard input : i.e keyboard */
      if(FD_ISSET(STDIN_FILENO, &rdfs))
      {
         /*stop process if the user types q*/
         // fgets(buffer, BUF_SIZE - 1, stdin);
         // if(strcmp(buffer, "q\n") == 0)
         // {
         //    break;
         // }
         break;
      }
      else if(FD_ISSET(sock, &rdfs))
      {
         /* new client */
         SOCKADDR_IN csin = { 0 };
         size_t sinsize = sizeof csin;
         int csock = accept(sock, (SOCKADDR *)&csin, &sinsize);
         if(csock == SOCKET_ERROR)
         {
            perror("accept()");
            continue;
         }

         /* after connecting the client sends its name */
         if(read_client(csock, buffer) == -1)
         {
            /* disconnected */
            continue;
         }

         /* what is the new maximum fd ? */
         max = csock > max ? csock : max;

         FD_SET(csock, &rdfs);

         Client c = { csock };
         strncpy(c.name, buffer, BUF_SIZE - 1);
         clients[actual] = c;
         actual++;
         write_client(csock, offline_buffer_all);
      }
      else
      {
         int i = 0;
         for(i = 0; i < actual; i++)
         {
            /* a client is talking */
            if(FD_ISSET(clients[i].sock, &rdfs))
            {

               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);

               char** separated_buffer = malloc(sizeof(char*)*3);

               int action = handle_command(buffer, separated_buffer);

               switch(action) {
                  case 1:
                     int j = 0;
                     for(j = 0; j < actual; j++)
                     {

                        if (strcmp(clients[j].name, name_to_chat) == 0){
                           printf("name_to_chat: %s\n", name_to_chat);
                           printf("message: %s\n", message);
                           if (strlen(message) >0) {
                              send_message_to_client(client, clients[j], message);
                           }else{
                              write_client(client.sock, "Please include a message 3 \n");
                           }
                           
                           break;
                        }
                     }
                     if (j == actual){
                        char message_not_found[BUF_SIZE] = {0};
                        sprintf(message_not_found, "Client %s not found \n", name_to_chat);
                        write_client(client.sock, message_not_found);
                     }

                     break;
               }
               char* command = malloc(sizeof(char)*BUF_SIZE);
               command = get_param(buffer);
               
               if ((strcmp(buffer, "msgto") != 0) && (strcmp(buffer, "msgto ") != 0) && (strcmp(command, "msgto") == 0)){
                  char* name_and_message_buffer = buffer + strlen(command) + 1;

                  char* name_to_chat = malloc(sizeof(char)*BUF_SIZE);
                  name_to_chat = get_param(name_and_message_buffer);

                  char* message = name_and_message_buffer+ strlen(name_to_chat)+1;

                  if (strcmp(name_to_chat, name_and_message_buffer) == 0) {
                     write_client(client.sock, "Please include a message 1 \n");
                     break;
                  }
                  //else{
                    // message = name_and_message_buffer+ strlen(name_to_chat)+1;
                  //}

                  
               }else if(c == 0)
               
               /* client disconnected */
               
               {
                  printf("discon \n");
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
               }
               else  
               {
                  strncat(offline_buffer_all, client.name, BUF_SIZE - strlen(client.name) - 1);
                  strncat(offline_buffer_all, " : ", BUF_SIZE - strlen(offline_buffer_all) - 1);
                  strncat(offline_buffer_all, buffer, BUF_SIZE - strlen(offline_buffer_all) - 1);
                  strncat(offline_buffer_all, CRLF, BUF_SIZE - strlen(offline_buffer_all) - 1);
                  printf("%s \n", buffer);
                  printf("from %s\n", client.name);
                  printf("chat all \n");
                  send_message_to_all_clients(clients, client, actual, buffer, 0);
               }
               break;
            }
         }
      }
   }

   clear_clients(clients, actual);
   end_connection(sock);
}

static void clear_clients(Client *clients, int actual)
{
   int i = 0;
   for(i = 0; i < actual; i++)
   {
      closesocket(clients[i].sock);
   }
}

static void remove_client(Client *clients, int to_remove, int *actual)
{
   /* we remove the client in the array */
   memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
   /* number client - 1 */
   (*actual)--;
}

static void send_message_to_all_clients(Client *clients, Client sender, int actual, const char *buffer, char from_server)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual; i++)
   {
      /* we don't send message to the sender */
      if(sender.sock != clients[i].sock)
      {
         if(from_server == 0)
         {
            strncpy(message, sender.name, BUF_SIZE - 1);
            strncat(message, " : ", sizeof message - strlen(message) - 1);
            strncat(message, buffer, sizeof message - strlen(message) - 1);
         }else{
            strncpy(message, buffer, sizeof message - strlen(message) - 1);
         }
         write_client(clients[i].sock, message);
      }
   }
}

static void send_message_to_client(Client sender, Client receiver, const char *buffer)
{
   char message[BUF_SIZE];
   message[0] = 0;
   strncpy(message, sender.name, BUF_SIZE - 1);
   strncat(message, " : ", sizeof message - strlen(message) - 1);
   strncat(message, buffer, sizeof message - strlen(message) - 1);
   write_client(receiver.sock, message);
}

static char* get_param(char* buffer){
   // Get the first word before the space and do not take it out of the buffer
   char* param = malloc(sizeof(char)*BUF_SIZE);
   int i = 0;
   while(buffer[i] != ' ' && buffer[i] != '\0'){
      param[i] = buffer[i];
      i++;
   }
   param[i] = '\0';
   return param;
}

static int init_connection(void)
{
   SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
   SOCKADDR_IN sin = { 0 };

   if(sock == INVALID_SOCKET)
   {
      perror("socket()");
      exit(errno);
   }

   sin.sin_addr.s_addr = htonl(INADDR_ANY);
   sin.sin_port = htons(PORT);
   sin.sin_family = AF_INET;

   if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
   {
      perror("bind()");
      exit(errno);
   }

   if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
   {
      perror("listen()");
      exit(errno);
   }

   return sock;
}

static void end_connection(int sock)
{
   closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
   int n = 0;

   if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
   {
      perror("recv()");
      /* if recv error we disonnect the client */
      n = 0;
   }

   buffer[n] = 0;

   return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
   if(send(sock, buffer, strlen(buffer), 0) < 0)
   {
      perror("send()");
      exit(errno);
   }
}

int main(int argc, char **argv)
{
   init();

   app();

   end();

   return EXIT_SUCCESS;
}
