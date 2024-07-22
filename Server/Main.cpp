#include <iostream>
#include "include/Server.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	auto ChatServer = std::make_unique<Server>();
	ChatServer->Listen(L"0.0.0.0", 17000U);

	while (true) {
		ChatServer->AcceptClient();
	}
	
	std::cin.get();
}	