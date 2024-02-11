#pragma once
#include "../Logger.h"


class LoggerLvlOne : public Logger
{
public:
	LoggerLvlOne(char* file_path);
	void write(Event event);
};

