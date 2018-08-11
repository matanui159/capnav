#include <windows.h>

#define TIMER 10

#define KEY_DOWN(key) (GetKeyState(key) & 0x8000)
#define KEY_TOGGLED(key) (GetKeyState(key) & 0x0001)
#define KEY_INTERCEPT() (KEY_TOGGLED(VK_CAPITAL) && !KEY_DOWN(VK_CONTROL) && !KEY_DOWN(VK_MENU))

static void send_input(WORD key, _Bool up) {
	INPUT input = {INPUT_KEYBOARD};
	input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	if (up) {
		input.ki.dwFlags |= KEYEVENTF_KEYUP;
	}
	SendInput(1, &input, sizeof(INPUT));
}

static void send_press(WORD key) {
	send_input(key, 0);
	send_input(key, 1);
}

static DWORD g_repeat;
static void CALLBACK timer_proc(HWND wnd, UINT msg, UINT_PTR id, DWORD time) {
	if (KEY_INTERCEPT()) {
		switch (g_repeat) {
			case 'Q':
				send_press(VK_UP);
				break;
			case 'E':
				send_press(VK_DOWN);
				break;
		}
	}
}

static LRESULT CALLBACK keyboard_proc(int code, WPARAM wpm, LPARAM lpm) {
	static UINT_PTR timer;
	static _Bool numlock;
	DWORD key = ((KBDLLHOOKSTRUCT*)lpm)->vkCode;
	if (code == HC_ACTION && (wpm == WM_KEYDOWN || wpm == WM_KEYUP)) {
		if (key == VK_CAPITAL && wpm == WM_KEYDOWN) {
			if (KEY_TOGGLED(VK_CAPITAL)) {
				if (numlock && !KEY_TOGGLED(VK_NUMLOCK)) {
					send_press(VK_NUMLOCK);
				}
			} else {
				numlock = KEY_TOGGLED(VK_NUMLOCK);
				if (numlock) {
					send_press(VK_NUMLOCK);
				}
			}
		}

		if (key == 'Q' || key == 'E') {
			if (wpm == WM_KEYDOWN) {
				if (key != g_repeat) {
					g_repeat = key;
					timer_proc(NULL, 0, 0, 0);
					timer = SetTimer(NULL, timer, TIMER, timer_proc);
				}
			} else if (key == g_repeat) {
				KillTimer(NULL, timer);
				timer = 0;
				g_repeat = 0;
			}
		}

		if (key >= 'A' && key <= 'Z' && KEY_INTERCEPT()) {
			if (wpm == WM_KEYDOWN) {
				switch (key) {
					case 'A':
						send_press(VK_LEFT);
						break;
					case 'D':
						send_press(VK_RIGHT);
						break;
					case 'W':
						send_press(VK_UP);
						break;
					case 'S':
						send_press(VK_DOWN);
						break;
					case 'F':
					case 'R':
						send_press(VK_CAPITAL);
						send_input(VK_CONTROL, 0);
						send_input(VK_MENU, 0);
						send_press(key);
						send_input(VK_MENU, 1);
						send_input(VK_CONTROL, 1);
						break;
				}
			}
			return 1;
		}
	}
	return CallNextHookEx(NULL, code, wpm, lpm);
}

void entry() {
	SetWindowsHookEx(WH_KEYBOARD_LL, keyboard_proc, NULL, 0);
	for (;;) {
		MSG msg;
		GetMessage(&msg, NULL, 0, 0);
		DispatchMessage(&msg);
	}
}