#include "LoggerFactory.h"
#include "Loggers//LoggerLvlZero.h"
#include "Loggers//LoggerLvlOne.h"
#include "Loggers//LoggerLvlTwo.h"

Logger* LoggerFactory::createLogger(int level, char* file_path)
{
	Logger* logger;

	switch (level)
	{
	case 1:
		logger = new LoggerLvlOne(file_path);
		break;
	case 2:
		logger = new LoggerLvlTwo(file_path);
		break;
	default:
		logger = new LoggerLvlZero(file_path);
		break;
	}

	return logger;
}