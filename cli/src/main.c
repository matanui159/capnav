#include <windows.h>
#include "io.h"

#define BUFFER_SIZE 256

void entry() {
	SetConsoleTitle(TEXT("capnav cli"));
	io_init();

	for (;;) {
		DWORD size = GetCurrentDirectory(0, NULL);
		TCHAR cwd[size];
		GetCurrentDirectory(size, cwd);
		io_write(cwd, size - 1, 0b1001);
		IO_WRITE("> ", 0b1010);

		TCHAR buffer[BUFFER_SIZE];
		io_read(buffer, BUFFER_SIZE, 0b1111);
		IO_ERROR("This is an error\n\n", 0b1100);
	}
}