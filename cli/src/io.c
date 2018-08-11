#include "io.h"

#define CONSOLE_COLOR(handle, color) SetConsoleTextAttribute((handle), (color) & 0xF)

static HANDLE g_input;
static HANDLE g_output;
static HANDLE g_error;

void io_init() {
	g_input = GetStdHandle(STD_INPUT_HANDLE);
	g_output = GetStdHandle(STD_OUTPUT_HANDLE);
	g_error = GetStdHandle(STD_ERROR_HANDLE);
}

void io_read(PTSTR text, int size, char color) {
	DWORD read;
	CONSOLE_COLOR(g_output, color);
	ReadConsole(g_input, text, size, &read, NULL);
	text[read - 1] = '\0';
}

void io_write(PTSTR text, int size, char color) {
	CONSOLE_COLOR(g_output, color);
	WriteConsole(g_output, text, size, NULL, NULL);
}

void io_error(PTSTR text, int size, char color) {
	CONSOLE_COLOR(g_error, color);
	WriteConsole(g_error, text, size, NULL, NULL);
}