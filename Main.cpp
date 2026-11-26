#include"Core/Interpreter.h"
#include "Core/Tokenizer.h"
#include <iostream>
#include <string>
int main() {

	Interpreter interpreter;

	while (true) {
		std::cout << interpreter.getPrompt() << std::flush;
		std::string line;

		if (!readCommandLine(std::cin, line)) {
			break;
		}

		if (line == "") {
			continue;
		}

		interpreter.process(line);
	}
}