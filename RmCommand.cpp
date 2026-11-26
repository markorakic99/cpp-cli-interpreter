#include "RmCommand.h"
#include <cstdio>
#include <iostream>

RmCommand::RmCommand(std::string fn) {
	filename = fn;
}

void RmCommand::run() {
	if (filename == "") {
		std::cerr << "rm: nedostaje ime fajla\n";
		return;
	}

	if (std::remove(filename.c_str()) != 0) {
		std::cerr << "rm: ne mogu da obrisem fajl: " << filename << "\n";
	}
}
