#include "EchoCommand.h"
#include <iostream>

EchoCommand::EchoCommand(std::string t) {
	text = t;
}

void EchoCommand::run() {
	std::cout << text;
}