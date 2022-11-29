#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#define SIZE      12
#define LISTENQ   1024


char *msgRec(int sock);
void msgSend(int sockfd, char msg[]);
void errCheck(int retVal, char* msg, int term );

void welcomeMessage(int len);
void printArray(char arr[]);
void tolowerCase(char s[], int len);

int main(int argc, char *argv[]) 
{
  int attempt = 1;
  char guess[SIZE];
  char word[SIZE] = {};
  char correctGuess[SIZE] = {};
  int replay;
  int sockfd; //, rv;
  char sendMsg[512] = {};
  char *message = (char *)malloc(sizeof(char)*512);
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

  // 1
  //sockfd = socket(AF_INET, SOCK_STREAM, 0);
  errCheck(sockfd = socket(AF_INET, SOCK_STREAM, 0), "Socket call failed successfully\n", 0);

  // 2. 
  errCheck(connect(sockfd, (struct sockaddr *) servaddr, sizeof(struct sockaddr)), "Connect call failed successfully\n", 0);

  // 3. 
  strcpy(message, msgRec(sockfd));
  printf("Client received: %s\n", message);
  //errCheck();

  // 4. 
  strcpy(sendMsg, "READY");
  msgSend(sockfd, sendMsg);
  printf("Client sent: READY\n");

  // 5.
  strcpy(message, msgRec(sockfd));
  printf("Client received: %s\n", message);
  strcpy(word, message);
  
  welcomeMessage(strlen(word));

  do {
    // printArray(correctGuess);
    printf("%s\n", correctGuess);

    for (int i = 0; i < strlen(word); i++)
    {
      correctGuess[i] = '_';
    }

    printf("Please enter your %d-letter guess #%d:\n", strlen(word), attempt);
    scanf("%s", guess);
    tolowerCase(guess, strlen(guess));

    for (int i = 0; i < SIZE; i++) {
      for (int k = 0; k < SIZE; k++) {
        if (word[i] == guess[k])
          correctGuess[i] = word[i];
      }
    }

    attempt++;

    if (!strcmp(word, guess) || !strcmp(word, correctGuess)) {
      attempt = 7;
      printf("Congrajulations, you have guessed it right!\n");
    }

    if (attempt > 6) {
      printf("1. Play again\n");
      printf("2. Quit\n");
      scanf("%d", &replay);

      if (replay == 1) {
        attempt = 1;

        // 6. 
        //msgSend(sockfd, "MORE");
        strcpy(sendMsg, "MORE");
        msgSend(sockfd, sendMsg);
        printf("Client sent: MORE\n");

        // 7. 
        strcpy(message, msgRec(sockfd));
        printf("Client received: %s\n", message);
        strcpy(word, message);

        for (int i = 0; i < strlen(word); i++)
        {
        correctGuess[i] = '_';
        }

      } else if (replay == 2) {
        // 8. 
        // msgSend(sockfd, "QUIT");
        strcpy(sendMsg, "QUIT");
        msgSend(sockfd, sendMsg);
        printf("Client sent: QUIT\n");

        printf("Thank you for playing\n");
        exit(0);
      } else {
        printf("Please run the program again and enter a valid input\n");
        exit(-1);
      }
    }
  } while (attempt <= 6);


  close(sockfd);
  free(servaddrinfo);
  free(hintsinfo);
  free(message);

  return 0;
}

void welcomeMessage(int len) {
  printf("Welcome to the Wordle Game\n");
  printf("You have six attempts to guess the %d-letter word: \n", len);
}

void printArray(char arr[]) {
  for (int i = 0; i < SIZE; i++) {
    printf("%c", arr[i]);
  }
}

void tolowerCase(char s[], int len)
{
  for (int i = 0; i < len; i++)
  {
    s[i] = tolower(s[i]);
  }
}