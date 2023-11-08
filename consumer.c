#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#include "shared.h"

int main(int argc, char *argv[]) {
	int fd;
	struct shared *sharedptr;	

	fd = shm_open(mysharedmemory, O_EXCL | O_CREAT | O_RDWR, 0666);
	if(fd == -1)
		errExit("shm_open");

	if(ftruncate(fd, sizeof(struct shared)) == -1)
		errExit("ftruncate");


	sharedptr = mmap(NULL, sizeof(*sharedptr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (sharedptr == MAP_FAILED)
		errExit("mmap");

	memset(sharedptr, 0, sizeof(*sharedptr));

	if (sem_init(&sharedptr->sem, 1, 1) == -1)
		errExit("sem_init-cons");

	while(1) {
		if (sem_wait(&sharedptr->sem) == -1)
			errExit("sem_wait");
		
		if(sharedptr->close == 1)
		{
			shm_unlink(mysharedmemory);
			exit(EXIT_SUCCESS);	
		}	
		for (size_t j = 0; j < sharedptr->cnt; j++) {	
			
			printf("print string %ld: ", j);
			puts(sharedptr->buf[j]);
			sleep(1);
			
		}
		
		sharedptr->cnt = 0;

		if (sem_post(&sharedptr->sem) == -1)
			errExit("sem_post");
	}

}


