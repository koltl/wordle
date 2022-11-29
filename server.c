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

// library function definitions
char *msgRec(int sock);
void msgSend(int sockfd, char msg[]);
void errCheck(int retVal, char* msg, int term );

int main(int argc, char *argv[]) {
    int sockfd, clientsockfd, port, pid, numberOfProccesses;

    struct sockaddr_in server;

    // this program can support a maximum of two arguments (file & port number)
    if (argc > 2) {

        // if # of arguments is greater than two, print out usage clause
        printf("Usage: %s port_number\n", argv[0]);
        exit(-1);

    } else if (argc == 2) {

        // if port number is not within correct range,
        // print the accepted port range and exit
        port = atoi(argv[1]);
        if (port < 1024 || port > 65535) {
            printf("Port number must be between 1024 and 65535, inclusively\n");
            exit(-1);
        }

    } else {

        // no second command line argument (port) given,
        // set a default port
        port = 3280;

    }
    
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = INADDR_ANY;

    // create a socket, otherwise run perror and exit
    errCheck(sockfd = socket(AF_INET, SOCK_STREAM, 0),
            "Error with creating server socket",
            0);

    // bind to IP:PORT, otherwise run perror and exit
    errCheck(bind(sockfd, (struct sockaddr *)&server, sizeof(struct sockaddr_in)),
            "Binding socket failed",
            0);

    // listen for up to 12 connections,
    // otherwise run perror and exit
    errCheck(listen(sockfd, 12), 
            "Error with listening on socket",
            0);

    // log that server is now listening on specified port
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
    msgSend(clientsocket, "HELLO");

    // receive word from client
    char *recMessage = msgRec(clientsocket);
    printf("Child (%d): received '%s'\n", getpid(), recMessage);

    // if word received is 'QUIT', quit the process.
    if (strcmp("QUIT", recMessage) == 0) {
        printf("Child (%d): Exiting process\n", getpid());
        close(clientsocket);
        exit(0);
    }

    // if word received is either 'READY', 'MORE', or 'WORD',
    // select a random word and send to client
    while (strcmp("READY", recMessage) == 0 
            || strcmp("MORE", recMessage) == 0
            || strcmp("WORD", recMessage) == 0) {

        // generate random number in range of word pool size
        // and set random word to an item in words pool list
        // at a random index
        randomNum = rand() % 7;
        strcpy(randomWord, words[randomNum]);

        // log and send the random word to client
        printf("Child (%d): sending word '%s' to client\n", getpid(), randomWord);
        msgSend(clientsocket, randomWord);

        // receive a message from client
        char *recMessage = msgRec(clientsocket);
        printf("Child (%d): received '%s'\n", getpid(), recMessage);
    }

    // if received message is something other than 'READY',
    // 'MORE', and 'WORD' - end the process
    if (strcmp("QUIT", message) == 0) {
        printf("Child (%d): Exiting process\n", getpid());
        close(clientsocket);
        exit(0);
    }

    printf("Child (%d): Exiting process\n", getpid());
    close(clientsocket);
    exit(0);
}