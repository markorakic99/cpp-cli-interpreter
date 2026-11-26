#include "PromptCommand.h"
#include "../Core/Interpreter.h"

PromptCommand::PromptCommand(Interpreter* i, std::string p) {
	interp = i;
	newPrompt = p;
}

void PromptCommand::run() {
	interp->setPrompt(newPrompt);
}
