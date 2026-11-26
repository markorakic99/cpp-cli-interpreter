#include "ParserName.h"
#include "../Core/Tokenizer.h"

Parsed ParserName::parse(std::string line) {
	Parsed p;
	auto toks = tokenize(line);

	if (toks.size() == 0) return p;

	p.name = toks[0].text;

	if (toks.size() > 1) {
		p.hasExtra = true;
	}

	return p;
}