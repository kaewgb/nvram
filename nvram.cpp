#include <map>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>		// For open()
#include <unistd.h>		// For close() & sysconf()
#include <sys/mman.h>	// For mmap()
#include "nvram.h"

#define	NVRAM_PATH			"/media/PENDRIVE"

using namespace std;

// Library's global variable
static int filecount = 0;			// Count number of mapped/allocated variables -- is used to generage file names
map<void *, nvram_heap_t> db;		// Database: what address maps to what file and what is the size

enum error_messages {
	NVRAM_SUCCESS=1,
	NVRAM_FAILED
};

void init_timer(struct timeval *start){
	gettimeofday(start, NULL);
}

double get_collapsed_time(struct timeval *start){

	double seconds;
	struct timeval end;

	gettimeofday(&end, NULL);
	seconds = end.tv_sec - start->tv_sec;
	seconds += (double)(end.tv_usec - start->tv_usec)/1000000.0;

	*start = end;
	return seconds;
}

static inline long get_page_size(){
	return sysconf(_SC_PAGESIZE);
}

int create_file(char *filename, size_t size){
	FILE *f = fopen(filename, "w");
	if(f == NULL)
		return NVRAM_FAILED;

	fseek(f, size-1, SEEK_SET);
	fwrite((const void *) " ", sizeof(char), 1, f);
	fclose(f);
	return NVRAM_SUCCESS;
}

int nvram_alloc (
	void **ptr,		// Pointer to the newly allocated space
	size_t size		// Size of the allocation in bytes
){
	int fd;
	char filename[MAX_FILENAME_LENGTH];

	// Create file to be heap for the array
	sprintf(filename, "%s/nvram_var_%04d", NVRAM_PATH, filecount);
	if(create_file(filename, size) == NVRAM_FAILED)
		return NVRAM_FAILED;

	// Map the file to memory space
	if((fd = open(filename, O_RDWR)) == -1)
		return NVRAM_FAILED;

	*ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(*ptr == MAP_FAILED);
		return NVRAM_FAILED;
	close(fd);

	// Store in database
	nvram_heap_t temp;
	strcpy(temp.filename, filename);
	temp.size = size;
	db[*ptr] = temp;

	return NVRAM_SUCCESS;
}

int nvram_free(void *ptr){
	// TODO: Find the mapped size according to the address
	int alloc_size = db[ptr].size;
	if(munmap(ptr, alloc_size) == -1)
		return NVRAM_FAILED;
}

int nvram_put_nb(
	void *nvram_ptr, 	// Destionation (to the mapped nvram address)
	void *host_ptr, 	// Source (normal address)
	size_t size			// Size in bytes
){
	// TODO: check how to check if memcpy operation is successful (it returns destination addr)
	if(memcpy(nvram_ptr, host_ptr, size) != NULL)
		return NVRAM_SUCCESS;
}

int nvram_get_nb(
	void *host_ptr,		// Destination (normal address)
	void *nvram_ptr, 	// Source (nvram address)
	size_t size			// Size in bytes
){
	// TODO: check how to check if memcpy operation is successful (it returns destination addr)
	if(memcpy(host_ptr, nvram_ptr, size) != NULL)
		return NVRAM_SUCCESS;
}
