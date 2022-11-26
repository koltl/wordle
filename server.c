#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#define MESSAGE_MAXSIZE 512

void client_communication(int clientsocket);

int main(int argc, char *argv[]) {
    int sockfd, clientsockfd, port, pid, numberOfProccesses;

    struct sockaddr_in server;

    if (argc > 2) {
        printf("Usage: %s port_number\n", argv[0]);
        exit(-1);
    }

    port = 3280;

    if (port < 1024 || port > 65535) {
        printf("Port number must be between 1024 and 65535, inclusively\n");
        exit(-1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error with creating server socket");
        exit(-1);
    }

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1) {
        perror("Binding socket failed");
        exit(-1);
    }

    if (listen(sockfd, 12) == -1) {
        perror("Error with listening on socket");
        exit(-1);
    }

    printf("Server now listening on port %d\n", port);

    for (;;) {
        while (((clientsockfd = accept(sockfd, NULL, NULL)) == -1));
        printf("A client connected\n");
        pid = fork();
        switch(pid) {
            case -1:
                // case of fork failed to be created
                perror("\nFailed to create fork!  Exiting...");
                
                // close clientsockfd, fork failed
                close(clientsockfd);
            case 0:
                printf("\nChild: Child process started\n");
                client_communication(clientsockfd);
            
            default:
                if (pid != -1) {
                    numberOfProccesses++;
                }

        }
    }

}

void client_communication(int clientsocket) {
    char message[] = "HELLO";
    char words[4][44] = {"pipe", "socket", "fork", "thread"};
    printf(words[1]);
    printf("Server: sending %s\n", message);
    send(clientsocket, &message, MESSAGE_MAXSIZE, 0);
    recv(clientsocket, message, MESSAGE_MAXSIZE, 0);
    printf("From client: %s\n", message);
    while (strcmp("READY", message) == 0 || strcmp("MORE", message) == 0) {
        printf("Sending word to client... \n");
        send(clientsocket, words[3], MESSAGE_MAXSIZE, 0);
        recv(clientsocket, message, MESSAGE_MAXSIZE, 0);
        printf("this prints\n");
    }

    if (strcmp("QUIT", message) == 0) {
        printf("exiting process");
        exit(0);
    }
    exit(0);
}