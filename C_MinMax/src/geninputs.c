#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMLINES 1000
#define NUM_ENTRY_LINE 10


int main()
{
    int number;

    int i,j;
    srand(time(NULL));

    FILE * fp;
    fp = fopen("file01", "w");

    for(i = 0; i < NUMLINES; i++)
    {
        for(j = 1; j<NUM_ENTRY_LINE; j++)
        {

            number = rand();
            fprintf(fp, "%d ", number);
        }
        fprintf(fp,"\n");
    }


    fclose(fp);

    return 0;
}
