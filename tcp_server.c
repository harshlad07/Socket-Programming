// TCP server code for chatting application 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 100
#define PORT 8080

// func to chat between client and server

void chat_func(int connfd)
{
	char buff[MAX];
	int flag = 1;

	printf("\n************ Server *************\n\n");

	while(1) 
	{
		while(flag == 1)
		{
			memset(buff, '\0', MAX);
			
			if((read(connfd, buff, MAX)) == -1)
			{
				printf("failed to read from client \n");
				close(connfd);
				exit(0);
			}

			if(strlen(buff) == 1)
			{
				flag = 0;
				break;
			}

			printf("\nClient : %s", buff);

		}

		while(flag == 0)
		{
			memset(buff, '\0', MAX);
			
			printf("\nServer : ");
			
			int i=0;
			while((buff[i++] = getchar()) != '\n')
				;

			if((write(connfd, buff, MAX)) == -1)
			{
				printf("failed to write \n");
				close(connfd);
				exit(0);
			}

			if(strlen(buff) == 1)
			{
				flag = 1;
			}
		}			
	}
}


int main()
{
	int sockfd, connfd;
	socklen_t len;	
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
		
	memset(&servaddr, '\0', sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// long integer from host byte order to network byte order.
	servaddr.sin_port = htons(PORT);	// short integer from host byte order to network byte order.

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// server listens
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("Server listening..\n");
		
	len = sizeof(cli);

	// Accept the data from client
	connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
	
	if (connfd < 0) {
		
		printf("server accept failed...\n");
		close(sockfd);
		exit(0);
	}
	else
		printf("server accepted the client...\n");

	// Function to chat between client and server
	chat_func(connfd);

	// After chatting close the socket
	close(sockfd);
}
