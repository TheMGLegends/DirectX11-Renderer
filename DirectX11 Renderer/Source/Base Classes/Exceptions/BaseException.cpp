#include "BaseException.h"

#include <sstream>

const char* BaseException::what() const
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginContext();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string BaseException::GetOriginContext() const
{
	std::ostringstream oss;
	oss << "File: " << file << std::endl << "Line: " << line;
	return oss.str();
}
