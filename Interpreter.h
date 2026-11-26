#pragma once
#include <string>
#include <vector>
#include "../Parser/Parsed.h"

class Command;

struct TextResult {
	bool ok = true;
	std::string text;
	std::string error;
};

struct RedirectInfo {
	std::string inputFile;
	std::string outputFile;
	bool append = false;
	bool hasInput = false;
	bool hasOutput = false;
	bool badPosition = false;
};

class Interpreter {
	private:
		TextResult extractText(const Parsed& p);
		Command* createCommand(const Parsed& p, const std::string& preparedText);
		std::string prompt;
		RedirectInfo parseRedirects(std::string& line);

		bool checkLexical(const std::string& line);
		bool checkRedirectSyntax(const RedirectInfo& red);
		bool checkSyntax(const Parsed& p);
		bool checkSemantics(const Parsed& p, const RedirectInfo& red, bool isFirst, bool isLast);

	public:
		void process(std::string line);
		std::string getPrompt();
		void setPrompt(std::string p);
		Interpreter();
		std::vector<std::string> splitPipeline(const std::string& line);
		void executePipeline(std::vector<std::string>& segments, RedirectInfo& red);
};


