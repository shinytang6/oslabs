// shmread.c
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
void delSem(int);

void main(int argc, char *argv[]) {
	int i;
	// set up shared memory
	int shmId = shmget(1234, SHMSIZE, IPC_CREAT|0666);
	if(shmId == -1) {
		printf("shmget failed.\n");
		exit(1);
	}
	
	char *shmAddr;
	shmAddr = shmat(shmId, 0, 0);

	// set up semaphore
	int semId = semget(5678, 1, IPC_CREAT|0666);
	if(!initSem(semId)) {
		printf("Initialize semaphore failed.\n");
		exit(1);
	}

	printf("semId: %d\n", semId);
	for (i = 0; i < 10; ++i) {
		if(!P(semId))
			exit(1);
		printf("Read the content: %s\n", shmAddr);
		sleep(2);
		printf("Read the content: %s\n", shmAddr);
		if(!V(semId))
			exit(1);
	}

	delSem(semId);
	
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

void delSem(int semId) {
	union semun sem;
	if (semctl(semId, 0, IPC_RMID, sem) == -1)
	{
		printf("Delete semaphore failed\n");
	}
}
