#include <limits.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>

#define MAX_KEY 256

void parent_func()
{
	char *buffer[1024];

	while(readStdInLine(buffer))
	{
		//Process contents from buffer
	}
}

char * readStdInLine(char *buffer)
{
	char *result = fgets(buffer, 1024, stdin);
	int len;

	//fgets returns NULL on error or end of input
	//so the buffer contents will be undefined

	if (result == NULL)
	{
		return NULL;
	}

	len = strlen (buffer);
	if( len == 0 )
	{
		return NULL;
	}

	if (buffer[len - 1] == '\n' )
	{
		buffer[len - 1] = 0;
	}
}

//Assume one number per line

//Map each line to a random of N keys and compute local min/max of input (may not be best idea since might be more efficient for the collector to do this step)
void myMapOneLine(void)
{
	int32_t key; //randomly generated key
	int32_t value;

	static int32_t min_val = INT_MAX;
	static int32_t max_val = INT_MIN;

	scanf("%d", &value);
	srand(time(NULL));
	key = rand() % MAX_KEY;

	min_val = (value < min_val) ? value: min_val;
	max_val = (value > max_val) ? value: max_val;

	//printf("%d\t%d ", key, value);
	printf("%d\t%d\t%d ", key, min_val, max_val);
}

/**
 * Best Map function
 */
void myMapLines(void)
{
	char line[1024], *pos, *end;
	int32_t key;
	double val;
	double min_val;
	double max_val;
	while (fgets(line, sizeof(line), stdin)) {

		/* Set the limits so min and max are always set properly */
		min_val = DBL_MAX;
		max_val = DBL_MIN;

		/* Process each line separately */
		pos = line;
		for (pos = line; ; pos = end) {
			val = strtod(pos, &end);
			if (pos == end)
				break;
			// process val here

			min_val = (value < min_val) ? value: min_val;
			max_val = (value > max_val) ? value: max_val;

		}
		srand(time(NULL));
		key = rand() % MAX_KEY;
		printf("%d\t%f\t%f", key, min_val, max_val);
	}

}// End myMapLines

void main(void)
{
	myMapLines();
}
