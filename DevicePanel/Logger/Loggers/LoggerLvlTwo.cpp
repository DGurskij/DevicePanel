#include "LoggerLvlTwo.h"

LoggerLvlTwo::LoggerLvlTwo(char* file_path) : Logger(file_path)
{

}

void LoggerLvlTwo::write(Event event)
{
	char buf[90];
	char time_str[21];

	struct tm time;
	gmtime_s(&time, &event.timestamp);
	std::strftime(time_str, 21, "%F %T", &time);

	sprintf_s(buf, 90, "%s  %20s  %12d  %12d  %12d\n", time_str, event.event_id, event.p1, event.p2, event.p3);

	this->log_stream = std::ofstream(file_path, std::ios_base::app);
	this->log_stream << buf;
	this->log_stream.close();
}