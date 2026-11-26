#pragma once
#include <string>
#include <vector>
#include <istream>

struct Token {
	std::string text;
	bool quoted = false;
};

std::vector<Token> tokenize(std::string line);
bool readCommandLine(std::istream& in, std::string& line);