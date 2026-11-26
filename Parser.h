#pragma once
#include "Parsed.h"
#include <string>

class Parser {
	public:
		virtual ~Parser() {}
		virtual Parsed parse(std::string line) = 0;
};