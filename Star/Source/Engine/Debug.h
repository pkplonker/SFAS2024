#pragma once
#include <iostream>
#include <string>

#define DEBUG(message) Debug::Log(__FILE__,__LINE__, message)

class Debug
{
public:

	static void Log(const char* file, const int line, const std::string& message)
	{
		std::cout << message << ": " << file << ":" << line << std::endl;
	}
};
