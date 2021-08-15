#ifndef LOGGER_H
#define LOGGER_H

#include <ctime>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h> 

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

class Logger
{
public:
	static Logger &Instance()
	{
		static Logger instance;

		return instance;
	}

	enum class Level { LOG_MESSAGE, LOG_WARNING, LOG_ERROR };

	void Log(const std::string &message, Level level)
	{
		std::ostringstream oss;

		switch (level)
		{
			case Level::LOG_MESSAGE:
				SetColor(Color::GREEN, Color::BLACK);
				oss << "LOG MESSAGE -- ";
				break;
			case Level::LOG_WARNING:
				SetColor(Color::YELLOW, Color::BLACK);
				oss << "LOG WARNING -- ";
				break;
			case Level::LOG_ERROR:
				SetColor(Color::RED, Color::BLACK);
				oss << "LOG ERROR   -- ";
				break;
		}

		oss << "[" << GetFormattedTime() << "] : " << message;
		std::cout << oss.str() << std::endl;

		ResetColor();
	}

private:
	Logger()
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);
	}

	std::string GetFormattedTime()
	{
		std::chrono::time_point<std::chrono::system_clock, std::chrono::system_clock::duration> now = std::chrono::system_clock::now();
		std::time_t calendarTime = std::chrono::system_clock::to_time_t(now);

		constexpr size_t SIZE = 50;
		char formattedTime[SIZE];

		/* c_time_s uses localtime_s + asctime_s */
		//ctime_s(formattedTime, SIZE, &calendarTime);

		/* same as above */
		//std::tm brokenDownTime;
		//(void)localtime_s(&brokenDownTime, &calendarTime);
		//asctime_s(formattedTime, &brokenDownTime);

		/* strftime uses format string */
		std::tm brokenDownTime;
		(void)localtime_s(&brokenDownTime, &calendarTime);
		std::strftime(formattedTime, SIZE, "%a %d %Y - %H:%M:%S", &brokenDownTime);

		return formattedTime;
	}

	enum class Color
	{
		RESET_COLOR,

		BLACK = 30,
		RED,
		GREEN,
		YELLOW,
		BLUE,
		MAGENTA,
		CYAN,
		WHITE,
	};

	void SetColor(Color text, Color background)
	{
		std::ostringstream oss;

		oss << "\x1b[" << static_cast<int>(text) << "m";
		oss << "\x1b[" << static_cast<int>(background) + 10 << "m";

		std::cout << oss.str();
	}

	void ResetColor()
	{
		SetColor(Color::WHITE, Color::BLACK);
	}
};

#endif // LOGGER_H