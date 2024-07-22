#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <vector>
#include <string>
#include "include/Client.h"

class Server {
public:
	Server() : Socket{}, SocketAddress{}, ActiveClients{} {
		Initialize();
	}

	~Server() {
		closesocket(Socket);
		CleanUp();
	}

	void Listen(const WCHAR* IPAddress, u_short PORT);
	SOCKET AcceptClient();
	void CloseClient(SOCKET Client);
	bool ReceiveMessage(SOCKET Client);

private:
	SOCKET Socket;
	sockaddr_in SocketAddress;
	std::vector<SOCKET> ActiveClients;

	void CreateSocket();
	void CreateSocketAddress(const WCHAR* IPAddress, u_short PORT);
	void BindAddressToSocket();

	void Initialize();
	void CleanUp();
	void CloseProgram();
};