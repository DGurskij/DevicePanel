#pragma once
#include <semaphore>
#include "Logger/Logger.h"
#include <Windows.h>

typedef struct StructChannel
{
	HANDLE pipe_wirte;
	HANDLE pipe_read;
} Channel;

/*
	Device panel configuration
*/
typedef struct StructPanel
{
	/*
	* Common configuration
	*/
	bool panel_power;

	/*
	* Logger part
	*/
	char* log_file_path;
	Logger* logger;
	Logger** loggers;

	/*
	* Event emitter part
	*/
	bool emitter_locked;
	std::binary_semaphore* emitter_lock;

	// min time between spawn event (ms)
	unsigned event_spawn_time;
	unsigned count_session_events;
	unsigned count_logged_events;

	Channel channel;
	std::binary_semaphore* channel_sync;
} Panel;
