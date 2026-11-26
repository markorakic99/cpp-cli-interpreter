#include "TimeCommand.h"
#include <iostream>
#include <ctime>

void TimeCommand::run() {
	std::time_t t = std::time(nullptr);
	std::tm tm{};
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	tm = *std::localtime(&t);
#endif
	
	if (tm.tm_hour < 10) std::cout << "0";
	std::cout << tm.tm_hour << ":";
	if (tm.tm_min < 10) std::cout << "0";
	std::cout << tm.tm_min << ":";
	if (tm.tm_sec < 10) std::cout << "0";
	std::cout << tm.tm_sec;
}