#include "include/Server.h"
#include <thread>
#include <algorithm>
#include <iostream>
#include <utility>

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

	char Buffer[4096];
	int BytesReceived = recv(ClientSocket, Buffer, sizeof(Buffer), 0);

	if (BytesReceived <= 0) {
		int errorCode = WSAGetLastError();
		std::cout << "Client Connection Closed..." << std::endl;
		closesocket(ClientSocket);
	}

	std::string Name(Buffer, BytesReceived);
	ActiveClients.insert({ ClientSocket, Name });

	std::cout << "Connection Established with " << Name << "..." << std::endl;

	std::thread ReceiveThread(&Server::ReceiveMessages, this, ClientSocket);
	ReceiveThread.detach();

	return ClientSocket;
}

void Server::CloseClient(SOCKET Client) {
	closesocket(Client);
	auto it = ActiveClients.find(Client);
	if (it != ActiveClients.end()) {
		ActiveClients.erase(it);
	}
}

void Server::ReceiveMessages(SOCKET Client) {
	char Buffer[4096];

	while (true) {
		int BytesReceived = recv(Client, Buffer, sizeof(Buffer), 0);

		if (BytesReceived <= 0) {
			int errorCode = WSAGetLastError();
			std::cout << "Connection With " << ActiveClients[Client] << " is Closed..." << std::endl;
			CloseClient(Client);
			break;
		}

		std::string TempMessage(Buffer, BytesReceived);
		std::string Message = ActiveClients[Client] + " : " + TempMessage;
		std::cout<< Message <<std::endl;

		for (const auto& TempClient : ActiveClients) {
			if (TempClient.first != Client) {
				int ReturnCode = send(TempClient.first, Message.c_str(), Message.length(), 0);

				if (ReturnCode == SOCKET_ERROR) {
					std::cout << "Sending message to " + TempClient.second + "Failed...\n";
					std::cout << "Closing Connection With " + TempClient.second + "...\n";
					CloseClient(TempClient.first);
				}
			}
		}
	}
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
