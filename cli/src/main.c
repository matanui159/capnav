#include <windows.h>

static PTSTR g_buffer;
static int g_buffer_size;

static void buffer_reserve(int size) {
	static HANDLE heap;
	if (heap == NULL) {
		heap = GetProcessHeap();
	}

	int cap = size * sizeof(TCHAR) * 2;
	if (g_buffer == NULL) {
		g_buffer_size = size;
		g_buffer = HeapAlloc(heap, 0, cap);
	} else if (size > g_buffer_size) {
		g_buffer_size = size;
		g_buffer = HeapReAlloc(heap, 0, g_buffer, cap);
	}
}

void entry() {
	HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD mode;
	GetConsoleMode(input, &mode);
	SetConsoleMode(input, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	for (;;) {
		DWORD size = GetCurrentDirectory(g_buffer_size, g_buffer);
		if (size > g_buffer_size) {
			buffer_reserve(size);
			continue;
		}
		SetConsoleTextAttribute(output, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		WriteConsole(output, g_buffer, size - 1, NULL, NULL);

		PTSTR sep = TEXT("> ");
		SetConsoleTextAttribute(output, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		WriteConsole(output, sep, 2, NULL, NULL);

		size = 0;
		for (;;) {
			TCHAR c;
			DWORD read;
			ReadConsole(input, &c, 1, &read, NULL);
			if (read > 0) {
				WORD color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
				switch (c) {
					case '?':
						color = FOREGROUND_GREEN | FOREGROUND_BLUE;
					case '+':
						color = FOREGROUND_GREEN;
						break;
					case '-':
						color = FOREGROUND_RED | FOREGROUND_BLUE;
						break;
					case '=':
						color = FOREGROUND_RED | FOREGROUND_GREEN;
						break;
				}
				SetConsoleTextAttribute(output, color | FOREGROUND_INTENSITY);

				if (c == '\b') {
					if (size == 0) {
						continue;
					} else {
						--size;
					}
				} else {
					g_buffer[size++] = c;
					if (size == g_buffer_size) {
						buffer_reserve(size);
					}
				}
				if (c == '\r') {
					c = '\n';
				}
				WriteConsole(output, &c, 1, NULL, NULL);
				if (c == '\n') {
					break;
				}
			}
		}
		g_buffer[size] = '\0';
		MessageBox(NULL, g_buffer, NULL, 0);
	}
}