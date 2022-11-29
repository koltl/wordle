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


void msgSend(int sockfd, char msg[512])
{
    int ret;
    strcat(msg, "<");
    if(send(sockfd, msg, 512, 0) == -1){
        perror("Error sending");
        close(sockfd);
    }else{
        printf("\nsent!\n");
    }
    
}


char *msgRec(int sockfd)
{
    char msg[512];
    char *finMsg = malloc(sizeof(char)*512);
    int byteRec = recv(sockfd, msg, 512, 0);
    if(byteRec> 0){
        printf("\nmessage recieved!");
    }else if(byteRec ==0) {
        printf("Recieved nothing, closing");
        close(sockfd);
    }else{
        perror("Error recieving");
        close(sockfd);
    }
    for(int i = 0; i < 512; i++){
        if(msg[i] == '<'){
            memcpy(finMsg, msg, (i));
            return(finMsg);
        }
    }
}
