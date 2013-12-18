#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int number;
    int maxnumber;
    int i,j;
    srand(time(NULL));
    maxnumber = 1<<30;
    FILE * fp;
    fp = fopen("file01", "w");

    for(i = 0; i < 1000; i++)
    {
        for(j = 1; j<10; j++)
        {

            number = rand() % maxnumber;
            fprintf(fp, "%d ", number);
        }
        fprintf(fp,"\n");
    }


    fclose(fp);

    return 0;
}
