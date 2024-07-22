#include <iostream>
#include <string>
#include "include/Client.h"

void Client::SetSocket(SOCKET NewSocket) {
	Socket = NewSocket;
}

SOCKET Client::GetSocket() const {
	return Socket;
}

void Client::ConnectToServer(const WCHAR* IPAddress, u_short PORT) {
	CreateSocket();
	CreateServerAddress(IPAddress, PORT);

	if (connect(Socket, reinterpret_cast<sockaddr*>(&ServerAddress), sizeof(ServerAddress)) == SOCKET_ERROR) {
		std::cout << "Server Connection failed..." << std::endl;
		CloseProgram();
	}

	std::cout << "Successfully Connected to Server on PORT : " << PORT << std::endl;

	std::string Message;
	std::cout << "Enter Your Chat Name : ";
	std::getline(std::cin, Message);

	int ReturnCode = send(Socket, Message.c_str(), Message.length(), 0);

	if (ReturnCode == SOCKET_ERROR) {
		std::cout << "Message Sending Failed...\n";
		closesocket(Socket);
		CleanUp();
	}
}

void Client::SendMessages() {
	std::string Message;
	while (true) {
		std::getline(std::cin, Message);

		if (Message == "/quit") {
			break;
		}

		int ReturnCode = send(Socket, Message.c_str(), Message.length(), 0);

		if (ReturnCode == SOCKET_ERROR) {
			std::cout << "Message Sending Failed...\n";
			break;
		}
	}
	closesocket(Socket);
	CleanUp();
}

void Client::ReceiveMessages() {
	char Buffer[4096];

	while (true) {
		int BytesReceived = recv(Socket, Buffer, sizeof(Buffer), 0);

		if (BytesReceived <= 0) {
			std::cout << "Error Receiving Message..." << std::endl;
			break;
		}

		std::string Message(Buffer, BytesReceived);
		std::cout << Message << std::endl;
	}
	closesocket(Socket);
	CleanUp();
}

void Client::CreateSocket() {
	Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (Socket == INVALID_SOCKET) {
		std::cout << "Socket Creation failed..." << std::endl;
		CloseProgram();
	}
}

void Client::CreateServerAddress(const WCHAR* IPAddress, u_short PORT) {
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(PORT);

	//Filling IP Address
	if (InetPton(AF_INET, IPAddress, &ServerAddress.sin_addr) != 1) {
		std::cout << "Setting IP Address failed..." << std::endl;
		CloseProgram();
	}
}

void Client::Initialize() {
	WSADATA Data;
	if (WSAStartup(MAKEWORD(2, 2), &Data) != 0) {
		std::cout << "Client Initialization failed..." << std::endl;
		std::cout << "Closing the program..." << std::endl;
		std::exit(0);
	};
}

void Client::CleanUp() {
	WSACleanup();
}

void Client::CloseProgram() {
	std::cout << "Closing the program..." << std::endl;
	closesocket(Socket);
	CleanUp();
	std::exit(0);
}

bool operator==(const Client& Client1, const Client& Client2) {
	return (Client1.GetSocket() == Client2.GetSocket());
}