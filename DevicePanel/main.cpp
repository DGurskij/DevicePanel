#include <map>
#include <iostream>
#include <string>
#include <thread>

#include "Emitter/Emitter.h"
#include "Registrator/Registrator.h"
#include "Logger/LoggerFactory.h"

#include "utils/Utils.h"
#include <system_error>

void showCommandList(Panel* panel, const char* params);
void exitPanel(Panel* panel, const char* params);
void clearScreen(Panel* panel, const char* params);

void printCurrentDate(Panel* panel, const char* params);
void printCurrentTime(Panel* panel, const char* params);
void printStats(Panel* panel, const char* params);

void setLoggerLevel(Panel* panel, const char* params);

typedef void(*CommandHandler)(Panel*, const char*);
typedef std::map<std::string, CommandHandler> CommandsMap;

const char* TITLE = "Panel(0.1):\n";

int main()
{
	Panel* panel = new Panel;

	/*
	* Setup initial panel config
	*/

	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

	if (!CreatePipe(&panel->channel.pipe_read, &panel->channel.pipe_wirte, &sa, sizeof(Event) * 100000))
	{
		DWORD error_code = GetLastError();
		std::cerr << "Pipe create error[" << error_code << "]: " << std::system_category().message(error_code) << "\n";
		return 1;
	}

	panel->event_spawn_time = 100;
	panel->count_session_events = 0;
	panel->count_logged_events = 0;
	panel->emitter_locked = true;

	panel->log_file_path = new char[] {"logs"};

	Logger* loggerLvl0 = LoggerFactory::createLogger(0, panel->log_file_path);
	Logger* loggerLvl1 = LoggerFactory::createLogger(1, panel->log_file_path);
	Logger* loggerLvl2 = LoggerFactory::createLogger(2, panel->log_file_path);

	panel->logger = loggerLvl0;
	panel->loggers = new Logger * [3] { loggerLvl0,	loggerLvl1,	loggerLvl2 };

	std::thread emitter(emitterWorker, panel);
	std::thread registrator(registratorWorker, panel);

	panel->emitter_lock = new std::binary_semaphore{ 0 };
	panel->channel_sync = new std::binary_semaphore{ 1 };

	/*
	* Setup commands
	*/

	CommandsMap commands =
	{
		{ "help",     showCommandList },
		{ "exit",     exitPanel },
		{ "clear",    clearScreen },
		{ "date",     printCurrentDate },
		{ "time",     printCurrentTime },
		{ "stat",     printStats },
		{ "level",    setLoggerLevel },
		{ "pause",    pauseEmitter },
		{ "resume",   resumeEmitter },
		{ "faster",   fasterEmitter },
		{ "slower",   slowerEmitter },
		{ "interval", intervalEmitter },
	};

	CommandsMap::const_iterator command;

	std::string input;
	std::string inputed_command;
	char inputed_params[50];

	panel->panel_power = true;
	std::cout << TITLE;

	/*
	* Device workflow
	*/

	panel->emitter_locked = false;
	panel->emitter_lock->release();

	do
	{
		std::cout << ">> ";
		std::getline(std::cin, input);

		ltrim(input);

		size_t space_pos = input.find(' ');

		if (space_pos == -1)
		{
			inputed_command = input;
		}
		else
		{
			inputed_command = input.substr(0, space_pos);
			std::string params = input.substr(space_pos + 1);
			ltrim(params);
			strcpy_s(inputed_params, 50, params.c_str());
		}

		command = commands.find(inputed_command);

		if (command == commands.end())
		{
			std::cerr << "Unknown command '" << input << "', enter 'help' to show commands\n";
		}
		else
		{
			command->second(panel, space_pos == -1 ? NULL : inputed_params);
		}
	} while (panel->panel_power);

	std::cout << "System::shutdown\n";

	// unlock emitter if was puased
	if (panel->emitter_locked)
	{
		resumeEmitter(panel, NULL);
	}

	emitter.join();

	panel->channel_sync->acquire(); // await registrator

	registrator.join();

	delete panel->loggers[0];
	delete panel->loggers[1];
	delete panel->loggers[2];
	delete[] panel->loggers;

	delete panel;

	return 0;
}

void exitPanel(Panel* panel, const char* params)
{
	panel->panel_power = false;
}

void clearScreen(Panel* panel, const char* params)
{
	system("cls");
	std::cout << TITLE;
}

void showCommandList(Panel* panel, const char* params)
{
	std::cout << "\n";
	std::cout << "help            : show exist commands\n";
	std::cout << "exit            : shutdown panel\n";
	std::cout << "clear           : clear screen\n";
	std::cout << "date            : print current date\n";
	std::cout << "time            : print current time\n";
	std::cout << "stat            : print count events for this session\n";
	std::cout << "pause           : pause spawning events\n";
	std::cout << "resume          : resume spawning events\n";
	std::cout << "faster          : reduce min interval between spawn events by half\n";
	std::cout << "slower          : doubles min interval between spawn events\n";
	std::cout << "interval [time] : print min interval or set it [time](ms) \n";
	std::cout << "level N         : set logger level N => [0; 2]\n";
	std::cout << "\n";
}

void printCurrentDate(Panel* panel, const char* params)
{
	time_t timestamp = std::time(NULL);
	struct tm time;

	localtime_s(&time, &timestamp);
	char time_str[21];

	std::strftime(time_str, 21, "%F\n", &time);
	std::cout << time_str;
}

void printCurrentTime(Panel* panel, const char* params)
{
	time_t timestamp = std::time(NULL);
	struct tm time;

	localtime_s(&time, &timestamp);
	char time_str[21];

	std::strftime(time_str, 21, "%T\n", &time);
	std::cout << time_str;
}

void printStats(Panel* panel, const char* params)
{
	std::cout << "Count events: " << panel->count_session_events << "\n";
}

void setLoggerLevel(Panel* panel, const char* params)
{
	int level = atoi(params);

	if (level < 0 || level > 2)
	{
		std::cerr << "Unknown level '" << level << "', allow levels [0; 2]\n";
	}
	else
	{
		panel->logger = panel->loggers[level];
	}

}
