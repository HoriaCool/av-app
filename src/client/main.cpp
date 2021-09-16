#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Hello from client! args: " << argv[argc - 1] << "\n";

	return 0;
}