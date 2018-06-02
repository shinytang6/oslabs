// fiforead.c
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
 
int main()
{
    const char *fifo_name = "/tmp/fifo";
    int pipe, in, res;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;
    int bytes_write = 0;
 
    printf("Process %d, opening fifo...\n", getpid());

    pipe = open(fifo_name, O_RDONLY);

    in = open("out.txt", O_WRONLY|O_CREAT, 0644);
 
    if (pipe != -1)
    {  
    	res = read(pipe, buffer, PIPE_BUF);
    	while (res > 0) {
		bytes_write = write(in, buffer, res);	
		if (bytes_write == -1) {
			printf("Write to out.txt failed.\n");
			exit(1);
		}	
		res = read(pipe, buffer, PIPE_BUF);
	}
        close(pipe);
        close(in);
    }
    else
    {
        exit(1);
    }
 
    printf("Process %d finished.\n", getpid());
    exit(0);
}
