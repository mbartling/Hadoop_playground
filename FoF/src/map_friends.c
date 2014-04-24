/*
 * map_friends.c
 *
 *  Created on: Feb 20, 2014
 *      Author: bart4128
 */

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 	10000
#define ZERO_BYTE 		0

void myMapLines(void);

extern int errno;
int main(int argc, const char * argv[])
{

    myMapLines();
    //emit_friend( line );

    //return 0;

    exit(EXIT_SUCCESS);
}


void myMapLines( void )
{
    char line[MAX_LINE_LEN];
    char friend[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char output[2*MAX_LINE_LEN];
    char *result;
    int n = 0;
    int idx = 0;
    int comparison;
    int friend_offset;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        /* Convert everything to lower case and remove any numbers */
        while(line[idx])
        {
            if(line[idx] == '\n')
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;

        /*Read in string and count number of characters read*/
        sscanf(&line[idx] , "%s %n", key, &n);
        idx += n;
        friend_offset = idx;
        while( sscanf(&line[idx] , "%s %n", friend, &n) > 0)
        {
            //Clear the output string
            //memset(output, '\0', 2*MAX_LINE_LEN);

            idx += n;
            comparison = strcmp(key, friend);
            if( comparison > 0)
            {
                //strcat(output, friend);
                strcpy(output, friend);
                strcat(output, " ");
                strcat(output, key);
                strcat(output, "\t");
                strcat(output, &line[friend_offset]);

            }
            if( comparison < 0)
            {
                //strcat(output, key);
                strcpy(output, key);
                strcat(output, " ");
                strcat(output, friend);
                strcat(output, "\t");
                strcat(output, &line[friend_offset]);

            }
            puts(output);
        }
        idx = 0;
    }
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }




}
