#include "Debug.h"

void Debug::Log(const char* file, const int line, const std::string& message, bool showLine)
{
	if (showLine) std::cout << message << ": " << file << ":" << line << std::endl;
	else std::cout << message << std::endl;
}