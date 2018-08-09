#include "sys.h"
#include <windows.h>

static char g_key;

static LRESULT CALLBACK keyboard_proc(int code, WPARAM wpm, LPARAM lpm) {
	if (GetKeyState(VK_CAPITAL) && code == HC_ACTION && wpm == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*)lpm;
		if (info->vkCode >= 'A' && info->vkCode <= 'Z') {
			g_key = info->vkCode;
			return 1;
		}
	}
	return CallNextHookEx(NULL, code, wpm, lpm);
}

// static void send_change(WORD key, _Bool up) {
// 	INPUT input = {INPUT_KEYBOARD};
// 	input.ki.wVk = key;
// 	if (up) {
// 		input.ki.dwFlags = KEYEVENTF_KEYUP;
// 	}
// 	SendInput(1, &input, sizeof(INPUT));
// }

static void send_press(WORD key) {
	INPUT inputs[2];
	inputs[0].type = INPUT_KEYBOARD;
	inputs[0].ki.wVk = key;
	inputs[0].ki.time = 0;
	inputs[0].ki.dwExtraInfo = 0;

	inputs[1] = inputs[0];
	inputs[0].ki.dwFlags = 0;
	inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(2, inputs, sizeof(INPUT));
}

static void CALLBACK timer_proc(HWND wnd, UINT msg, UINT_PTR id, DWORD time) {
	int x, y;
	sys_update(g_key, &x, &y);
	g_key = 0;

	while (x < 0) {
		send_press(VK_LEFT);
		++x;
	}
	while (x > 0) {
		send_press(VK_RIGHT);
		--x;
	}
	while (y < 0) {
		send_press(VK_UP);
		--y;
	}
	while (y > 0) {
		send_press(VK_DOWN);
		++y;
	}
}

void entry() {
	if (SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0) == NULL) {

		PTSTR error;
		DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		FormatMessage(flags, NULL, GetLastError(), 0, (LPSTR)&error, 0, NULL);
		MessageBox(NULL, error, NULL, MB_ICONERROR);
		LocalFree(error);
		ExitProcess(1);

	} else {
	
		SetTimer(NULL, 0, SYS_TIMER, timer_proc);
		for (;;) {
			MSG msg;
			GetMessage(&msg, NULL, 0, 0);
			DispatchMessage(&msg);
		}

	}
}