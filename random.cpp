#include <stdlib.h>
#include <time.h>

//
// This simple lib is just for testing and is NOT thread-safe
//

void init_random_int_array(int **intarray, int n, int max_value){
	int i, *array;
	time_t t;
	srand((unsigned) time(&t));

	array = (int *) malloc(n*sizeof(int));
	for(i=0;i<n;i++)
		array[i] = rand()%max_value;

	*intarray = array;
}

