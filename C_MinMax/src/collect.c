/*
 * collect.c
 *
 *  Created on: Jan 1, 2014
 *      Author: Mike
 */


#include <limits.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>

#define MAX_KEY_IN 256

enum
{
	key_max,
	key_min
};

void myMapLines(void)
{

	int32_t key;
	int32_t prev_key;
	double val;
	double min_val;
	double max_val;

	double local_max_val;
	double local_min_val;

	/* Set the limits so min and max are always set properly */
	local_min_val = DBL_MAX;
	local_max_val = DBL_MIN;
	prev_key = 0;
	while (scanf("%d\t%f\t%f\n",&key, &min_val, &max_val) > 0)
	{

		//If we have moved onto the next key set then do this
		if( prev_key && prev_key != key)
		{
			printf("%d\t%f\n", key_max, max_val);
			printf("%d\t%f\n", key_min, min_val);

			/* Update to new key set */
			prev_key = key;
			local_max_val = max_val;
			local_min_val = min_val;

		}
		else /* We have the first key set or are processing the same key set*/
		{

			/* Process each line separately */
			prev_key 		= key;
			local_min_val 	= (min_val < local_min_val) ? min_val: local_min_val;
			local_max_val 	= (max_val > local_max_val) ? max_val: local_max_val;
		} /* End Else*/

	} /* End While */

	printf("%d\t%f\n", key_max, max_val);
	printf("%d\t%f\n", key_min, min_val);


}// End myMapLines

void main(void)
{
	myMapLines();
}
