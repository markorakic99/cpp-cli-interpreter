#pragma once
#include <string>

struct Parsed {
	std::string name;
	std::string opt;
	std::string opt2;
	std::string arg;

	bool hasOpt = false;
	bool hasOpt2 = false;
	bool hasArg = false;
	bool argQuoted = false;
	bool hasExtra = false;
};


