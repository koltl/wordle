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


void msgSend(int sock, char msg[512])
{
    int ret;
    strcat(msg, "<");
    printf("\nsending: ");
    printf(msg);
    if(send(sock, msg, 512, 0) == -1){
        perror("Error sending");
        close(sock);
    }else{
        printf("\nsent!");
    }
    
}


char *msgRec(int sockfd)
{
    char msg[512];
    char *finMsg = malloc(sizeof(char)*512);
    if(recv(sockfd, msg, 512, 0) > 0){
        printf("message recieved!");
    }
    printf("\n\nThe base recieved message is ");
    printf("%s\n", msg);
    
    for(int i = 0; i < 512; i++){
        if(msg[i] == '<'){
            memcpy(finMsg, msg, (i));
            printf("Final Message: ");
            printf(finMsg);
            printf("\n");
            return(finMsg);
        }
    }
}
