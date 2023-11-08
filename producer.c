#include "shared.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int main(int argc, char *argv[])
{
	int            fd;
	size_t         len;
	char		string[STRING_SIZE];
	struct shared  *sharedptr;

	fd = shm_open(mysharedmemory, O_RDWR, 0);
	if (fd == -1)
		errExit("shm_open");

	sharedptr = mmap(NULL, sizeof(*sharedptr), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (sharedptr == MAP_FAILED)
		errExit("mmap");
	

	sleep(0.75);

	while(1) {
		if (sem_wait(&sharedptr->sem) == -1)
				errExit("sem_wait");

		while(sharedptr->cnt < 2) {

			printf("please type a string to input into buffer (type /exit to exit program): ");

			scanf("%s", sharedptr->buf[sharedptr->cnt]);
			if(strcmp(sharedptr->buf[sharedptr->cnt],"/exit") == 0) {				

				sharedptr->close = 1;
				printf("%d",sharedptr->close);
				if(sem_post(&sharedptr->sem) == -1)
					errExit("sem_post");
				exit(EXIT_SUCCESS);
			}
			sharedptr->cnt++;
		}

		if (sem_post(&sharedptr->sem) == -1)
			errExit("sem_post");

	}

	exit(EXIT_SUCCESS);
}
