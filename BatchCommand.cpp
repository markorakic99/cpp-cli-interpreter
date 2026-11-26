#include "BatchCommand.h"
#include <string>
#include <iostream>
#include <fstream>
#include "../Core/Interpreter.h"
#include "../Core/Tokenizer.h"

BatchCommand::BatchCommand(Interpreter* i, std::string fn) {
	interp = i;
	filename = fn;

}

void BatchCommand::run() {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "batch: Ne mogu da otvorim fajl: " << filename << "\n";
		return;
	}

	std::string line;
	while (readCommandLine(file, line)) {
		if (line.empty()) continue;
		interp->process(line);
	}
}