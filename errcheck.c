
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*For questions on the project as a whole check the README*/

/*Author:        Boyd Lenich                                */
/*Filename:      Errcheck.c                                 */
/*Creation date: 11/15/2022                                 */
/*Professor:     Dr. Frye                                   */
/*Assignment:    Final project: wordle                      */
/*Purpose:       Handle errors for wordle project           */

/*
-Function:      errCheck()
-Description:   handles error checking for worlde project to keep main code clean
-Parameters:    int retVal: the return value from the function you want to check
                char *msg: The text you want to be placed in the perror
                int term: stands for terminating, if 0 it kills the program, if 1 it keeps running
*/

//NOTE:You can place the function you are invoking in place of retVal directly
//  to save code space
void errCheck(int retVal, char *msg, int term )
{
	if (retVal == -1){
        perror(msg);
        if(term == 0){
            exit(-1);
        }
    }
}
