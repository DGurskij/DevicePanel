#include "LoggerLvlOne.h"

LoggerLvlOne::LoggerLvlOne(char* file_path) : Logger(file_path)
{

}

void LoggerLvlOne::write(Event event)
{
	char buf[60];
	char time_str[21];

	struct tm time;
	gmtime_s(&time, &event.timestamp);
	std::strftime(time_str, 21, "%F %T", &time);

	sprintf_s(buf, 60, "%s  %20s  %12d\n", time_str, event.event_id, event.p1);

	this->log_stream = std::ofstream(file_path, std::ios_base::app);
	this->log_stream << buf;
	this->log_stream.close();
}