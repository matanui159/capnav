#include <windows.h>

void sys_entry() {
	MessageBox(NULL, TEXT("Hello, World!"), NULL, 0);
	ExitProcess(0);
}