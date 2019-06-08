/*
���� �������� tcp ���ŷ ���ڼ����̴�.
���� ���������� �ѹ��� recv�� send�� �Ͼ�� �κ��� ������ ���̴�.
recv �ѹ��� ��� �����͸� �޾Ҵ��� Ȯ������ �ʱ� �����̴�.
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