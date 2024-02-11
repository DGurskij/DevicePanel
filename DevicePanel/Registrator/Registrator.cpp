#include "Registrator.h"
#include <iostream>
#include "../utils/Utils.h"

void registratorWorker(Panel* panel)
{
	Event event = {};

	HANDLE pipe_read = panel->channel.pipe_read;
	DWORD bytes_read;

	do
	{
		if (!panel->emitter_locked)
		{
			panel->channel_sync->try_acquire(); // lock channel shutdown until log events
		}

		do
		{
			Sleep(random(1, 20)); // emulate high load
			if (!ReadFile(pipe_read, &event, sizeof(Event), &bytes_read, NULL))
			{
				DWORD error_code = GetLastError();

				if (error_code == 109 && !panel->panel_power)
				{
					break;
				}

				std::cerr << "Pipe read error[" << error_code << "]: " << std::system_category().message(error_code) << "\n";
			}
			else
			{
				panel->logger->write(event);
				panel->count_logged_events++;

				if (!panel->panel_power)
				{
					std::cout << "\r" << "Logged " << panel->count_logged_events << " from " << panel->count_session_events << std::flush;
				}
			}
		} while (panel->count_logged_events < panel->count_session_events);

		// allow close handles if all messages logged
		panel->channel_sync->release();
	} while (panel->panel_power);

	CloseHandle(panel->channel.pipe_read);
}
