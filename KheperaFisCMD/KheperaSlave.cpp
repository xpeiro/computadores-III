
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h> 
void closesocket(int socket) { close(socket); }
#endif

#define KP_INIT 'I'
#define KP_PLACE 'P'

int main()
{
	struct sockaddr_in serv_addr;
    struct hostent *server;
    int sockfd, port=12000;
    char IP[15];
	char buffer[1];	

	#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	typedef SSIZE_T ssize_t;
	#endif	
	
	printf("Introduzca una direccion IP\n");
	scanf("%s", &IP);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	server = gethostbyname(IP);
	memcpy((char *) &serv_addr.sin_addr.s_addr, (char *) server->h_addr, server->h_length);
	serv_addr.sin_port = htons(port);	

	printf("Connecting...\n");
	connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr));
	printf("Connected\nRecieving... \n");
	
	ssize_t bytesrec = recv(sockfd, buffer, 2, 0);

	printf("Recieved bytes: %i\n", bytesrec);
	if (buffer[0]==KP_INIT) printf("Init Kephera\n");
	else if (buffer[0]==KP_PLACE)	printf("Place Kephera\n");		
	
	closesocket(sockfd);

}

	




