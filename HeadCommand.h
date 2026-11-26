#pragma once
#include "Command.h"
#include <string>

class HeadCommand : public Command {
	private:
		std::string opt;
		std::string text;
	public:
		HeadCommand(std::string o, std::string t);
		void run();
};

