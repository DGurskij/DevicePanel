#pragma once
#include <ctime>
#include <fstream>
#include <chrono>

#define EVENT_ID_LENGTH 20

typedef std::chrono::system_clock CLOCK;
typedef std::chrono::milliseconds ms;
typedef std::chrono::seconds sec;

typedef struct StructEvent
{
	ms timestamp;
	char event_id[EVENT_ID_LENGTH];

	int p1;
	int p2;
	int p3;
} Event;

class Logger
{
protected:
	Logger(char* file_path);

	char* file_path;
	std::ofstream log_stream;

public:
	virtual void write(Event event) = 0;
	~Logger();
};
