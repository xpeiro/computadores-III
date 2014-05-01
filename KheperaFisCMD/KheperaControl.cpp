
#include <string.h>
#include <stdio.h>


#ifdef _WIN32
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")
#endif

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
void closesocket(int socket) { close(socket); }
#endif

#define KP_PLACE 'P'
#define KP_INIT 'I'

int main() {

	#ifdef _WIN32
	WSADATA wsaData;
	WSAStartup(0x0202, &wsaData);
	typedef int socklen_t;
	typedef SSIZE_T ssize_t;
	#endif

	int socketfd, newsocket;	
	struct sockaddr_in serv_addr, cli_addr;
	socklen_t clilen;
	char order = 0;
	char message[1];
	int port=12000;

	socketfd = socket(AF_INET, SOCK_STREAM, 0);

	printf("Bienvenido a la prueba de conexions clientes \n\n");
	printf("Waiting for connection on port : %d \n", port);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	const int       optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &optVal, optLen);

	bind(socketfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	listen(socketfd, 5);

	newsocket = accept(socketfd, (struct sockaddr *) &cli_addr, &clilen);	
	printf("\nConnected\n");


	printf("\nInput command:\n 1-Init Kephera.\n");
	printf("\n 2-Move Kephera.\n");

	fflush(stdin);
	scanf("%c", &order);

	if (order == '1') {
		message[0] = KP_INIT;
		printf("KP_INIT\n");
	} else if (order == '2') {
		message[0] = KP_PLACE;
		printf("KP_PLACE\n");
	}

	ssize_t bytessent = send(newsocket, message, strlen(message), 0);

	closesocket(socketfd);
	closesocket(newsocket);

	printf("Bytes sent: %i \n", bytessent);

}

