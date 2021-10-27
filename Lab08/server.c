#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define PortNumber 5555

int main(int argc, char *argv[]) {
	struct sockaddr_in address;
	int sock, res_size, address_len = sizeof(address);
	char res[50];

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock < 0) {
		printf("Error: cannot create socket");
		exit(EXIT_FAILURE);
	}

	bzero(&address, address_len);
	address.sin_family = AF_INET;
	address.sin_port = htons(PortNumber);
	address.sin_addr.s_addr = INADDR_ANY;
	if(bind(sock, (struct sockaddr*)&address, address_len) == -1) {
		printf("Error: bind file");
		exit(EXIT_FAILURE);
	}
	printf("ready:\n");
	if(res_size = recvfrom(sock, res, sizeof(res), 0, (struct sockaddr*)&address, &address_len)) {
		if(res_size < 0) {
			printf("Error: recving");
			exit(EXIT_FAILURE);
		}
		else {
			printf("client: %s\n", res);
			char msg[50] = "server have received the message.";
			int msg_size = sendto(sock, msg, 50, 0, (struct sockaddr*)&address, address_len);
		}
	
	}
	
	return 0;
}
