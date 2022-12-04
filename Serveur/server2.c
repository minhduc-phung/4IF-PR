#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#include "server2.h"
#include "group.h"
#include "client2.h"
#include "command_handler.h"
#include "history_dm.h"


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
   /* the number of groups */
   int actual_group = 0;
   int actual_nb_dm;
   int max = sock;
   /* an array for all clients */
   Client clients[MAX_CLIENTS];
   Group groups[MAX_GROUPS];
   //History_DM history_dm[];

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
               //printf("found client \n");
               Client client = clients[i];
               int c = read_client(clients[i].sock, buffer);

               if(c == 0)
               
               /* client disconnected */               
               {
                  printf("%s disconnected from the server. \n", clients[i].name);
                  closesocket(clients[i].sock);
                  remove_client(clients, i, &actual);
                  strncpy(buffer, client.name, BUF_SIZE - 1);
                  strncat(buffer, " disconnected !", BUF_SIZE - strlen(buffer) - 1);
                  send_message_to_all_clients(clients, client, actual, buffer, 1);
                  break;
               }
               

               char** separated_buffer = malloc(sizeof(char*)*3);
               

               int action = handle_command(buffer, separated_buffer);
               //print the separated buffer elements
               int j = 0;
               for(j = 0; j < 3; j++)
               {
                  printf("separated_buffer[%d] = %s \n", j, separated_buffer[j]);
               }
               if (action != 0){
                  printf("%s executed action : %d \n", action);
               } else {
                  printf("%s chat globally \n", client.name);
               }
            
               switch(action) {
                  // Case 1: the client wants to send a message to another client (Direct message)
                  case 1:
                     //separated_buffer[0]: command ; separated_buffer[1]: name_to_chat ; separated_buffer[2]: message
                     
                     printf("direct message \n");
                     int j = 0;
                     for(j = 0; j < actual; j++)
                     {

                        if (strcmp(clients[j].name, separated_buffer[1]) == 0){
                           printf("name_to_chat: %s\n", separated_buffer[1]);
                           printf("message: %s\n", separated_buffer[2]);
                           if (strlen(separated_buffer[2]) >0) {
                              send_message_to_client(client, clients[j], separated_buffer[2]);
                           }else{
                              write_client(client.sock, "Please include a message 3 \n");
                           }
                           
                           break;
                        }
                     }
                     if (j == actual){
                        char message_not_found[BUF_SIZE] = {0};
                        sprintf(message_not_found, "Client %s not found \n", separated_buffer[1]);
                        write_client(client.sock, message_not_found);
                     }
                     break;
                  // Case 2: the client wants to create a group chat
                  case 2: 
                     //separated_buffer[0]: command ; separated_buffer[1]: group_name ; separated_buffer[2]: group_members
                     printf("create group \n");

                     // print out the separated buffer elements
                     //printf("separator_buffer[0] = %s \n", separated_buffer[0]);
                     //printf("separator_buffer[1] = %s \n", separated_buffer[1]);
                     //printf("separator_buffer[2] = %s \n", separated_buffer[2]);

                     // Check if the group name already exists
                     int l = 0;
                     for(l = 0; l < actual_group; l++)
                     {
                        if (strcmp(groups[l].name, separated_buffer[1]) == 0){
                           write_client(client.sock, "Group name already exists \n");
                           break;
                        }
                     }
                     // Break from the case if the group name already exists
                     if (l != actual_group){
                        break;
                     }

                     //printf("slicing strings to get member names \n");
                     // Put the members name in an array
                     char** separated_members = malloc(sizeof(char*)*MAX_CLIENTS);
                     int k = 0;
                     char* token = strtok(separated_buffer[2], " ");
                     while (token != NULL) {
                        separated_members[k] = token;
                        k++;
                        token = strtok(NULL, " ");
                     }
                     //printf("putting the sender in the group \n");
                     // Put the sender of the command in the array
                     separated_members[k] = client.name;
                     k++;
                     //printf("checking dups \n");
                     // Check the array to see if there's any duplicate and remove them from the array
                     int m = 0;
                     int n = 0;
                     for(m = 0; m < k; m++){
                        for(n = m+1; n < k; n++){
                           if (strcmp(separated_members[m], separated_members[n]) == 0){
                              separated_members[n] = "";
                           }
                        }
                     }
                     //printf("adding members to group \n");
                     // Add all the members to the group
                     Group g = { separated_buffer[1], 0 };
                     for(m = 0; m < k; m++){
                        if (strlen(separated_members[m]) > 0){
                           strncpy(g.members_name[m], separated_members[m], BUF_SIZE - 1);
                           g.nbMembers++;
                        }
                     }
                     groups[actual_group] = g;
                     actual_group++;
                     //printf("group created \n");
                     //printf("sending notis to members \n");
                     // Send a message to all the members of the group
                     char message_group_created[BUF_SIZE] = {0};
                     sprintf(message_group_created, "User %s has created a group named '%s' with you as a member. \n", client.name, separated_buffer[1]);
                     for(m = 0; m < k; m++){
                        if (strlen(g.members_name[m]) > 0){
                           for(n = 0; n < actual; n++){
                              if (strcmp(clients[n].name, g.members_name[m]) == 0){
                                 // Send to the group creator a different message than the other members
                                 if (strcmp(clients[n].name, client.name) != 0){
                                    write_client(clients[n].sock, message_group_created);
                                 } else {
                                    write_client(clients[n].sock, "Group created. \n");
                                 }
                              }
                           }
                        }
                     }
                     break;
                 // Case 3: the client sends a message to a group
                 case 3:
                     //separated_buffer[0]: command ; separated_buffer[1]: group_name ; separated_buffer[2]: message
                     printf("send message to group \n");
                     // Check if the group exists
                     int o = 0;
                     for(o = 0; o < actual_group; o++)
                     {
                        if (strcmp(groups[o].name, separated_buffer[1]) == 0){
                           // Check if the sender is a member of the group
                           int p = 0;
                           for(p = 0; p < groups[o].nbMembers; p++){
                              if (strcmp(groups[o].members_name[p], client.name) == 0){
                                 // Send the message to all the members of the group
                                 int q = 0;
                                 for(q = 0; q < groups[o].nbMembers; q++){
                                    if (strlen(groups[o].members_name[q]) > 0){
                                       for(n = 0; n < actual; n++){
                                          if (strcmp(clients[n].name, groups[o].members_name[q]) == 0){
                                             // Compose the message to send to the group members
                                             char message_group[BUF_SIZE] = {0};
                                             sprintf(message_group, "[%s] %s: %s \n",  groups[o].name, client.name, separated_buffer[2]);
                                             write_client(clients[n].sock, message_group);
                                          }
                                       }
                                    }
                                 }
                                 break;
                              }
                           }
                           if (p == groups[o].nbMembers){
                              write_client(client.sock, "You are not a member of this group \n");
                           }
                           break;
                        }
                     }
                     if (o == actual_group){
                        write_client(client.sock, "Group not found \n");
                     }
                     break;
                  // Case 4: the client wants to leave a group
                  case 4:
                     //separated_buffer[0]: command ; separated_buffer[1]: group_name
                     printf("leave group \n");
                     // Check if the group exists
                     int r = 0;
                     // Variable to check if the client is removed from the group
                     int removed = 0;
                     for(r = 0; r < actual_group; r++)
                     {
                        if (strcmp(groups[r].name, separated_buffer[1]) == 0){
                           // Check if the sender is a member of the group
                           int s = 0;
                           for(s = 0; s < groups[r].nbMembers; s++){
                              if (strcmp(groups[r].members_name[s], client.name) == 0){
                                 // Remove the sender from the group
                                 // Print out the members of the group before the removal
                                 int t = 0;
                                 for(t = 0; t < groups[r].nbMembers; t++){
                                    printf("member %d: %s \n", t, groups[r].members_name[t]);
                                 }
                                 remove_member(groups, s, r);
                                 // Print out the members of the group after the removal
                                 for(t = 0; t < groups[r].nbMembers; t++){
                                    printf("member %d: %s \n", t, groups[r].members_name[t]);
                                 }
                                 // Send a message to all the members of the group
                                 for(t = 0; t < groups[r].nbMembers; t++){
                                    if (strlen(groups[r].members_name[t]) > 0){
                                       for(n = 0; n < actual; n++){
                                          if (strcmp(clients[n].name, groups[r].members_name[t]) == 0){
                                             // Compose the message to send to the group members
                                             char message_group[BUF_SIZE] = {0};
                                             sprintf(message_group, "User %s has left the group '%s'. \n", client.name, groups[r].name);
                                             write_client(clients[n].sock, message_group);
                                             removed = 1;
                                          }
                                       }
                                    }
                                 }
                                 break;
                              }
                           }
                           // The 'removed' variable is used so that if the last client were to leave the group,
                           // the message "You are not a member of this group" is not sent due to the
                           // decrementation of nbMembers
                           if ((s == groups[r].nbMembers) && (removed == 0)){
                              write_client(client.sock, "You are not a member of this group \n");
                           }
                           break;
                        }
                     }
                     if (r == actual_group){
                        write_client(client.sock, "Group not found \n");
                     }
                     break;
                  // Case 5: the client wants to list all the groups
                  case 5:
                     printf("list groups \n");
                     // Send a message to the client with the list of all the groups
                     char message_list_groups[BUF_SIZE] = {0};
                     sprintf(message_list_groups, "List of all the groups: \n");
                     for(r = 0; r < actual_group; r++){
                        sprintf(message_list_groups, "%s- %s \n", message_list_groups, groups[r].name);
                     }
                     write_client(client.sock, message_list_groups);
                     break;
                  // Case 6: the client wants to remove a group
                  // Only the group creator can remove the group
                  // By default, the creator is the last member of the group
                  case 6:
                     //separated_buffer[0]: command ; separated_buffer[1]: group_name
                     printf("remove group \n");
                     int removed = 0;
                     // Check if the group exists
                     int u = 0;
                     for(u = 0; u < actual_group; u++)
                     {
                        if (strcmp(groups[u].name, separated_buffer[1]) == 0){
                           // Check if the sender is the creator of the group
                           if (strcmp(groups[u].members_name[groups[u].nbMembers - 1], client.name) == 0){
                              // Remove the group
                              remove_group(groups, u, &actual_group);
                              // Send a message to all the members of the group
                              for(r = 0; r < groups[u].nbMembers; r++){
                                 if (strlen(groups[u].members_name[r]) > 0){
                                    for(n = 0; n < actual; n++){
                                       if (strcmp(clients[n].name, groups[u].members_name[r]) == 0){
                                          // Compose the message to send to the group members
                                          char message_group[BUF_SIZE] = {0};
                                          sprintf(message_group, "Group '%s' has been removed. \n", separated_buffer[1]);
                                          write_client(clients[n].sock, message_group);
                                          removed = 1;
                                       }
                                    }
                                 }
                              }
                              break;
                           } else {
                              write_client(client.sock, "You are not the creator of this group \n");
                              break;
                           }
                        }
                     }
                     if ((u == actual_group) && (removed == 0)){
                        write_client(client.sock, "Group not found \n");
                     }
                     break;
                  // Case 7: the client wants to list all the members of a group
                  case 7:
                     //separated_buffer[0]: command ; separated_buffer[1]: group_name
                     printf("list members \n");
                     // Check if the group exists
                     int v = 0;
                     for(v = 0; v < actual_group; v++)
                     {
                        if (strcmp(groups[v].name, separated_buffer[1]) == 0){
                           // Send a message to the client with the list of all the members of the group
                           char message_list_members[BUF_SIZE] = {0};
                           sprintf(message_list_members, "List of all the members of the group '%s': \n", groups[v].name);
                           for(r = 0; r < groups[v].nbMembers; r++){
                              sprintf(message_list_members, "%s- %s \n", message_list_members, groups[v].members_name[r]);
                           }
                           write_client(client.sock, message_list_members);
                           break;
                        }
                     }
                     if (v == actual_group){
                        write_client(client.sock, "Group not found \n");
                     }
                     break;
                     
                 // Default: the client wants to send a message to all clients (Broadcast message)
                  default:
                     strncat(offline_buffer_all, client.name, BUF_SIZE - strlen(client.name) - 1);
                     strncat(offline_buffer_all, " : ", BUF_SIZE - strlen(offline_buffer_all) - 1);
                     strncat(offline_buffer_all, buffer, BUF_SIZE - strlen(offline_buffer_all) - 1);
                     strncat(offline_buffer_all, CRLF, BUF_SIZE - strlen(offline_buffer_all) - 1);
                     printf("%s \n", buffer);
                     printf("from %s\n", client.name);
                     printf("chat all \n");
                     send_message_to_all_clients(clients, client, actual, buffer, 0);
                     break;
               }

               /*
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
               */
               
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

static void remove_group(Group *groups, int to_remove, int *actual_group)
{
   /* we remove the group in the array */
   memmove(groups + to_remove, groups + to_remove + 1, (*actual_group - to_remove - 1) * sizeof(Group));
   /* number group - 1 */
   (*actual_group)--;
}

static void remove_member(Group *groups, int to_remove, int group_index)
{
   /* we remove a member in the array which is not the last one */
   memmove(groups[group_index].members_name + to_remove, groups[group_index].members_name + to_remove + 1, (groups[group_index].nbMembers - to_remove - 1) * BUF_SIZE);
   
   /* number member - 1 */
   (groups[group_index].nbMembers)--;
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

/*
static void send_message_to_group(Client sender, Group group, const char *buffer)
{
   int i = 0;
   char message[BUF_SIZE];
   message[0] = 0;
   for(i = 0; i < actual_group; i++)
   {
      // we don't send message to the sender 
      if(sender.sock != group.members[i].sock)
      {
         strncpy(message, sender.name, BUF_SIZE - 1);
         strncat(message, " : ", sizeof message - strlen(message) - 1);
         strncat(message, buffer, sizeof message - strlen(message) - 1);
         write_client(group.members[i].sock, message);
      }
   }
}
*/

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
