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
    } else if (argc == 2) {
        port = atoi(argv[1]);
        if (port < 1024 || port > 65535) {
            printf("Port number must be between 1024 and 65535, inclusively\n");
            exit(-1);
        }
    } else {
        port = 3280;
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
                // log child process started and handle client in function
                printf("\nChild: Child process #%d started\n", getpid());
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
    char randomWord[MESSAGE_MAXSIZE];
    int randomNum;
    // pool of words for gameplay
    char words[7][MESSAGE_MAXSIZE] = {"pipe", "socket", "fork", "thread", "connect", "listen", "bind"};

    // seed random number generator
    srand(getppid());

    // send 'HELLO' to client
    printf("Child (%d): sending '%s'\n", getpid(), message);
    send(clientsocket, "HELLO", MESSAGE_MAXSIZE, 0);
    recv(clientsocket, message, MESSAGE_MAXSIZE, 0);
    printf("Child (%d): received '%s'\n", getpid(), message);

    if (strcmp("QUIT", message) == 0) {
        printf("Child (%d): Exiting process\n", getpid());
        close(clientsocket);
        exit(0);
    }
    while (strcmp("READY", message) == 0 
            || strcmp("MORE", message) == 0
            || strcmp("WORD", message) == 0) {
        randomNum = rand() % 7;
        strcpy(randomWord, words[randomNum]);
        printf("Child (%d): sending word '%s' to client\n", getpid(), randomWord);
        send(clientsocket, randomWord, MESSAGE_MAXSIZE, 0);
        recv(clientsocket, message, MESSAGE_MAXSIZE, 0);
        printf("Child (%d): received '%s'\n", getpid(), message);
    }

    if (strcmp("QUIT", message) == 0) {
        printf("Child (%d): Exiting process\n", getpid());
        close(clientsocket);
        exit(0);
    }
    printf("Child (%d): Exiting process\n", getpid());
    close(clientsocket);
    exit(0);
}