#pragma once
#include "Command.h"
#include <string>

class EchoCommand : public Command {
	private:
		std::string text;

	public:
		EchoCommand(std::string t);
		void run();
};

