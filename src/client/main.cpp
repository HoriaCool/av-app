#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void send_file(FILE *fp, int sockfd){
	char data[SIZE] = {0};
	size_t n_read;

	while((n_read = fread(data, 1, SIZE, fp)) != 0) {
		if (send(sockfd, data, n_read, 0) == -1) {
			perror("[-]Error in sending file.");
			exit(1);
		}
		bzero(data, SIZE);
  	}
}

int main(int argc, char *argv[]) {
	const char *ip = "127.0.0.1";
	int port = 7000;
	int e;

	int sockfd;
	struct sockaddr_in server_addr;
	FILE *fp;
	char *filename = argv[argc - 1];

	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Server socket created successfully.\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip);

	e = connect(sockfd, (struct sockaddr*)& server_addr, sizeof(server_addr));
	if(e == -1) {
		perror("[-]Error in socket");
		exit(1);
	}
	printf("[+]Connected to Server.\n");

	fp = fopen(filename, "r");
	if (fp == NULL) {
		perror("[-]Error in reading file.");
		exit(1);
	}

	send_file(fp, sockfd);
	printf("[+]File data sent successfully.\n");

	printf("[+]Closing the connection.\n");
	close(sockfd);

	fclose(fp);

	return 0;
}