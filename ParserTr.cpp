#include "ParserTr.h"
#include <string>

Parsed ParserTr::parse(std::string line) {
	
	Parsed p;

	p.name = "tr";
	size_t i = 0;

	while (i < line.size() && line[i] == ' ') i++;
	while (i < line.size() && line[i] != ' ') i++;
	while (i < line.size() && line[i] == ' ') i++;

	if (i >= line.size()) return p;

	if (line[i] == '"') {
		i++;
		size_t start = i;
		while (i < line.size() && line[i] != '"') i++;
		p.arg = line.substr(start, i-start);
		p.hasArg = true;
		p.argQuoted = true;
		if (i < line.size()) i++;
		while (i < line.size() && line[i] == ' ') i++;
	}
	else if (line[i] != '-') {
		size_t start = i;
		while (i < line.size() && line[i] != ' ') i++;
		p.arg = line.substr(start, i - start);
		p.hasArg = true;
		while (i < line.size() && line[i] == ' ') i++;
	}

	if (i < line.size() && line[i] == '-') {
		i++;
		if (i < line.size() && line[i] == '"') {
			i++;
			size_t start = i;
			while (i < line.size() && line[i] != '"') i++;
			p.opt = line.substr(start, i-start);
			p.hasOpt = true;
			if (i < line.size()) i++;
		}
	} else return p;

	while (i < line.size() && line[i] == ' ') i++;

	if (i < line.size() && line[i] == '"') {
		i++;
		size_t start = i;
		while (i < line.size() && line[i] != '"') i++;
		p.opt2 = line.substr(start, i-start);
		p.hasOpt2 = true;
		if (i < line.size()) i++;
	}

	while (i < line.size() && line[i] == ' ') i++;

	if (i < line.size() && line[i] != ' ') {
		p.hasExtra = true;
	}

	return p;
}