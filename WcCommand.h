#pragma once
#include "Command.h"
#include <string>

class WcCommand : public Command {
	private:
		std::string text;
		std::string opt;
	public:
		WcCommand(std::string o, std::string t);
		void run();
};


