#pragma once

class Command {
	public:
		virtual ~Command() {}
		virtual void run() = 0;
};