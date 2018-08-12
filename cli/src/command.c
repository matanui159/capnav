#include "command.h"
#include "io.h"

static PTSTR trim(PTSTR str) {
	for (; *str == ' ' || *str == '\t' || *str == '\n'; ++str);
	
}

void command_run(PTSTR command) {
	if (command[0] == '+') {
		
	}
}