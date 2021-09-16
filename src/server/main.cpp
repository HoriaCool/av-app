#include <iostream>
#include "utils/utils.hpp"

int main(int argc, char *argv[])
{
	std::cout << "Hello from server! args: " << argv[argc - 1] << "\n";

	ASSERT(1, "test assert");

	return 0;
}