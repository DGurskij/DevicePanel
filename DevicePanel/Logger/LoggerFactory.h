#pragma once
#include "Logger.h"

class LoggerFactory
{
public:
	// Create logger by level - [0; 2], auto set 0 for wrong levels
	static Logger* createLogger(int level, char* file_path);
};