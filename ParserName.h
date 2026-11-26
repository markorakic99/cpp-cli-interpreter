#pragma once
#include "Parser.h"
#include <string>

class ParserName : public Parser {
	public:
		Parsed parse(std::string line);
};

