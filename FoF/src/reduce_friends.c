/*
 * reduce_words.c
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

#define MAX_LINE_LEN 	200
#define ZERO_BYTE 		0

void ReadLines(void);

extern int errno;
int main(int argc, const char * argv[])
{

    ReadLines();

    exit(EXIT_SUCCESS);
}

//http://www.daniweb.com/software-development/c/threads/206198/intersection-of-two-strings
void ReadLines( void )
{
    char line[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char prev_key[MAX_LINE_LEN];
    char friends[MAX_LINE_LEN];
    char mutual_friends[MAX_LINE_LEN];
    char outputs[MAX_LINE_LEN];
    char *result;
    int key_count = 0;

    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        if( sscanf(line , "%s\t%s", key, friends) > 0)
        {
            if(strcmp(prev_key,key) == 0) //If Keys are equal
            {
                key_count += 1;
                //Intersect
                intersect_friends(mutual_friends, friends);
            }else //Keys are not the same
            {
                if(key_count != 0)
                {
                    strcat(outputs, prev_key);
                    strcat(outputs, "\t");
                    strcat(outputs, mutual_friends);
                    //printf("%s\t%d\n",prev_key,key_count);
                    puts(outputs);
                }
                    strcpy(prev_key,key);
                    strcpy(mutual_friends,friends);
                    key_count = 0;
            }// end else words not equivalent
        }
    }
        printf("%s\t%d\n",prev_word,key_count);
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }


}

/* Todo: Optimize this later*/
void intersect_friends(char * str1, char * str2)
{
    char temp[MAX_LINE_LEN];
    char friend1[MAX_LINE_LEN];
    char friend2[MAX_LINE_LEN];
    int n1;
    int n2;
    int idx1 = 0;
    int idx2 = 0;
    
    while(sscanf(&str1[idx1] ,"%s%n", friend1, &n1) > 0)
    {
        
    }
}
