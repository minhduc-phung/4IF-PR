/* Client pour les sockets
 *    socket_client ip_server port
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int sockfd, newsockfd, clilen, chilpid, ok, nleft, nbwriten;
    char c;
    struct sockaddr_in serv_addr;

    char host[15] = "142.250.179.78";
    //struct hostent *server;
    char message[30] = "GET\n\n"; 
    char response[4096];

    if (argc != 3)
    {
        printf("usage  socket_client method port\n");
        exit(0);
    }
    /*
     *  partie client
     */
    printf("client starting\n");

    /* initialise la structure de donnee */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(host);
    serv_addr.sin_port = htons(atoi(argv[2]));

    /* ouvre le socket */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("Open socket error\n");
        exit(0);
    }

    /* effectue la connection */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Connect to server error\n");
        exit(0);
    }
    
    /* send the request */
    send(sockfd, message, strlen(message), 0);
    recv(sockfd, response, sizeof(response), 0);

    close(sockfd);

    printf("Response: %s\n", response);

    return 0;
}
