#include "Emitter.h"
#include <chrono>
#include <string>
#include <iostream>
#include "../utils/Utils.h"

typedef std::chrono::system_clock CLOCK;
typedef std::chrono::milliseconds ms;

const char events[5][EVENT_ID_LENGTH] =
{
	"detect asteroid",
	"detect high X-ray",
	"fire",
	"oxygen leak",
	"fuel leak",
};

void emitterWorker(Panel* panel)
{
	srand((unsigned int)time(NULL));

	Event event = {};

	HANDLE pipe_wirte = panel->channel.pipe_wirte;
	DWORD bytes_written;

	do
	{
		if (panel->emitter_locked)
		{
			panel->emitter_lock->acquire(); // block thread until 'resume'
		}
		else
		{
			std::this_thread::sleep_for(ms(panel->event_spawn_time + random(0, 10)));

			event.timestamp = std::time(NULL);
			strcpy_s(event.event_id, EVENT_ID_LENGTH, events[random(0, 4)]);

			event.p1 = random(-1000, 1000);
			event.p2 = random(-1000, 1000);
			event.p3 = random(-1000, 1000);
			
			if (!WriteFile(pipe_wirte, &event, sizeof(Event), &bytes_written, NULL))
			{
				DWORD error_code = GetLastError();
				std::cerr << "Pipe write error[" << error_code << "]: " << std::system_category().message(error_code) << "\n";
			}
			else
			{
				panel->count_session_events++;
			}

		}
	} while (panel->panel_power);

	CloseHandle(panel->channel.pipe_wirte);
}

void pauseEmitter(Panel* panel, const char* params)
{
	if (!panel->emitter_locked)
	{
		panel->emitter_lock->try_acquire();
		panel->emitter_locked = true;
	}
}

void resumeEmitter(Panel* panel, const char* params)
{
	panel->emitter_locked = false;
	panel->emitter_lock->release();
}

void fasterEmitter(Panel* panel, const char* params)
{
	unsigned spawn_time = panel->event_spawn_time / 2;

	if (spawn_time < 1)
	{
		spawn_time = 1;
	}

	panel->event_spawn_time = spawn_time;
}

void slowerEmitter(Panel* panel, const char* params)
{
	panel->event_spawn_time *= 2;
}

void intervalEmitter(Panel* panel, const char* params)
{
	if (params == NULL)
	{
		std::cout << "Spawn time(ms): " << panel->event_spawn_time << "\n";
	}
	else
	{
		int spawn_time = atoi(params);

		if (spawn_time < 0 || spawn_time > 1000)
		{
			std::cerr << "Wrong emitter time '" << spawn_time << "' allow [0, 1000]\n";
		}

		panel->event_spawn_time = spawn_time;
	}
}