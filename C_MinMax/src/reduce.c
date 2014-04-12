/*
 * reduce.c
 *
 *  Created on: Jan 9, 2014
 *      Author: Mike
 *
 *      Essentially the same thing as the collect function
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>
//#include <time.h>
#include <stdlib.h>


enum
{
	key_max=1,
	key_min
};

void myReduceLines(void)
{

	int32_t key;
	int32_t prev_key;

	double min_val;
	double max_val;
	double val;
	double local_max_val;
	double local_min_val;

	/* Set the limits so min and max are always set properly */
	local_min_val = DBL_MAX;
	local_max_val = DBL_MIN;
	prev_key = 0;
	while (scanf("%d\t%lf\n",&key, &val) > 0)
	{

		//If we have moved onto the next key set then do this
		if( prev_key && prev_key != key)
		{

			/* Update to new key set */
			prev_key = key;
			local_min_val 	= (val < local_min_val) ? val: local_min_val;
			local_max_val 	= (val > local_max_val) ? val: local_max_val;

		}
		else /* We have the first key set or are processing the same key set*/
		{

			/* Process each line separately */
			prev_key 		= key;
			local_min_val 	= (val < local_min_val) ? val: local_min_val;
			local_max_val 	= (val > local_max_val) ? val: local_max_val;
		} /* End Else*/

	} /* End While */

	printf("Max\t%lf\n", local_max_val);
	printf("Min\t%lf\n", local_min_val);



}// End myReduceLines

int main(void)
{
	myReduceLines();
	return 0;
}
