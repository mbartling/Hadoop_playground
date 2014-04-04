/*
 * map_words.c
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
void emit_word(char *line);

extern int errno;
int main(int argc, const char * argv[])
{
    char line[MAX_LINE_LEN];

    ReadLines();
    //emit_word( line );

    //return 0;

    exit(EXIT_SUCCESS);
}


void ReadLines( void )
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char *result;
    int n = 0;
    int idx = 0;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        while(line[idx])
        {
            line[idx] = tolower(line[idx]);
            if(ispunct(line[idx]) || isdigit(line[idx]))
            {
                line[idx] = ' ';
            }
            idx++;
        }
        idx = 0;
        while( sscanf(&line[idx] , "%s%n", word, &n) > 0)
        {
            idx += n;
            printf("%s\t%d\n",word, 1);
        }
        idx = 0;
    }
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }




}
/* The below code is a simple example, but does not work*/
/*
void emit_word( char *line)
{
    int num_words;
    int in_word;
    char word[MAX_LINE_LEN];

    char *word_ptr;
    word_ptr = &word[0];

    num_words = 0;
    in_word = 0;

    while( *line != ZERO_BYTE )
    {
        if( !isspace( *line ) )
        {
            *word_ptr++ = *line;

            //			if( !in_word )
            //			{
            //				num_words++;
            //				in_word = 1;
            //
            //			} //End if !in_word
        }//end if not space
        else
        {
            *word_ptr = 0; //set null char
            word_ptr = word; //go to beginning
            printf("%s\t1\n", word); //emit
            //			in_word = 0;
        }
        line++;
    }

    //	return num_words;
}
*/
