#include "HeadCommand.h"
#include <iostream>
#include <sstream>
#include <cctype>

HeadCommand::HeadCommand(std::string o, std::string t) {
	text = t;
	opt = o;
}

void HeadCommand::run() {
	if (opt.size() < 3 || opt[0] != '-' || opt[1] != 'n') {
		std::cerr << "head: neispravna opcija, ocekivano -n<broj>\n";
	}

	std::string numberPart = opt.substr(2);
	for (int i = 0; i < numberPart.size(); i++) {
		if (!std::isdigit((unsigned char)numberPart[i])) {
			std::cerr << "head: Ocekivane samo cifre u opciji posle -n.\n";
			return;
		}
	}

	if (numberPart.size() > 5) {
		std::cerr << "head: dozvoljeno je najvise 5 cifara.\n";
		return;
	}

	int n = std::stoi(opt.substr(2));

	std::istringstream stream(text);
	std::string line;
	int count = 0;

	while (count < n && std::getline(stream, line)) {
		std::cout << line << "\n";
		count++;
	}
}