#include <Winsock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

constexpr auto PORT = 8080;
constexpr auto IP_SERV = "127.0.0.1";
constexpr short BUFF_SIZE = 1024;

SOCKET serverSocket;
SOCKET newRequest;
int wsaResult;
int bindResult;
int listenResult;
int request;
int respons;
char inputData;
int convertIp;

in_addr ipAddres;

sockaddr_in addr;

WSADATA data;

std::vector<char> buffServ(BUFF_SIZE);
std::vector<char> buffClient(BUFF_SIZE);

void socketSetup()
{
	wsaResult = WSAStartup(MAKEWORD(2, 2), &data);

	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	convertIp = inet_pton(AF_INET, IP_SERV, &ipAddres);

	addr.sin_family = AF_INET;
	addr.sin_addr = ipAddres;
	addr.sin_port = htons(PORT);

	bindResult = bind(serverSocket, (SOCKADDR*)&addr, sizeof(addr));

	listenResult = listen(serverSocket, 2);

	newRequest = accept(serverSocket, NULL, NULL);
}

int dataTransmission()
{
	while (true)
	{
		request = recv(newRequest, buffClient.data(), buffClient.size(), 0);
		std::cout << buffClient.data();

		fgets(buffServ.data(), buffServ.size(), stdin);

		if (buffServ[0] == 'O' && buffServ[1] == 'O' && buffServ[2] == 'O')
		{
			shutdown(newRequest, SD_BOTH);
			closesocket(newRequest);
			WSACleanup();
			return 0;
		}
		
		respons = send(newRequest, buffServ.data(), buffServ.size(), 0);
		
	}

}

int errorHandling()
{
	if (wsaResult != NO_ERROR)
	{
		std::cout << "Error with wsa " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (serverSocket == INVALID_SOCKET)
	{
		std::cout << "Error with create socket " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (convertIp <= 0)
	{
		std::cout << "Error with IP format" << std::endl;
		return 1;
	}
	if (bindResult == SOCKET_ERROR)
	{
		std::cout << "Error binding " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (listenResult == SOCKET_ERROR)
	{
		std::cout << "Error listening " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (newRequest == INVALID_SOCKET)
	{
		std::cout << "Error accept " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		return 1;
	}
	if (request == SOCKET_ERROR)
	{
		std::cout << "Error with recieve message from client " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(newRequest);
		WSACleanup();
		return 1;
	}
	if (respons == SOCKET_ERROR)
	{
		std::cout << "Error with send message to client " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(newRequest);
		WSACleanup();
		return 1;
	}

}

void run()
{
	socketSetup();
	dataTransmission();
	errorHandling();
}

int main()
{

	run();

	closesocket(newRequest);
	WSACleanup();
	return 0;
}