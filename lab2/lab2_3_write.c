// shmwrite.c
#include <sys/sem.h>
#include <sys/shm.h>

#define SHMSIZE 512 
union semun
{
  int val;
  struct semid_ds *buf;
  unsigned short *array;
};

int P(int);
int V(int);
int initSem(int);

void main() {
	// set up shared memory
	int shmId = shmget(1234, SHMSIZE, IPC_CREAT|0666);
	if(shmId == -1) {
		printf("shmget failed.\n");
		exit(1);
	}
	
	char *shmAddr;
	shmAddr = shmat(shmId, 0, 0);

	// get semaphore
	int semId = semget(5678, 1, IPC_CREAT|0666);

	while(1) {
		if(!P(semId))
			exit(1);
		printf("Write something: ");
		gets(shmAddr);
		sleep(2);

		if(!V(semId))
			exit(1);
	}
	
}

int P(int semId) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = -1;
	sb.sem_flg = SEM_UNDO;
	int flag = semop(semId, &sb, 1);
	if(flag == -1) {
		printf("Operation P failed.\n");
		return 0;
	}
	return 1;
}

int V(int semId) {
	struct sembuf sb;
	sb.sem_num = 0;
	sb.sem_op = 1;
	sb.sem_flg = SEM_UNDO;
	int flag = semop(semId, &sb, 1);
	if(flag == -1) {
		printf("Operation V failed.\n");
		return 0;
	}
	return 1;
}

int initSem(int semId) {
	union semun sem;
	sem.val = 1;
	int flag = semctl(semId, 0, SETVAL, sem);
	if(flag == -1)
		return 0;
	return 1;	
}
