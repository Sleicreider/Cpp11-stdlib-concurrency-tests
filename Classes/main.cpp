#include <iostream>
#include "FileReader.h"

int main(int argc, char** argv)
{
	if (!argv[1]) return 1;

	FileReader f;
	if (!f.ReadFile(argv[1])) return 0;

	std::cout << "Hello World!" << std::endl;
	return 0;
}
