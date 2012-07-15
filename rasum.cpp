/////////////////////////////////
// Random Access Sum Benchmark //
/////////////////////////////////

#include <stdio.h>
#include "nvram.h"
#include "random.h"

#define	MAX	1000000

int main(int argc, char *argv[]){
	int *a, *b, *order;
	int sum_a = 0, sum_b = 0;
	int i, j, n = MAX;
	struct timeval timer;

	init_random_int_array(&a, n, 100);
	init_random_int_array(&order, n, 1000);

	nvram_alloc((void **) &b, n*sizeof(int));
	nvram_put_nb(b, a, n*sizeof(int));

	for(i=0;i<1000;i++)
		sum_a += a[order[i]];
	sum_a = 0;
	init_timer(&timer);
	for(j=0;j<1000;j++)
		for(i=j;i<MAX;i+=1000)
		sum_a += a[i];
	printf("sum_a = %d\n", sum_a);
	printf("Normal: %lf secs\n", get_collapsed_time(&timer));

	for(i=0;i<1000;i++)
		sum_b += b[order[i]];
	sum_b = 0;
	init_timer(&timer);
	for(j=0;j<1000;j++)
		for(i=j;i<MAX;i+=1000)
			sum_b += b[i];
	printf("sum_b = %d\n", sum_b);
	printf("NVRAM: %lf secs\n", get_collapsed_time(&timer));

	nvram_free(b);

	return 0;
}
