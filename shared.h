#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
} while (0)

#define STRING_SIZE 1024   /* Maximum size for exchanged string */
#define BUFFER_SIZE 2

/* Define a structure that will be imposed on the shared
   memory object */
const char * mysharedmemory = "/mysharedmemory";

struct shared {
	int close;
	sem_t  sem;            /* POSIX unnamed semaphore */
	size_t cnt;
	char   buf[BUFFER_SIZE][STRING_SIZE];   /* Data being transferred */
};
