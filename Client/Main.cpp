#include <iostream>
#include <string>
#include <thread>
#include "include/Client.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	Client ChatClient{};

	std::wstring AddressInput;
	std::cout << "Enter IP Address of the Server: ";
	std::getline(std::wcin, AddressInput);
	const wchar_t* WCharAddress = AddressInput.c_str();

	ChatClient.ConnectToServer(WCharAddress, 17000U);
	
	std::thread SendThread{ &Client::SendMessages, &ChatClient };
	std::thread ReceiveThread{ &Client::ReceiveMessages, &ChatClient };

	SendThread.join();
	ReceiveThread.join();
	std::cin.get();
}