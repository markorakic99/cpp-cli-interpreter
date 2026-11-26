#include "TouchCommand.h"
#include <string>
#include <iostream>
#include <fstream>

TouchCommand::TouchCommand(std::string fn) {
	filename = fn;
}

void TouchCommand::run() {
	if (filename == "") {
		std::cerr << "touch: neostaje ime fajla\n";
		return;
	}

	std::ifstream in(filename);
	if (in.good()) {
		std::cerr << "touch: Greska! Fajl '" << filename << "' vec postoji\n";
		return;
	}

	std::ofstream out(filename, std::ios::app);
	if (!out.is_open()) {
		std::cerr << "touch: ne mogu da otvorim fajl '" << filename << "'\n";
		return;
	}
	out.close();
}