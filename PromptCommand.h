#pragma once
#include <string>
#include "Command.h"

class Interpreter;

class PromptCommand : public Command {
	private:
	Interpreter* interp;
		std::string newPrompt;
	public:
		PromptCommand(Interpreter* i, std::string p);
		void run();
};

