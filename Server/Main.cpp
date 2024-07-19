#include <iostream>
#include "include/Server.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
	auto ChatServer = std::make_unique<Server>();
	ChatServer->Listen(17000U);
	std::cin.get();
}	