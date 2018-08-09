#include "sys.h"
#include <windows.h>

static UINT_PTR g_timer;
static char g_key;

static LRESULT CALLBACK keyboard_proc(int code, WPARAM wpm, LPARAM lpm) {
	KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*)lpm;
	if (code == HC_ACTION && wpm <= WM_KEYUP && info->vkCode >= 'A' && info->vkCode <= 'Z') {
		sys_event_t event = SYS_EVENT_DOWN;
		if (wpm == WM_KEYUP) {
			event = SYS_EVENT_UP;
		}
		if (sys_key(info->vkCode, event)) {
			return 1;
		}
	}
	return CallNextHookEx(NULL, code, wpm, lpm);
}

static void CALLBACK timer_proc(HWND wnd, UINT msg, UINT_PTR id, DWORD time) {
	sys_key(g_key, SYS_EVENT_TIMER);
}

_Bool sys_caps() {
	return GetKeyState(VK_CAPITAL);
}

void sys_move(sys_dir_t dir) {
	WORD key = 0;
	switch (dir) {
		case SYS_DIR_LEFT:
			key = VK_LEFT;
			break;
		case SYS_DIR_RIGHT:
			key = VK_RIGHT;
			break;
		case SYS_DIR_UP:
			key = VK_UP;
			break;
		case SYS_DIR_DOWN:
			key = VK_DOWN;
			break;
	}

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

void sys_tset(char key, int timer) {
	g_key = key;
	g_timer = SetTimer(NULL, g_timer, timer, timer_proc);
}

void sys_tkill() {
	if (g_timer != 0) {
		KillTimer(NULL, g_timer);
		g_timer = 0;
	}
}

void sys_entry() {
	if (SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0) == NULL) {

		PTSTR error;
		DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
		FormatMessage(flags, NULL, GetLastError(), 0, (LPSTR)&error, 0, NULL);
		MessageBox(NULL, error, NULL, MB_ICONERROR);
		LocalFree(error);
		ExitProcess(1);

	} else {
	
		for (;;) {
			MSG msg;
			GetMessage(&msg, NULL, 0, 0);
			DispatchMessage(&msg);
		}

	}
}