#pragma once
#include "Command.h"
#include <string>

class Interpreter;

class BatchCommand : public Command {
	private: 
		Interpreter* interp;
		std::string filename;
	public:
		BatchCommand(Interpreter* i, std::string fn);
		void run();
};

