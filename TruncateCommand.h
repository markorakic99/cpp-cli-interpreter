#pragma once
#include "Command.h"
#include <string>

class TruncateCommand : public Command {
	private:
		std::string filename;
	public:
		TruncateCommand(std::string fn);
		void run();
};

