#include <windows.h>

void entry() {
	MessageBox(NULL, TEXT("Hello, World!"), NULL, 0);
	ExitProcess(0);
}