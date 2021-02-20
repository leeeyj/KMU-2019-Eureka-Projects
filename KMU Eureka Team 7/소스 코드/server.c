#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "LEA_core.h"
#include "LEA_core.c"
 BYTE pbUserKey[16] = { 0x0f, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x69, 0x78, 0x87, 0x96, 0xa5, 0xb4, 0xc3, 0xd2, 0xe1, 0xf0 };
 BYTE pdwRoundKey[384] = { 0x0, };
int main(int argc, char *argv[])
{
	int fd_socket, fd_accept;
	int clilen;
	struct sockaddr_in addr, cli_addr;
	char buf[100];
	char buffer[BUFSIZ];
	int i;
	if((fd_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("server: socket");
		return 1;

	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(atoi(argv[1]));
	if(bind(fd_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{	
		perror("server: bind");
		return 1;
	}
	if ( listen(fd_socket, 5) == -1)
	{
		perror("server: listen");
		return 1;
	}

	printf("server start !!!\n");
	clilen = sizeof(cli_addr);
	if((fd_accept = accept (fd_socket, (struct sockaddr *)&cli_addr, &clilen)) == -1)
	{
		perror("server: accept");
		return 1;
	}

	inet_ntop(AF_INET, &cli_addr.sin_addr, buf,sizeof(buf));
	printf("client address => %s\n", buf);
	
	LEA_Key(pbUserKey, pdwRoundKey);
	
	while(1){
		
		memset(buffer, 0x00, BUFSIZ);
	
	if(read(fd_accept, buffer, BUFSIZ) == -1)
	{
		perror("server: read");
		return 1;
	}

	//LEA_Key(pbUserKey, pdwRoundKey);
	LEA_Dec(pdwRoundKey, buffer);

	
	printf("Decrypt: %s\n", buffer);
	}
	
	close(fd_accept);
	close(fd_socket);
	return 0;
}
	
