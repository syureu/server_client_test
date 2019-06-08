/*
가장 기초적인 tcp 블로킹 에코소켓이다.
다음 예제에서는 한번의 recv에 send가 일어나는 부분을 수정할 것이다.
recv 한번에 모든 데이터를 받았는지 확실하지 않기 때문이다.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	char message[BUF_SIZE];
	int strLength;
	SOCKADDR_IN servAddr, clntAddr;
	int clntAddrSize;
	int portNumber;

	/*
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	*/
	printf("input port number : ");
	scanf("%d", &portNumber);

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(portNumber);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR) ErrorHandling("listen() error");

	clntAddrSize = sizeof(clntAddr);

	for (int i = 0; i < 5; i++) {
		hClntSock = accept(hServSock, (SOCKADDR*)& clntAddr, &clntAddrSize);
		if (hClntSock == -1) ErrorHandling("accept() error");
		else printf("Connectd client %d \n", i + 1);
		while ((strLength = recv(hClntSock, message, BUF_SIZE, 0)) != 0) send(hClntSock, message, strLength, 0);
		closesocket(hClntSock);
	}
	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}