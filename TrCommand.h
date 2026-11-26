#pragma once
#include "Command.h"
#include <string>

class TrCommand : public Command {
	private:
		std::string text;
		std::string from;
		std::string to;
	public:
		TrCommand(std::string txt, std::string f, std::string t);
		void run();
};
