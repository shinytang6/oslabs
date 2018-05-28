#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAXSIZE 1024

int main(int argc, char **argv) {
	int sockfd, file, bytes;
	char buf[MAXSIZE];
	struct sockaddr_in serveAddr;

	if(argc != 2) {
		printf("Usage: ./client <ip_address>");
		exit(1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Creating socket error.\n");
		exit(1);
	}

	if(inet_pton(AF_INET, argv[1], &serveAddr.sin_addr) <= 0) {
		printf("inet_pton error for %s.\n", argv[1]);
		exit(1);
	}
	serveAddr.sin_family = AF_INET;
	serveAddr.sin_port = htons(1234);
	
	if(connect(sockfd, (struct sockaddr *) &serveAddr, sizeof(serveAddr)) < 0) {
		printf("Connect error.\n");
		exit(1);
	}

	printf("Sending file to server.\n");
	file = open("source.txt", O_RDONLY, 0644);

	while((bytes = read(file, buf, MAXSIZE)) > 0) {
		printf("%d\n", bytes);
		if(bytes < MAXSIZE)
			buf[bytes] = '\0';
		send(sockfd, buf, bytes, 0);
	}

	close(file);
	close(sockfd);
	return 0;
}
