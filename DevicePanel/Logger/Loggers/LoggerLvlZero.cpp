#include "LoggerLvlZero.h"

LoggerLvlZero::LoggerLvlZero(char* file_path) : Logger(file_path)
{

}

void LoggerLvlZero::write(Event event)
{
	char buf[45];
	char time_str[21];

	struct tm time;
	gmtime_s(&time, &event.timestamp);
	std::strftime(time_str, 21, "%F %T", &time);

	sprintf_s(buf, 45, "%s  %20s\n", time_str, event.event_id);

	this->log_stream = std::ofstream(file_path, std::ios_base::app);
	this->log_stream << buf;
	this->log_stream.close();
}