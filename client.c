#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <strings.h>


#define LISTENQ   1024
//#define MAXLINE   512


void communication(int sockfd, char message[]);


int main(int argc, char *argv[])
{
    int sockfd; //, rv;
    char message[512];
    struct sockaddr_in *servaddr;

    struct addrinfo *hintsinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));
    struct addrinfo *servaddrinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));

    if (argc != 3)
    {
        printf("\nusage: %s <hostname> <port_number>\n\n", argv[0]);
        exit(-1);
    }


    if ( getaddrinfo(argv[1], NULL, hintsinfo, &servaddrinfo) < 0)
    {
        printf("Error in getaddrinfo call: %s\n", gai_strerror);
        exit(-1);
    }

    servaddr = (struct sockaddr_in *)servaddrinfo->ai_addr;

    // Assign values to some members of the struct that servaddr points to.
    servaddr->sin_port = htons(atoi(argv[2]));
    servaddr->sin_family = AF_INET;

    //tcpConnection(sockfd, servaddr, message);

    // Creates a socket for tcp using the SOCK_STREAM and validates it.
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        perror("Socket call failed\n");
        exit(-1);
    }

    if ( ( connect(sockfd, (struct sockaddr *) servaddr, sizeof(struct sockaddr))) == -1 )
    {
        perror("Connect call failed successfully\n");
        exit(-1);
    }

    communication(sockfd, message);

    close(sockfd);
    free(servaddrinfo);
    free(hintsinfo);
}


void communication(int sockfd, char message[])
{
    if ( recv(sockfd, message, 512, 0) > 0 )
    {
        printf("Client received: %s\n", message);
    }
    else
    {
        printf("\nServer has died peacefully!\n");
        close(sockfd);
        exit(-1);
    }

    if ( send(sockfd, "READY", 512, 0) > 0 )
    {
        printf("Client sent: %s\n", "READY");
    }
    else
    {
        printf("\nServer has died peacefully!\n");
        close(sockfd);
        exit(-1);
    }


}