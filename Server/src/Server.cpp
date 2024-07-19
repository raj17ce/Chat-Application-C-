#include "include/Server.h"
#include <iostream>

void Server::Listen(u_short PORT) {
	CreateSocket();
	CreateSocketAddress(PORT);
	BindAddressToSocket();

	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Socket Listening failed..." << std::endl;
		CloseProgram();
	}

	std::cout << "Server Started Listening On Port : " << PORT << std::endl;
}

void Server::CreateSocket() {
	Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (Socket == INVALID_SOCKET) {
		std::cout << "ListenSocket Creation failed..." << std::endl;
		CloseProgram();
	}
}

void Server::CreateSocketAddress(u_short PORT) {
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(PORT);

	//Filling IP Address
	if (InetPton(AF_INET, _T("0.0.0.0"), &SocketAddress.sin_addr) != 1) {
		std::cout << "Setting IP Address failed..." << std::endl;
		CloseProgram();
	}
}

void Server::BindAddressToSocket() {
	if (bind(Socket, reinterpret_cast<sockaddr*>(&SocketAddress), sizeof(SocketAddress)) == SOCKET_ERROR) {
		std::cout << "Binding ServerAddress to ListenSocket failed..." << std::endl;
		CloseProgram();
	}
}

void Server::Initialize() {
	WSADATA Data;
	if (WSAStartup(MAKEWORD(2, 2), &Data) != 0) {
		std::cout << "Server Initialization failed..." << std::endl;
		std::cout << "Closing the program..." << std::endl;
		std::exit(0);
	};
}

void Server::CleanUp() {
	WSACleanup();
}

void Server::CloseProgram() {
	std::cout << "Closing the program..." << std::endl;
	closesocket(Socket);
	CleanUp();
	std::exit(0);
}
