#pragma once
#include "Parser.h"
#include <string>

class ParserTr : public Parser {
	public:
		Parsed parse(std::string line);
};
