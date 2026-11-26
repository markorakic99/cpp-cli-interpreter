#include "Interpreter.h"
#include <iostream>
#include <cctype>

static bool isAllowedChar(char c) {
	return std::isalnum((unsigned char)c) || c == '\t' ||
		c == ' ' || c == '|' || c == '<' || c == '>' ||
		c == '"' || c == '-' || c == '.' || c == '/' ||
		c == '_' || c == ':' || c == '\\';
}

bool Interpreter::checkLexical(const std::string& line) {
	bool inQuotes = false;
	std::string markers(line.size(), ' ');
	bool hasError = false;

	for (size_t i = 0; i < line.size(); i++) {
		char c = line[i];
		if (inQuotes) {
			if (c == '"') inQuotes = false;
		}
		else {
			if (c == '"') {
				inQuotes = true;
			}
			else if (!isAllowedChar(c)) {
				markers[i] = '^';
				hasError = true;
			}
		}
	}

	if (inQuotes) {
		std::cerr << "Greska! Navodnici nisu zatvoreni.\n";
		return false;
	}

	if (hasError) {
		std::cerr << "Greska! Nedozvoljeni znakovi:\n";
		std::cerr << line << "\n";
		std::cerr << markers << "\n";
		return false;
	}
	return true;
}

bool Interpreter::checkRedirectSyntax(const RedirectInfo& red) {

	if (red.badPosition) {
		std::cerr << "Greska! Redirekcija mora biti na kraju komande\n";
		return false;
	}

	if (red.hasOutput && red.outputFile.empty()) {
		std::cerr << "Greska! Nedostaje ima fajla posle '>'\n";
		return false;
	}
	if (red.hasInput && red.inputFile.empty()) {
		std::cerr << "Greska! Nedostaje ime fajla posle '<'\n";
		return false;
	}
	return true;
}

bool Interpreter::checkSyntax(const Parsed& p) {

	if (p.hasExtra) {
		std::cerr << p.name << ": previse argumenata\n";
		return false;
	}

	if (p.name == "wc") {
		if (!p.hasOpt) {
			std::cerr << "wc: nedostaje opcija\n";
			return false;
		}
		if (p.opt != "-w" && p.opt != "-c") {
			std::cerr << "wc: nepoznata opcija: " << p.opt << "\n";
			return false;
		}
	}

	if (p.name == "head") {
		if (!p.hasOpt) {
			std::cerr << "head: nedostaje opcija\n";
			return false;
		}
		if (p.opt.size() < 3 || p.opt[0] != '-' || p.opt[1] != 'n' || p.opt.size() > 7) {
			std::cerr << "head: neispravna opcija: " << p.opt << "\n";
			return false;
		}
		for (size_t i = 2; i < p.opt.size(); i++) {
			if (!std::isdigit((unsigned char)p.opt[i])) {
				std::cerr << "head: neispravna opcija\n";
				return false;
			}
		}
	}
	if (p.name == "prompt" || p.name == "touch" || p.name == "truncate" ||
		p.name == "rm" || p.name == "batch") {
		if (!p.hasArg || p.arg.empty()) {
			std::cerr << p.name << ": nedostaje argument\n";
			return false;
		}
	}
	if (p.name == "tr") {
		if (!p.hasOpt) {
			std::cerr << "tr: nedostaje prva opcija\n";
			return false;
		}
	}

	return true;
}

static bool usesArgAsInput(const std::string& name) {
	return name == "echo" || name == "wc" || name == "head" || name == "tr";
}

static bool hasNoInputStream(const std::string& name) {
	return name == "time" || name == "date";
}

static bool hasNoOutputStream(const std::string& name) {
	return name == "touch" || name == "rm" || name == "prompt" || name == "truncate";
}

bool Interpreter::checkSemantics(const Parsed& p, const RedirectInfo& red, bool isFirst, bool isLast) {
	if (red.hasInput && p.hasArg && usesArgAsInput(p.name)) {
		std::cerr << p.name << ": ne moze i argument i redirekcija u ovoj funkciji\n";
		return false;
	}
	if (!isFirst) {
		if (hasNoInputStream(p.name)) {
			std::cerr << p.name << ": ne moze primati ulaz iz cevovoda\n";
			return false;
		}
		if (p.hasArg && usesArgAsInput(p.name)) {
			std::cerr << p.name << ": ulaz je vec definisan cevovodom, ne moze i argument\n";
			return false;
		}
	}
	if (!isLast && hasNoOutputStream(p.name)) {
		std::cerr << p.name << ": ne moze prosledjivati izlaz u cevovod\n";
		return false;
	}

	return true;
}