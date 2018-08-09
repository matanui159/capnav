#include "sys.h"
#include <windows.h>

static UINT_PTR g_timer;
static char g_timer_key;

static LRESULT CALLBACK keyboard_proc(int code, WPARAM wpm, LPARAM lpm) {
	KBDLLHOOKSTRUCT* info = (KBDLLHOOKSTRUCT*)lpm;
	if (code == HC_ACTION && wpm <= WM_KEYUP && info->vkCode >= 'A' && info->vkCode <= 'Z') {
		sys_event_t event = SYS_EVENT_DOWN;
		if (wpm == WM_KEYUP) {
			event = SYS_EVENT_UP;
		}
		if (sys_key_event(info->vkCode, event)) {
			return 1;
		}
	}
	return CallNextHookEx(NULL, code, wpm, lpm);
}

static void CALLBACK timer_proc(HWND wnd, UINT msg, UINT_PTR id, DWORD time) {
	sys_key_event(g_timer_key, SYS_EVENT_TIMER);
}

static void send_input(WORD key, _Bool down) {
	INPUT input = {INPUT_KEYBOARD};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	if (!down) {
		input.ki.dwFlags = KEYEVENTF_KEYUP;
	}
	SendInput(1, &input, sizeof(INPUT));
}

static void send_press(WORD key) {
	send_input(key, 1);
	send_input(key, 0);
}

_Bool sys_key_caps() {
	return GetKeyState(VK_CAPITAL);
}

void sys_key_arrow(sys_arrow_t dir) {
	switch (dir) {
		case SYS_ARROW_LEFT:
			send_press(VK_LEFT);
			break;
		case SYS_ARROW_RIGHT:
			send_press(VK_RIGHT);
			break;
		case SYS_ARROW_UP:
			send_press(VK_UP);
			break;
		case SYS_ARROW_DOWN:
			send_press(VK_DOWN);
			break;
	}
}

void sys_key_action(char key) {
	send_press(VK_CAPITAL);
	send_input(VK_LCONTROL, 1);
	send_input(VK_LMENU, 1);
	send_press(key);
	send_input(VK_LMENU, 0);
	send_input(VK_LCONTROL, 0);
}

void sys_timer_set(char key, int timer) {
	g_timer_key = key;
	g_timer = SetTimer(NULL, g_timer, timer, timer_proc);
}

void sys_timer_kill() {
	if (g_timer != 0) {
		KillTimer(NULL, g_timer);
		g_timer = 0;
	}
}

void sys_entry() {
	SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0);
	for (;;) {
		MSG msg;
		GetMessage(&msg, NULL, 0, 0);
		DispatchMessage(&msg);
	}
}