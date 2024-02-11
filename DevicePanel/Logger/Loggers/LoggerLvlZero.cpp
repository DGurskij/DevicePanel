#include "LoggerLvlZero.h"

LoggerLvlZero::LoggerLvlZero(char* file_path) : Logger(file_path)
{

}

void LoggerLvlZero::write(Event event)
{
	char buf[50];
	char time_str[21];

	struct tm time;
	std::time_t t = std::chrono::duration_cast<sec>(event.timestamp).count();
	gmtime_s(&time, &t);
	std::strftime(time_str, 21, "%F %T", &time);

	sprintf_s(buf, 50, "%s.%03dZ  %20s\n", time_str, (int)(event.timestamp.count() % 1000), event.event_id);

	this->log_stream = std::ofstream(file_path, std::ios_base::app);
	this->log_stream << buf;
	this->log_stream.close();
}