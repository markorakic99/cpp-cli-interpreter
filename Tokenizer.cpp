#include "Tokenizer.h"


std::vector<Token> tokenize(std::string line) {
	std::vector<Token> tokens;
	size_t i = 0;

	while (i < line.size()) {
		
		while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) {
			i++;
		}

		if (i >= line.size()) break;

		if (line[i] == '"') {
			i++;
			size_t start = i;

			while (i < line.size() && line[i] != '"') {
				i++;
			}

			std::string t;

			if (i < line.size()) {
				t = line.substr(start, i-start);
			}
			else {
				t = line.substr(start);
			}

			tokens.push_back({t, true});

			if (i < line.size()) {
				i++;
			}
		}
		else {
			size_t start = i;

			while (i < line.size() && line[i] != ' ' && line[i] != '\t') {
				i++;
			}
			tokens.push_back({line.substr(start, i-start), false});
		}
	}
	return tokens;
}

bool readCommandLine(std::istream& in, std::string& line) {
	const size_t MAX_LINE_LENGTH = 512;

	line.clear();
	char c;
	bool any = false;

	while (line.size() < MAX_LINE_LENGTH && in.get(c)) {
		any = true;
		if (c == '\n') return true;
		line += c;
	}
	
	if (!any) return false;

	if (line.size() == MAX_LINE_LENGTH) {
		while (in.get(c) && c != '\n') {
			//odbaci ostatak predugacke linije do kraja reda
		}
	}

	return true;
}