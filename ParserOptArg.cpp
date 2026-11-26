#include "ParserOptArg.h"
#include "../Core/Tokenizer.h"
#include <string>

Parsed ParserOptArg::parse(std::string line) {
	Parsed p;
	
	auto toks = tokenize(line);

	if (toks.size() == 0) return p;

	p.name = toks[0].text;

	if (toks.size() == 2) {
		p.opt = toks[1].text;
		p.hasOpt = true;
	}
	if (toks.size() == 3) {
		p.opt = toks[1].text;
		p.hasOpt = true;
		p.arg = toks[2].text;
		p.hasArg = true;
		p.argQuoted = toks[2].quoted;
	}

	if (toks.size() > 3) {
		p.hasExtra = true;
	}
	return p;
}