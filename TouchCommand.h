#pragma once
#include "Command.h"
#include <string>

class TouchCommand : public Command {
	private:
		std::string filename;
	public:
		TouchCommand(std::string fn);
		void run();
};

