// fifowrite.c
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
 
int main()
{
    const char *fifo_name = "/tmp/fifo";
    int pipe, in, res;
    char buffer[PIPE_BUF + 1];

    res = mkfifo(fifo_name, 0777);
    if (res != 0) {
    	printf("Create fifo failed.\n");
    	exit(1);
    }
 
    printf("Process %d, opening fifo...\n", getpid());
 
    pipe = open(fifo_name, O_WRONLY);
    in = open("in.txt", O_RDONLY);
 
    if (pipe != -1)
    {
        int bytes_read = 0;
 
        bytes_read = read(in, buffer, PIPE_BUF);
        buffer[bytes_read] = '\0';
        while (bytes_read > 0)
        {
            res = write(pipe, buffer, bytes_read);
            if (res == -1)
            {
                printf("Write to pipe failed.\n");
                exit(1);
            }
 
            bytes_read = read(in, buffer, PIPE_BUF);
        }
        close(pipe);
        close(in);
    } else {
        exit(1);
    }
 
    printf("Process %d finished.\n", getpid());
    exit(0);
}
