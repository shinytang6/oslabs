#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    ssize_t pip_data = 0;
    int filedesc[2];
    char buffer[BUFSIZ+1];
    pid_t pid;
	int in, out, flag;

	in = open("in.txt", O_RDONLY, S_IRUSR);
	out = open("out.txt", O_WRONLY|O_CREAT);
    if(pipe(filedesc) == 0) {
        pid = fork();
        if(pid == -1) {
            printf("Fork failed.\n");
            exit(1);
        }
 
        if(pid == 0) {
            
            pip_data = read(filedesc[0], buffer, BUFSIZ);
            write(out, buffer, pip_data);
			printf("Read %ld bytes: %s\n", pip_data, buffer);
            exit(0);
        } else {
			flag = read(in, buffer, 1024);
            pip_data = write(filedesc[1], buffer, flag);
            printf("Wrote %ld bytes: %s\n", pip_data, buffer);
 
            sleep(2);
            exit(0);
        }
    } else {
        printf("Create pipe failed.\n");
        exit(1);
    }
}
