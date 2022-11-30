/****************************************************************************************/
/*																				                                              */
/* Author: 			    Hamad Ahmed 												    										        */
/* Date Created:   	November 6, 2022   											    							          */
/* Major:  		      Computer Science											                              */
/* Professor Name: 	Dr. Frye    												                                */
/* Due Date: 		    11:59pm, Tuesday, November 29, 2022     						                */
/* Course:          CSC 328                                                             */
/* Assignment: 		  Wordle assignment												                            */
/* Filename: 		    client.c     						        				                            */
/* Description: 	  The purpose of this assignment is to establish a connection with a  */
/*                  server, communicate with the server, and create the Wordle gameplay */
/* Citations:       Line 64 to 75, in this program, is based on Dr. Frye's 'client.c',  */
/*                  found in her '/export/home/public/frye/csc328/sockets/sum'          */
/*                  directory, example's from line 102 to 114.                          */
/*                                                                                      */
/****************************************************************************************/

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

// Library functions prototypes
char *msgRec(int sock);
void msgSend(int sockfd, char msg[]);
void errCheck(int retVal, char* msg, int term );

// Client functions prototypes
void welcomeMessage(int len);
void tolowerCase(char *s, int len);
int claValidation(int argc, char *argv[], int port);
void defaultGuesses(char *arr, int len);
void guessValidation(char *correctGuess, char *word, char* guess);


int main(int argc, char *argv[]) 
{
  // Declares all the variables needed.
  char guess[SIZE], word[SIZE] = {}, correctGuess[SIZE] = {};
  int port, replay, sockfd, attempt = 1; 
  char sendMsg[512] = {};
  char *message = (char *)malloc(sizeof(char)*512);
  struct sockaddr_in *servaddr;

  struct addrinfo *hintsinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));
  struct addrinfo *servaddrinfo = (struct addrinfo *)malloc(sizeof(struct addrinfo));

  // Validates the command line arguments and stores the port number 
  port = claValidation(argc, argv, port);

  // Converts the hostname to IP Address and validates it.
  if ( getaddrinfo(argv[1], NULL, hintsinfo, &servaddrinfo) < 0)
  {
      printf("Error in getaddrinfo call: %s\n", gai_strerror);
      exit(-1);
  }

  // servaddr points to the ai_addr struct in serveraddrinfo struct
  servaddr = (struct sockaddr_in *)servaddrinfo->ai_addr;

  // Assign values to some members of the struct that servaddr points to.
  servaddr->sin_port = htons(port);
  servaddr->sin_family = AF_INET;

  // Establishes a TCP connection and validates it.
  errCheck(sockfd = socket(AF_INET, SOCK_STREAM, 0), "Socket call failed successfully\n", 0);
  errCheck(connect(sockfd, (struct sockaddr *) servaddr, sizeof(struct sockaddr)), "Connect call failed successfully\n", 0);

  // Receives a message from the server and stores it.
  strcpy(message, msgRec(sockfd));
 
  // Sends a "READY" message out to the server.
  strcpy(sendMsg, "READY");
  msgSend(sockfd, sendMsg);

  // Receives a message from the server and stores it.
  strcpy(message, msgRec(sockfd));
  strcpy(word, message);
  
  // Starts the gameplay.
  welcomeMessage(strlen(word));
  defaultGuesses(correctGuess, strlen(word));

  do {
    // Displays the correct guesses
    printf("%s\n", correctGuess);

    // Prompts the user for a guess, stores it, and converts it into lowercase.
    printf("Please enter your %d-letter guess #%d:\n", strlen(word), attempt);
    scanf("%s", guess);
    tolowerCase(guess, strlen(guess));

    // Validates the user guess
    guessValidation(correctGuess, word, guess);
    attempt++;

    if (!strcmp(word, guess) || !strcmp(word, correctGuess)) {
      attempt = 7;
      printf("Congrajulations, you have guessed it right!\n");
    }

    if (attempt > 6) {
      // Prompts the user to replay or quit the game if
      // the user runs out of attempts or guesses the word correctly with the given attempts.
      printf("The actual word was %s\n", word);
      printf("1. Play again\n");
      printf("2. Quit\n");
      scanf("%d", &replay);

      // If the user chooses to replay, then resets some of the values
      // and restarts the game.
      if (replay == 1) {
        attempt = 1;

        strcpy(sendMsg, "MORE");
        msgSend(sockfd, sendMsg);

        strcpy(message, msgRec(sockfd));
        strcpy(word, message);

        memset(correctGuess, 0, SIZE * sizeof(char));
        defaultGuesses(correctGuess, strlen(word));

      } else if (replay == 2) { // Terminates the program, if the users want to quit the game.
        strcpy(sendMsg, "QUIT");
        msgSend(sockfd, sendMsg);

        printf("Thank you for playing\n");
        exit(0);
      } else { // Validates the option.
        printf("Please run the program again and enter a valid input\n");
        exit(-1);
      }
    }
  } while (attempt <= 6);

  // Closes the file descriptor and frees the malloc memory.
  close(sockfd);
  free(servaddrinfo);
  free(hintsinfo);
  free(message);

  return 0;
}

/**********************************************************************/
/* 																		                                */
/* Function name:	welcomeMessage								                      */
/* Description: 	Displays a welcome message out to the screen        */
/* Parameters:  	int len - length of the word                        */
/* Return Value: 	none                                                */
/* 																	                                  */
/**********************************************************************/
void welcomeMessage(int len) {
  // Displays a welcome message 
  printf("Welcome to the Wordle Game\n");
  printf("You have six attempts to guess the %d-letter word: \n", len);
}


/*****************************************************************************/
/* 																		                                       */
/* Function name:	tolowerCase    								                             */
/* Description: 	Converts all the letters in a string to lower case letters */
/* Parameters:  	char *s - string to convert                                */
/*                int len - length of the string                             */
/* Return Value: 	none                                                       */
/* 																	                                         */
/*****************************************************************************/
void tolowerCase(char *s, int len)
{
  // Loops through each character and replaces it
  // with its lower case version.
  for (int i = 0; i < len; i++)
  {
    s[i] = tolower(s[i]);
  }
}

/*****************************************************************************/
/* 																		                                       */
/* Function name:	claValidation  								                             */
/* Description: 	Validates the command-line arguments and returns the port  */
/*                number                                                     */
/* Parameters:  	int argc - number of command-line arguments                */
/*                char *argv - command-line arguements array                 */
/*                int port - port number                                     */
/* Return Value: 	port number                                                */
/* 																	                                         */
/*****************************************************************************/
int claValidation(int argc, char *argv[], int port)
{
  // Checks if the user inputs the correct number of CLA.
  if (argc > 3 || argc < 2)
  {
      printf("\nusage: %s <hostname> <port_number: optional>\n\n", argv[0]);
      exit(-1);
  }
  // If the user inputs 3 CLA, then stores the port and validates it.
  else if (argc == 3)
  {
    port = atoi(argv[2]);
    if (port < 1024 || port > 65535)
    {
      printf("Port number out or range, must be between 1024 and 65535 inclusive!\n");
      exit(-1);
    }
  }
  // Else if user inputs 2 CLA, then sets the default port number.
  else if (argc == 2)
  {
    port = 3280;
  }

  return port;
}


/*****************************************************************************/
/* 																		                                       */
/* Function name:	defualtGuesses    								                         */
/* Description: 	Sets the correct guesses array to its default value        */
/* Parameters:  	char *arr - correct guesses array                          */
/*                int len - length of the array                              */
/* Return Value: 	none                                                       */
/* 																	                                         */
/*****************************************************************************/
void defaultGuesses(char *arr, int len)
{
  // Resets the correct guesses array to it's default values.
  for (int i = 0; i < len; i++)
  {
    arr[i] = '_';
  }
}

/*****************************************************************************/
/* 																		                                       */
/* Function name:	guessValidation      					                             */
/* Description: 	validates the user guess and stores the correct values     */
/* Parameters:  	char *correctGuess - correct guesses array                 */
/*                char *word - the actual word                               */
/*                char *guess - user's guess                                 */
/* Return Value: 	none                                                       */
/* 																	                                         */
/*****************************************************************************/
void guessValidation(char *correctGuess, char *word, char* guess)
{
  // Compares the user guess with the actual word and
  // stores the found letters in their correct position.
  for (int i = 0; i < SIZE; i++) {
      for (int k = 0; k < SIZE; k++) {
        if (word[i] == guess[k])
          correctGuess[i] = word[i];
      }
    }
}