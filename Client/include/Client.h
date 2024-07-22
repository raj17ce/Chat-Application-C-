#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>

class Client {
public:
	Client() : Socket{}, ServerAddress{} {
		Initialize();
	}

	~Client() {
		closesocket(Socket);
		CleanUp();
	}

	void SetSocket(SOCKET NewSocket);
	SOCKET GetSocket() const;

	void ConnectToServer(const WCHAR* IPAddress, u_short PORT);

	void SendMessages();
	void ReceiveMessages();

	friend bool operator==(const Client& Client1, const Client& Client2);
private:
	SOCKET Socket;
	sockaddr_in ServerAddress;

	void CreateSocket();
	void CreateServerAddress(const WCHAR* IPAddress, u_short PORT);

	void Initialize();
	void CleanUp();
	void CloseProgram();
};