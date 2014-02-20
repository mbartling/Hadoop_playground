/*
 * map_words.c
 *
 *  Created on: Feb 20, 2014
 *      Author: bart4128
 */

#include <stdio.h>
#include <cstdint>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_LEN 	200
#define ZERO_BYTE 		0

void ReadLine(char *line);
void emit_word(char *line);

int main(int argc, const char * argv[])
{
	char line[MAX_LINE_LEN];

	ReadLine( line );
	emit_word( line );

	return 0;
}

void ReadLine( char * line)
{

	int num_chars_read;

	num_chars_read = 0;

	while( ( *line = getchar() ) != '\n' )
	{
		line++;
		if( ++num_chars_read >= MAX_LINE_LEN ) break;
	}
	//*line = NULL;
	*line = ZERO_BYTE;

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
