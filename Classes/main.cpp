//Project uses C++17 and was build via CMake
//Tested on Windows msvc
//Also compiles for gcc & clang https://godbolt.org/z/cfmtpT
//Doesn't compile on OSX since XCode still doesn't support <filesystem>, use boost::filesytem as alternative instead

#include <iostream>
#include "FileReader.h"

int main(int argc, char** argv)
{
	if (!argv[1])
	{
		std::cerr << "Requires one argument for a file to process" << std::endl;
		return 1;
	}

	FileReader f;
	if (!f.Process(argv[1])) return 1;

	f.PrintResult();

	return 0;
}
