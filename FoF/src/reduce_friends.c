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

#define MAX_LINE_LEN 	400
#define ZERO_BYTE 		0

#define DEBUG_MODE 0

#if DEBUG_MODE
#define FPRINTF(...) fprintf(__VA_ARGS__)
#else
#define FPRINTF(...) /*Dummy*/
#endif

void myReduceLines(void);
void intersect_friends(char *, char *);
extern int errno;
int main(int argc, const char * argv[])
{

    myReduceLines();

    exit(EXIT_SUCCESS);
}

//http://www.daniweb.com/software-development/c/threads/206198/intersection-of-two-strings
void myReduceLines( void )
{
    char line[MAX_LINE_LEN];
    char key[MAX_LINE_LEN];
    char prev_key[MAX_LINE_LEN];
    char friends[MAX_LINE_LEN];
    char mutual_friends[MAX_LINE_LEN];
    char outputs[MAX_LINE_LEN];
    char *result;
    int key_count = 0;
    int idx;
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        while(line[idx])
        {
            if(line[idx] == '\n')
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;

        //if( sscanf(line , "%[^\t]s\t%[^\t\n\0]s", key, friends) > 0)
        if( sscanf(line , "%[^\t]%n", key, &idx) > 0)
        {
            strcpy(friends, &line[idx]);
            //fputs(key, stderr);
            FPRINTF(stderr, "\tK: %s\n", key);
            FPRINTF(stderr, "\tN: %d\n", idx);
            FPRINTF(stderr, "\tFA: %s\n", friends);
            //fputs(friends, stderr);
            if(strcmp(prev_key,key) == 0) //If Keys are equal
            {
                key_count += 1;
                //Intersect
                intersect_friends(mutual_friends, friends);
            }else //Keys are not the same
            {
                if(key_count != 0)
                {
                    strcpy(outputs, prev_key);
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
        //puts(outputs);
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

    //char * str1 = &str1_b[0];
    //char * str2 = &str2_b[0];
    int n1;
    int n2;
    int idx1 = 0;
    int idx2 = 0;

    while(sscanf(&str1[idx1] ,"%s%n", friend1, &n1) > 0)
    {
       idx1 += n1;
       idx2 = 0;

       while(sscanf(&str2[idx2], "%s%n", friend2, &n2) > 0)
       {
            idx2 += n2;
               // FPRINTF(stderr, "\tF1: %s\n",friend1);

            if(strcmp(friend1, friend2) == 0)
            {
                strcat(temp, friend2);
                strcat(temp, " ");

                FPRINTF(stderr, "\tF2: %s\n",friend2);
            }
       }

    }
    //puts(temp);
    //Super Dangerous
    FPRINTF(stderr, "\tT: %s\n",temp);
    strcpy(str1, temp);
    //memcpy(str1,temp, MAX_LINE_LEN);
}
