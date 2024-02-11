#pragma once
#include "../Logger.h"


class LoggerLvlZero : public Logger
{
public:
	LoggerLvlZero(char* file_path);
	void write(Event event);
};

