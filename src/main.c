#include <windows.h>

#define TIMER 10

static void send_input(WORD key, _Bool up) {
	INPUT input = {INPUT_KEYBOARD};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key;
	if (up) {
		input.ki.dwFlags = KEYEVENTF_KEYUP;
	}
	SendInput(1, &input, sizeof(INPUT));
}

static void send_press(WORD key) {
	send_input(key, 0);
	send_input(key, 1);
}

static void send_action(WORD key) {
	send_press(VK_CAPITAL);
	send_input(VK_LCONTROL, 0);
	send_input(VK_LMENU, 0);
	send_press(key);
	send_input(VK_LMENU, 1);
	send_input(VK_LCONTROL, 1);
}

static char g_key;
static LRESULT CALLBACK keyboard_proc(int code, WPARAM wpm, LPARAM lpm) {
	static UINT_PTR timer;

	char key = ((KBDLLHOOKSTRUCT*)lpm)->vkCode;
	if (code == HC_ACTION && wpm <= WM_KEYUP && key >= 'A' && key <= 'Z') {
		if (wpm == WM_KEYDOWN) {
			g_key = key;
		} else if (key == g_key) {
			g_key = 0;
		}

		if (GetKeyState(VK_CAPITAL)) {
			if (wpm == WM_KEYDOWN) {
				switch (key) {
					case 'A':
						send_press(VK_LEFT);
						break;
					case 'D':
						send_press(VK_RIGHT);
						break;
					case 'W':
					case 'Q':
						send_press(VK_UP);
						break;
					case 'S':
					case 'E':
						send_press(VK_DOWN);
						break;
					case 'F':
					case 'R':
						send_action(key);
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
	DWORD prev = GetTickCount();
	for (;;) {
		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			DispatchMessage(&msg);
		}

		DWORD next = GetTickCount();
		while (next - prev >= TIMER) {
			switch (g_key) {
				case 'Q':
					send_press(VK_UP);
					break;
				case 'E':
					send_press(VK_DOWN);
					break;
			}
			prev += TIMER;
		}
	}
}