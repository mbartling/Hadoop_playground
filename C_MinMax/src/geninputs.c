#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUMLINES 1000
#define NUM_ENTRY_LINE 10

int main(int argc, char**argv)
{
	int number;
	int numLines;
	int i,j;
	srand(time(NULL));
	//    scanf("%d",&numLines);
	if(argc == 2)
	{
		if(sscanf(argv[1],"%d",&numLines) < 1)
		{
			return -1;
		}

		for(i = 0; i < numLines; i++)
		{
			for(j = 1; j<NUM_ENTRY_LINE-1; j++)
			{

				number = rand();
				printf("%d ", number);
			}
			number = rand();
			printf("%d", number);

			printf("\n");
		}
	}
	else
	{
		FILE * fp;
		fp = fopen("file01", "w");

		for(i = 0; i < NUMLINES; i++)
		{
			for(j = 1; j<NUM_ENTRY_LINE-1; j++)
			{

				number = rand();
				fprintf(fp, "%d ", number);
			}
			number = rand();
			fprintf(fp, "%d", number);

			fprintf(fp,"\n");
		}


		fclose(fp);
	}
	return 0;
}
