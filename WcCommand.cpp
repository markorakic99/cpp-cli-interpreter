#include "WcCommand.h"
#include <string>
#include <iostream>
#include <cctype>

WcCommand::WcCommand(std::string o, std::string t) {
	text = t;
	opt = o;
}

void WcCommand::run() {
	if (opt == "-c") {
		size_t count = text.size();
		std::cout << count;
	}
	else if (opt == "-w") {
		int count = 0;
		bool inWord = false;

		for (size_t i = 0; i < text.size(); i++) {

			char c = text[i];

			if (std::isspace((unsigned char)c)) {
				inWord = false;
			}
			else {
				if (inWord == false) {
					count = count + 1;
					inWord = true;
				}
			}
		}
		std::cout << count;
	}
	else {
		std::cerr << "Wc: nepoznata opcija: " << opt << "\n";
	}
}