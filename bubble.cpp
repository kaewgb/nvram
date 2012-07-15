#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "nvram.h"

#define	MAX	100000

void swap(int &a, int &b){
	int c = a;
	a = b;
	b = c;
}
void print_sample(int *array){
	int i;
	for(i=0;i<10;i++)
		printf("%d ", array[i]);
	printf("\n");
}
int main(int argc, char *argv[]){
	int *num, *a, *b;
	time_t t;
	int i, n = MAX;
	bool sorted = false;
	struct timeval timer;

	// Init array
	num = (int *) malloc(n*sizeof(int));
	srand((unsigned) time(&t));
	for(i=0;i<n;i++)
		num[i] = rand()%1000000;
	print_sample(num);
	printf("Sorting...\n");

	// Normal memory
	a = (int *) malloc(n*sizeof(int));
	memcpy(a, num, n*sizeof(int));
	init_timer(&timer);
	while(!sorted){
		sorted = true;
		for(i=1;i<n;i++){
			if(a[i] < a[i-1]){
				swap(a[i], a[i-1]);
				sorted = false;
			}
		}
	}
	printf("Normal memory: %lf seconds\n", get_collapsed_time(&timer));
	print_sample(a);
	free(a);

	// Flash memory
	nvram_alloc((void **) &b, n*sizeof(int));
	nvram_put_nb(b, num, n*sizeof(int));
	init_timer(&timer);
	sorted = false;
	while(!sorted){
		sorted = true;
		for(i=1;i<n;i++){
			if(b[i] < b[i-1]){
				swap(b[i], b[i-1]);
				sorted = false;
			}
		}
	}
	printf("NVRAM memory: %lf seconds\n", get_collapsed_time(&timer));
	print_sample(b);
	nvram_free(b);
	return 0;

}
