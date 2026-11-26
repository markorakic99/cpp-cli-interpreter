#pragma once
#include "Parser.h"
#include <string>

class ParserOptArg : public Parser {
	public:
		Parsed parse(std::string line);
};

