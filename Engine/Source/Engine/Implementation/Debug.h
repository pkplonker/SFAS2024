#pragma once
#include <iostream>
#include <string>

#define Debug(message) Debug::Log(__FILE__,__LINE__, message, false);
#define DebugLine(message) Debug::Log(__FILE__,__LINE__, message, true);

class Debug
{
public:

	static void Log(const char* file, const int line, const std::string& message, bool showLine);
};
