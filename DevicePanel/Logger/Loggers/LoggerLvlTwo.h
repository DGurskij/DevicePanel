#pragma once
#include "../Logger.h"


class LoggerLvlTwo : public Logger
{
public:
	LoggerLvlTwo(char* file_path);
	void write(Event event);
};

