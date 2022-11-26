#include <Winsock2.h>
#include <WS2tcpip.h>
#include <vector>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

constexpr auto PORT = 8080;
constexpr auto IP_SERV = "127.0.0.1";
constexpr short BUFF_SIZE = 1024;

SOCKET clientSocket;
int wsaResult;
int connectionToServ;
int convertIp;
char inputData;
int request;
int respons;

in_addr ipAddres;

sockaddr_in addr;

WSADATA data;

std::vector<char> buffClient(BUFF_SIZE);
std::vector<char> buffServ(BUFF_SIZE);

void setupClient()
{
	wsaResult = WSAStartup(MAKEWORD(2, 2), &data);

	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	convertIp = inet_pton(AF_INET, IP_SERV, &ipAddres);

	addr.sin_family = AF_INET;
	addr.sin_addr = ipAddres;
	addr.sin_port = htons(PORT);

	connectionToServ = connect(clientSocket, (SOCKADDR*) &addr, sizeof(addr));
}

int dataTransmission()
{
	while (true)
	{

		fgets(buffClient.data(), buffClient.size(), stdin);

		if (buffClient[0] == 'O' && buffClient[1] == 'O' && buffClient[2] == 'O')
		{
			shutdown(clientSocket, SD_BOTH);
			closesocket(clientSocket);
			WSACleanup();
			return 0;
		}
			
		respons = send(clientSocket, buffClient.data(), buffClient.size(), 0);

		request = recv(clientSocket, buffServ.data(), buffServ.size(), 0);
		std::cout << buffServ.data();

	}

}

int errorHandling()
{
	if (wsaResult != NO_ERROR)
	{
		std::cout << "Error with wsa " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "Error with create socket " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	if (convertIp <= 0)
	{
		std::cout << "Error with IP format" << std::endl;
		return 1;
	}
	if (connectionToServ == SOCKET_ERROR)
	{
		std::cout << "Error with connection to server " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	if (request == SOCKET_ERROR)
	{
		std::cout << "Error with recieve message from server " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	if (respons == SOCKET_ERROR)
	{
		std::cout << "Error with send message to server " << "--[" << WSAGetLastError() << "]--" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	
}

void run()
{
	setupClient();
	dataTransmission();
	errorHandling();
}

int main()
{
	run();

	closesocket(clientSocket);
	WSACleanup();
	return 0;
}