#include "TCPServer.hpp"


int main()
{
	TCPServer server;

	server.init();
	server.run();

	return 0;
}