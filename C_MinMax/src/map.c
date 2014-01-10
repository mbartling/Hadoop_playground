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
//Map each line to a random of N keys and compute local min/max of input (may not be best idea since might be more efficient for the collector to do this step)
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

			min_val = (val < min_val) ? val: min_val;
			max_val = (val > max_val) ? val: max_val;

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
