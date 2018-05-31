#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define MAXSIZE 1024

int copy(int method, int size) {
	clock_t start, end;
	FILE *infs, *outfs;
	int in, out, i;
	char buf[MAXSIZE];

	//start = time(NULL);
	start = clock();
	if(method == 1) {
		in = open("source.txt", O_RDONLY, S_IRUSR);
		out = open("target.txt",O_WRONLY|O_CREAT);
		if(in == -1 || out == -1) {
			printf("Open file failed.\n");
			return 0;
		}

		while(read(in, buf, size) > 0)
			write(out, buf, size);
		close(in);
		close(out);
	} else {
		infs = fopen("source.txt", "at+");
		outfs = fopen("target.txt", "at+");
		if(infs == NULL || outfs == NULL) {
			printf("Open file failed.\n");
			return 0;
		}

		while(fread(buf, size * sizeof(char), 1, infs) > 0)
			fwrite(buf, size * sizeof(char), 1, outfs);
	
		close(infs);
		close(outfs);
	}

	end = clock();
	//end = time(NULL);
	char *operation = method ? "fread/fwrite" : "read/write";
	printf("Operation %s, %d bytes each time, time %dms.\n", operation, size, (int)(end - start));

	return 1;
}

int main() {

	copy(0, 1);
	copy(1, 1);

	copy(0, 1024);
	copy(1, 1024);

	return 0;

}
