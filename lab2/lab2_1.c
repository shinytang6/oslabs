#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
void main() {  
	int fpid, ret, status;
	void doSomething();
	signal(SIGUSR1, doSomething);
	fpid = fork();
	if (fpid < 0 )
		printf("Error in fork.\n");
	else if (fpid > 0) {
		printf("I am father process.\n");
		printf("My parent process id is %d\n", getppid());
		printf("My process id is %d\n", getpid());
		printf("My child process id is %d\n", fpid);
		printf("\n");
		kill(fpid,SIGUSR1);
	  
		fpid = wait( &status);
		printf( "wait() return child process id = %d and status = %d\n", fpid, status );  
	}
	else {
		sleep(2);
		printf("I am child process.\n");
		printf("My parent process id is %d\n", getppid());
		printf("My process id is %d\n", getpid());
		printf("My child process id is %d\n", fpid);
		printf("\n");
		ret = execl("/bin/ls","ls");  
		if(ret == -1)
			printf("Execl error.\n");
		exit(1);
	} 
}  
void doSomething() {
	system("xeyes");
}
