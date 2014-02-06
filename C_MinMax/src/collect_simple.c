/*
 * collect.c
 *
 *  Created on: Jan 1, 2014
 *      Author: Mike
 */

#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <stdlib.h>

#define DEBUG_MODE 0
#if DEBUG_MODE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...) /*DUMMY*/
#endif


#define MAX_KEY_IN 256

enum
{
	key_max=1,
	key_min
};

void myCollectLines(void)
{

	int32_t key;
	int32_t prev_key;

	double min_val;
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
//			printf("%d\t%lf\n", key_max, local_max_val);
//			printf("%d\t%lf\n", key_min, local_min_val);

			/* Update to new key set */
			prev_key = key;
			PRINTF("=====================\n");
			PRINTF("lmin:%lf\tlmax:%lf\tmin:%lf\tmax:%lf\n", local_min_val, local_max_val, min_val, max_val);
//			local_min_val 	= min_val ;
//			local_max_val 	= max_val ;

		}
		else /* We have the first key set or are processing the same key set*/
		{

			/* Process each line separately */
			prev_key 		= key;
			PRINTF("=====================\n");
			PRINTF("lmin:%lf\tlmax:%lf\tmin:%lf\tmax:%lf\n", local_min_val, local_max_val, min_val, max_val);
			local_min_val 	= (min_val < local_min_val) ? min_val: local_min_val;
			local_max_val 	= (max_val > local_max_val) ? max_val: local_max_val;
		} /* End Else*/

	} /* End While */

	printf("%d\t%lf\n", key_max, local_max_val);
	printf("%d\t%lf\n", key_min, local_min_val);


}// End myMapLines

int main(void)
{
	myCollectLines();
	return 0;
}
