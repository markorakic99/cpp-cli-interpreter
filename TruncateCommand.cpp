#include "TruncateCommand.h"
#include <fstream>
#include <iostream>

TruncateCommand::TruncateCommand(std::string fn) {
	filename = fn;
}

void TruncateCommand::run() {

	std::ifstream check(filename);

	if (filename == "") {
		std::cerr <<"truncate: Nedostaje ime fajla\n";
		return;
	}

	if (!check.good()) {
		std::cerr << "truncate: fajl ne postoji\n";
		return;
	}
	check.close();

	std::ofstream out(filename, std::ios::trunc);
	if (!out.is_open()) {
		std::cerr << "truncate: ne mogu da otvorim fajl: " << filename << "\n";
		return;
	}
	out.close();
}