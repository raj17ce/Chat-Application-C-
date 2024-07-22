#include "include/Server.h"
#include <iostream>

void Server::Listen(const WCHAR* IPAddress, u_short PORT) {
	CreateSocket();
	CreateSocketAddress(IPAddress, PORT);
	BindAddressToSocket();

	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Socket Listening failed..." << std::endl;
		CloseProgram();
	}

	std::cout << "Server Started Listening On Port : " << PORT << std::endl;
}

SOCKET Server::AcceptClient() {
	SOCKET ClientSocket = accept(Socket, nullptr, nullptr);

	if (ClientSocket == INVALID_SOCKET) {
		std::cout << "Accepting Client failed..." << std::endl;
		return -1;
	}

	//TO-Do : Get Client Name
	ActiveClients.emplace_back(ClientSocket);

	return ActiveClients.back();
}

void Server::CloseClient(SOCKET Client) {
	auto it = std::find(ActiveClients.begin(), ActiveClients.end(), Client);
	ActiveClients.erase(it);
}

bool Server::ReceiveMessage(SOCKET Client) {
	char Buffer[4096];
	int BytesReceived = recv(Client, Buffer, sizeof(Buffer), 0);
	std::cout<<"Bytes : " << BytesReceived << std::endl;
	if (BytesReceived < 0) {
		std::cout << "Error Receiving Message..."<<std::endl;
		return false;
	}

	std::string Message(Buffer, BytesReceived);
	std::cout<<Message<<std::endl;
	CloseProgram();
	return true;
}

void Server::CreateSocket() {
	Socket = socket(AF_INET, SOCK_STREAM, 0);

	if (Socket == INVALID_SOCKET) {
		std::cout << "ListenSocket Creation failed..." << std::endl;
		CloseProgram();
	}
}

void Server::CreateSocketAddress(const WCHAR* IPAddress, u_short PORT) {
	SocketAddress.sin_family = AF_INET;
	SocketAddress.sin_port = htons(PORT);

	//Filling IP Address
	if (InetPton(AF_INET, IPAddress, &SocketAddress.sin_addr) != 1) {
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
