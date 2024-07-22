#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <map>
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
	void ReceiveMessages(SOCKET Client);

private:
	SOCKET Socket;
	sockaddr_in SocketAddress;
	std::map<SOCKET, std::string> ActiveClients;

	void CreateSocket();
	void CreateSocketAddress(const WCHAR* IPAddress, u_short PORT);
	void BindAddressToSocket();

	void Initialize();
	void CleanUp();
	void CloseProgram();
};