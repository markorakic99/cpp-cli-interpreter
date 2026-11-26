#pragma once
#include "Parser.h"
#include <string>

class ParserNameArg : public Parser {
	public:
		Parsed parse(std::string line);
};

