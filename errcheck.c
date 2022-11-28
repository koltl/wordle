/*************************************************************
 *      
 * Filename: errcheck.c 
 * purpose: Handles error checking for ease of use and code readability
 *
 *************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//retVal should be the value returned by a function that is being error checked
//msg is a pointer to the string you want to send with perror
//term should be a 0 if you want to terminate program on error

void errCheck(int retVal, char *msg, int term )
{
	if (retVal == -1){
        perror(msg);
        if(term == 0){
            exit(-1);
        }
    }
}
