#include "Interpreter.h"
#include "Tokenizer.h"

#include "../Parser/Parser.h"
#include "../Parser/ParserName.h"
#include "../Parser/ParserNameArg.h"
#include "../Parser/ParserOptArg.h"
#include "../Parser/ParserTr.h"

#include "../Commands/Command.h"
#include "../Commands/TimeCommand.h"
#include "../Commands/DateCommand.h"
#include "../Commands/EchoCommand.h"
#include "../Commands/TouchCommand.h"
#include "../Commands/WcCommand.h"
#include "../Commands/TruncateCommand.h"
#include "../Commands/RmCommand.h"
#include "../Commands/PromptCommand.h"
#include "../Commands/HeadCommand.h"
#include "../Commands/TrCommand.h"
#include "../Commands/BatchCommand.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>

Interpreter::Interpreter() {
	prompt = "$";
}

std::string Interpreter::getPrompt() {
	return prompt;
}

void Interpreter::setPrompt(std::string p) {
	prompt = p;
}


//EXTRACT TEXT


TextResult Interpreter::extractText(const Parsed& p) {
	TextResult r;

	if (!p.hasArg || p.arg == "") {
		std::ostringstream ss;
		ss << std::cin.rdbuf();
		r.text = ss.str();
		std::cin.clear();
		FILE* fp;
		freopen_s(&fp, "CON", "r", stdin);
		return r;
	}
	if (p.argQuoted) {
		r.text = p.arg;
		return r;
	}

	std::ifstream file(p.arg);
	if (!file.is_open()) {
		r.ok = false;
		r.error = "Ne mogu da otvorim fajl " + p.arg;
		return r;
	}

	std::ostringstream ss;
	ss << file.rdbuf();
	r.text = ss.str();
	return r;
}


//PRAVLJENJE KOMANDE


Command* Interpreter::createCommand(const Parsed& p, const std::string& preparedText) {
	if (p.name == "date") return new DateCommand();
	if (p.name == "time") return new TimeCommand();
	if (p.name == "touch") return new TouchCommand(p.arg);
	if (p.name == "echo") return new EchoCommand(preparedText);
	if (p.name == "wc") return new WcCommand(p.opt, preparedText);
	if (p.name == "rm") return new RmCommand(p.arg);
	if (p.name == "truncate") return new TruncateCommand(p.arg);
	if (p.name == "prompt") return new PromptCommand(this, p.arg);
	if (p.name == "head") return new HeadCommand(p.opt, preparedText);
	if (p.name == "tr") return new TrCommand(preparedText, p.opt, p.opt2);
	if (p.name == "batch") return new BatchCommand(this, p.arg);
	return nullptr;
}


//PROCESS


void Interpreter::process(std::string line) {
	
	if (!checkLexical(line)) return;

	RedirectInfo red = parseRedirects(line);

	if (!checkRedirectSyntax(red)) return;

	std::vector<std::string> segments = splitPipeline(line);
	if (segments.size() > 1) {
		executePipeline(segments, red);
		return;
	}

	auto toks = tokenize(line);
	if (toks.size() == 0) return;

	std::string name = toks[0].text;
	Parser* parser = nullptr;
	if (name == "time" || name == "date") {
		parser = new ParserName();
	 }
	else if (name == "wc" || name == "head") {
		parser = new ParserOptArg();
	}
	else if (name == "tr") {
		parser = new ParserTr();
	}
	else {
		parser = new ParserNameArg();
	}

	Parsed p = parser->parse(line);
	delete parser;

	if (!checkSyntax(p)) return;
	if (!checkSemantics(p, red, true, true)) return;

	if (p.name == "prompt" && (!p.hasArg || !p.argQuoted)) {
		std::cerr << "prompt: mora imati argument pod navodnicima\n";
		return;
	}

	std::ifstream inFile;
	std::streambuf* oldCin = nullptr;

	if (red.hasInput) {
		inFile.open(red.inputFile);
		if (!inFile.is_open()) {
			std::cerr << "Greska! Ne mogu da nadjem fajl: " << red.inputFile << "\n";
			return;
		}
		oldCin = std::cin.rdbuf(inFile.rdbuf());
	}

	bool needsText = (p.name == "echo" || p.name == "wc" || p.name == "head" || p.name == "tr");
	std::string preparedText;

	if (needsText) {
		TextResult tr = extractText(p);
		if (!tr.ok) {
			std::cerr << p.name << ":" << tr.error << "\n";
			return;
		}
		preparedText = tr.text;
	}

	Command* cmd = createCommand(p, preparedText);
	if (cmd == nullptr) {
		std::cerr << "Unknown command: " << p.name << "\n";
		return;
	}

	std::ofstream outFile;
	std::ostringstream consoleCapture;
	std::streambuf* oldCout = nullptr;

	if (red.hasOutput) {
		auto mode = red.append ? (std::ios::out | std::ios::app) : std::ios::out;
		outFile.open(red.outputFile, mode);
		if (!outFile.is_open()) {
			if (oldCin) std::cin.rdbuf(oldCin);
			std::cerr << "Greska! Ne mogu da otvorim fajl za pisanje: " << red.outputFile <<"\n";
			delete cmd;
			return;
		}
		oldCout = std::cout.rdbuf(outFile.rdbuf());
	}
	else {
		oldCout = std::cout.rdbuf(consoleCapture.rdbuf());
	}

	cmd->run();
	delete cmd;

	std::cout.rdbuf(oldCout);
	if (oldCin) std::cin.rdbuf(oldCin);

	if (!red.hasOutput) {
		std::string out = consoleCapture.str();
		std::cout << out;
		if (!out.empty() && out.back() != '\n') {
			std::cout << "\n";
		}
	}
}


//REDIREKCIJA


RedirectInfo Interpreter::parseRedirects(std::string& line) {
	RedirectInfo r;
	auto toks = tokenize(line);
	std::vector<Token> cleanToks;
	bool redirectionStarted = false;

	for (size_t i = 0; i < toks.size(); i++) {
		if (toks[i].quoted) {
			if (redirectionStarted) r.badPosition = true;
			cleanToks.push_back(toks[i]);
			continue;
		}

		std::string t = toks[i].text;

		if (t.size() >= 2 && t[0] == '>' && t[1] == '>') {
			redirectionStarted = true;
			r.hasOutput = true;
			r.append = true;
			if (t.size() == 2) {
				if (i + 1 < toks.size()) r.outputFile = toks[++i].text;
			} 
			else r.outputFile = t.substr(2);
		}
		else if (!t.empty() && t[0] == '>') {
			redirectionStarted = true;
			r.hasOutput = true;
			r.append = false;
			if (t.size() == 1) {
				if (i + 1 < toks.size()) r.outputFile = toks[++i].text;
			} 
			else r.outputFile = t.substr(1);
		}
		else if (!t.empty() && t[0] == '<') {
			redirectionStarted = true;
			r.hasInput = true;
			if (t.size() == 1) {
				if (i+1 < toks.size()) r.inputFile = toks[++i].text;
			}
			else r.inputFile = t.substr(1);
		}
		else {
			if (redirectionStarted) r.badPosition = true;
			cleanToks.push_back(toks[i]);
		}
	}

	std::string cleanLine;
	for (auto& t : cleanToks) {
		if (!cleanLine.empty()) cleanLine += " ";
		if (t.quoted) cleanLine += "\"" + t.text + "\"";
		else cleanLine += t.text;
	}
	line = cleanLine;

	return r;
}

//PIPELINE

std::vector<std::string> Interpreter::splitPipeline(const std::string& line) {
	std::vector<std::string> segments;
	std::string current;
	bool inQuotes = false;

	for (char c : line) {
		if (c == '"') {
			inQuotes = !inQuotes;
			current += c;
		}
		else if (c == '|' && !inQuotes) {
			segments.push_back(current);
			current = "";
		}
		else {
			current += c;
		}
	}
	segments.push_back(current);
	return segments;
}

void Interpreter::executePipeline(std::vector<std::string>& segments, RedirectInfo& red) {
	
	if (!checkRedirectSyntax(red)) return;

	std::vector<Parsed> parsedSegs;

	for (size_t i = 0; i < segments.size(); i++) {
		auto toks = tokenize(segments[i]);
		if (toks.empty()) {
			std::cerr << "Greska! Prazna komanda u cevovodu\n";
			return;
		}

		std::string name = toks[0].text;
		Parser* parser = nullptr;

		if (name == "time" || name == "date") parser = new ParserName();
		else if (name == "wc" || name == "head") parser = new ParserOptArg();
		else if (name == "tr") parser = new ParserTr();
		else parser = new ParserNameArg();

		parsedSegs.push_back(parser->parse(segments[i]));
		delete parser;
	}

	for (size_t i = 0; i < parsedSegs.size(); i++) {
		bool isFirst = (i == 0);
		bool isLast = (i == parsedSegs.size() - 1);
		if (!checkSyntax(parsedSegs[i])) return;
		if (!checkSemantics(parsedSegs[i], red, isFirst, isLast)) return;
	}

	std::string inputText = "";

	for (size_t i = 0; i < parsedSegs.size(); i++) {
		bool isFirst = (i == 0);
		bool isLast = (i == parsedSegs.size() - 1);
		const Parsed& p = parsedSegs[i];

		std::istringstream inputStream(inputText);
		std::ostringstream outputStream;
		std::ofstream outFile;
		std::ifstream inFile;
		std::streambuf* oldCin = nullptr;
		std::streambuf* oldCout = nullptr;

		if (!isFirst) {
			oldCin = std::cin.rdbuf(inputStream.rdbuf());
		}
		else if (red.hasInput) {
			inFile.open(red.inputFile);
			if (!inFile.is_open()) {
				std::cerr << "Greska! Ne mogu da nadjem fajl: " << red.inputFile << "\n";
				return;
			}
			oldCin = std::cin.rdbuf(inFile.rdbuf());
		}

		if (isLast && red.hasOutput) {
			auto mode = red.append ? (std::ios::out | std::ios::app) : std::ios::out;
			outFile.open(red.outputFile, mode);
			if (!outFile.is_open()) {
				if (oldCin) std::cin.rdbuf(oldCin);
				std::cerr << "Greska! Ne mogu da otvorim fajl za pisanje: " << red.outputFile << "\n";
				return;
			}
			oldCout = std::cout.rdbuf(outFile.rdbuf());
		}
		else {
			oldCout = std::cout.rdbuf(outputStream.rdbuf());
		}

		bool needsText = (p.name == "echo" || p.name == "wc" || p.name == "head" || p.name == "tr");
		std::string preparedText;

		if (needsText) {
			TextResult tr = extractText(p);
			if (!tr.ok) {
				if (oldCout) std::cout.rdbuf(oldCout);
				if (oldCin) std::cin.rdbuf(oldCin);
				std::cerr << p.name << ":" << tr.error << "\n";
				return;
			}
			preparedText = tr.text;
		}

		Command* cmd = createCommand(p, preparedText);
		if (cmd == nullptr) {
			if (oldCout) std::cout.rdbuf(oldCout);
			if (oldCin) std::cin.rdbuf(oldCin);
			std::cerr << "Unknown command: " << p.name << "\n";
			return;
		}
		cmd->run();
		delete cmd;

		if (!isLast) {
			inputText = outputStream.str();
		}

		if (oldCout) std::cout.rdbuf(oldCout);
		if (oldCin) std::cin.rdbuf(oldCin);

		if (isLast && !red.hasOutput) {
			std::string out = outputStream.str();
			std::cout << out;
			if (!out.empty() && out.back() != '\n') {
				std::cout << "\n";
			}
		}
	}
}