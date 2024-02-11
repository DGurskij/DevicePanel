#include "Logger.h"
#include <iostream>

Logger::Logger(char* file_path)
{
	this->file_path = file_path;
}

Logger::~Logger()
{
}
