#ifndef	NVRAM_HEADER
#define	NVRAM_HEADER

//
// Include files
//
#include <sys/time.h>	// For gettimeofday()

//
// Constants
//
#define	MAX_FILENAME_LENGTH	250
#define	ERRX(str){		\
	perror(str);		\
	exit(1);			\
}

//
// Data structure
//
typedef struct _nvram_heap{
	char filename[MAX_FILENAME_LENGTH];
	size_t size;
} nvram_heap_t;

//
// Functions
//
int nvram_alloc (
	void **ptr,		// Pointer to the newly allocated space
	size_t size	// Size of the allocation in bytes
);
int nvram_free(void *ptr);
int nvram_put_nb(
	void *nvram_ptr, 	// Destionation (to the mapped nvram address)
	void *host_ptr, 	// Source (normal address)
	size_t size			// Size in bytes
);
int nvram_get_nb(
	void *host_ptr,		// Destination (normal address)
	void *nvram_ptr, 	// Source (nvram address)
	size_t size			// Size in bytes
);

void init_timer(struct timeval *start);
double get_collapsed_time(struct timeval *start);

#endif
