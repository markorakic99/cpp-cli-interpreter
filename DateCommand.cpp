#include "DateCommand.h"
#include <iostream>
#include <ctime>

void DateCommand::run() {
	std::time_t t = std::time(nullptr);
	std::tm tm{};
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	tm = *std::localtime(&t);
#endif
	
	if (tm.tm_mday < 10) std::cout << "0";
	std::cout << tm.tm_mday << "/";
	if (tm.tm_mon + 1 < 10) std::cout << "0";
	std::cout << (tm.tm_mon + 1) << "/";
	std::cout << (tm.tm_year + 1900);
}