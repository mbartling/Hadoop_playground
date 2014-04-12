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

void myReduceLines(void);

extern int errno;
int main(int argc, const char * argv[])
{

    myReduceLines();

    exit(EXIT_SUCCESS);
}


void myReduceLines( void )
{
    char line[MAX_LINE_LEN];
    char word[MAX_LINE_LEN];
    char prev_word[MAX_LINE_LEN];
    char *result;
    int n = 0;
    int word_count = 0;

    //fgets(line, MAX_LINE_LEN, stdin);
    while((result = fgets(line, MAX_LINE_LEN, stdin)) != NULL)
    {
        if(ferror(stdin))
        {
            perror("Error reading stdin.");
        }
        if( sscanf(line , "%s\t%d", word, &n) > 0)
        {
            //if(prev_word)
            //printf("%s\t%d\n",word,n);
            if(strcmp(prev_word,word) == 0)
            {
                word_count += n;
            }else
            {
                if(word_count != 0)
                {
                    printf("%s\t%d\n",prev_word,word_count);
                }
                    strcpy(prev_word,word);
                    word_count = n;
            }// end else words not equivalent
        }
    }
        printf("%s\t%d\n",prev_word,word_count);
    if(ferror(stdin))
    {
        perror("Error reading stdin.");
    }


}

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

