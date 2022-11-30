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

//For questions on the project as a whole check the README


/*Author:        Boyd Lenich                                         */
/*Filename:      msgHandle.c                                         */
/*Creation date: 11/18/2022                                          */
/*Professor:     Dr. Frye                                            */
/*Assignment:    Final project: wordle                               */
/*Purpose:       Handle send recieving and messaging protocol        */



/*
-Function:      msgSend()
-Description:   Handles message sending
-Parameters:    int sockfd: The socket on which you want to send
                char msg[512]:  The message you want to send 
            NOTE: Can not be passed a plane string, must be char* variable
            I.E msgSend(sock, msg) works, msgSend(sock, "HELLO") does not             
*/
void msgSend(int sockfd, char msg[512])
{
    int ret;
    //Message protocol
    strcat(msg, "<");
    if(send(sockfd, msg, 512, 0) == -1){
        perror("Error sending");
        close(sockfd);
    }else{
        printf("\nsent!\n");
    }
    
}


/*
-Function:      msgRec()
-Description:   Handles message recieving
-Parameters:    int sockfd: The socket on which you want to recieve
-Return value:  Returns the message recieved as a char*
*/

char *msgRec(int sockfd)
{
    char msg[512];  //base message recieved
    char *finMsg = malloc(sizeof(char)*512); //the *final* that has been processed
    int byteRec = recv(sockfd, msg, 512, 0); 
    if(byteRec> 0){
       
    }else if(byteRec ==0) {
        printf("Recieved nothing, closing");
        close(sockfd);
    }else{
        perror("Error recieving");
        close(sockfd);
    }   
    //Message protocol
    for(int i = 0; i < 512; i++){
        if(msg[i] == '<'){
            memcpy(finMsg, msg, (i)); 
            return(finMsg);
        }
    }
}
