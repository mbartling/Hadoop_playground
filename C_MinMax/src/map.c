/*
 * map.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Mike
 */

#include <float.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_KEY 255

/**
 * Simple map function which reads in lines from STDIN and outputs the max and min of each line
 */
void myMapLines(void)
{
	char line[1024], *pos, *end;
	int key;
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

			min_val = (val < min_val) ? val: min_val;
			max_val = (val > max_val) ? val: max_val;

		}

		key = (rand() % MAX_KEY) + 1;
		printf("%d\t%lf\t%lf\n", key, min_val, max_val);
	}

}// End myMapLines

int main(void)
{
	srand(time(NULL));
	myMapLines();
	return 0;
}
