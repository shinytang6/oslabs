#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <fcntl.h>

#define MAXSIZE 1024

int main() {
	int sockfd, newfd;
	int file, bytes;
	char buf[MAXSIZE];
	struct sockaddr_in serveAddr;
	struct sockaddr_in clientAddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1) {
		printf("Creatin socket error.\n");
		exit(1);
	}

	serveAddr.sin_family = AF_INET;
	serveAddr.sin_addr.s_addr = INADDR_ANY;
	serveAddr.sin_port = htons(1234);

	if(bind(sockfd, (struct sockaddr *) &serveAddr, sizeof(serveAddr)) == -1) {
		printf("Binding socket error.\n");
		exit(1);
	}

	if(listen(sockfd, 10) == -1) {
		printf("Listening error");
		exit(1);
	}

	printf("Waiting for client requests...\n");

	while(1) {
		if((newfd = accept(sockfd, (struct sockaddr *)NULL, NULL)) == -1) {
			printf("Accepting client error.\n");
			continue;
		}

	//	printf("Received a connection from %s", inet_ntoa(clientAddr.sin_addr));

		file = open("target.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
		while((bytes = recv(newfd, buf, MAXSIZE, 0)) > 0)
			write(file, buf, bytes);

		close(newfd);
		close(file);

		printf("Receiving file successfully.\n");
	}

	close(sockfd);


}
