#pragma once
#include "Command.h"
#include <string>

class RmCommand : public Command {
	private:
		std::string filename;
	public:
		RmCommand(std::string fn);
		void run();
};

