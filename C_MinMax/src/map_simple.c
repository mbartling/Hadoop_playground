/**
 * map.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Mike
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_KEY 255

enum
{
	key_max=1,
	key_min
};

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

	/** Get arbitrary number of numbers from the line */
	while (fgets(line, sizeof(line), stdin)) {

		/* Set the limits so min and max are always set properly */
		min_val = DBL_MAX;
		max_val = DBL_MIN;

		/* Process each line separately */
		pos = line;
		/* position ourselves at the start of each number and move forward*/
		for (pos = line; ; pos = end) {
			val = strtod(pos, &end);
			if (pos == end)
				break;
			// process val here

			min_val = (val < min_val) ? val: min_val;
			max_val = (val > max_val) ? val: max_val;

		}


		printf("%d\t%lf\n", key_max, max_val);
		printf("%d\t%lf\n", key_min, min_val);
	}

}// End myMapLines

int main(void)
{

	myMapLines();
	return 0;
}
