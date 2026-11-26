#include "ParserNameArg.h"
#include "../Core/Tokenizer.h"

Parsed ParserNameArg::parse(std::string line) {
	Parsed p;

	auto toks = tokenize(line);

	if (toks.size() == 0) return p;

	p.name = toks[0].text;

	if (toks.size() == 2) {
		p.arg = toks[1].text;
		p.hasArg = true;
		p.argQuoted = toks[1].quoted;
	}

	if (toks.size() > 2) {
		p.hasExtra = true;
	}

	return p;
}