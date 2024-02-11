#include "LoggerLvlOne.h"

LoggerLvlOne::LoggerLvlOne(char* file_path) : Logger(file_path)
{

}

void LoggerLvlOne::write(Event event)
{
	char buf[62];
	char time_str[21];

	struct tm time;
	std::time_t t = std::chrono::duration_cast<sec>(event.timestamp).count();
	gmtime_s(&time, &t);
	std::strftime(time_str, 21, "%F %T", &time);

	sprintf_s(buf, 62, "%s.%03dZ  %20s  %12d\n", time_str, (int)(event.timestamp.count() % 1000), event.event_id, event.p1);

	this->log_stream = std::ofstream(file_path, std::ios_base::app);
	this->log_stream << buf;
	this->log_stream.close();
}