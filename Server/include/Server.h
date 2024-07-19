#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

class Server {
public:
	Server() : Socket{}, SocketAddress{} {
		Initialize();
	}

	~Server() {
		closesocket(Socket);
		CleanUp();
	}

	void Listen(u_short PORT);

private:
	SOCKET Socket;
	sockaddr_in SocketAddress;

	void CreateSocket();
	void CreateSocketAddress(u_short PORT);
	void BindAddressToSocket();

	void Initialize();
	void CleanUp();
	void CloseProgram();
};