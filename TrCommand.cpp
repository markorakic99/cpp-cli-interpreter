#include "TrCommand.h"
#include <string>
#include <iostream>

TrCommand::TrCommand(std::string txt, std::string f, std::string t) {
	text = txt;
	from = f;
	to = t;
}

void TrCommand::run() {
	if (from == "") {
		std::cerr << "tr: prva opcija mora biti ispravno zadata.\n";
		return;
	}

	std::string result = text;
	size_t pos = 0;

	while ((pos = result.find(from, pos)) != std::string::npos) {
		result.replace(pos, from.size(), to);
		pos += to.size();
	}

	std::cout << result;
}