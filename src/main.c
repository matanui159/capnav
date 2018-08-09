#include "sys/sys.h"

static _Bool g_repeat[26];
static char g_timer;

_Bool sys_key(char key, sys_event_t event) {
	_Bool repeat = 0;
	int index = key - 'A';
	if (event == SYS_EVENT_DOWN) {
		repeat = g_repeat[index];
		g_repeat[index] = 1;
	} else if (event == SYS_EVENT_UP) {
		g_repeat[index] = 0;
	}

	if (sys_caps()) {
		if (event != SYS_EVENT_UP) {
			switch (key) {
				case 'A':
					sys_move(SYS_DIR_LEFT);
					break;
				case 'D':
					sys_move(SYS_DIR_RIGHT);
					break;
				case 'W':
					sys_move(SYS_DIR_UP);
					break;
				case 'S':
					sys_move(SYS_DIR_DOWN);
					break;
			}
		}

		if (!repeat && (key == 'Q' || key == 'E')) {
			if (event == SYS_EVENT_UP) {
				if (key == g_timer) {
					sys_tkill();
				}
			} else if (key == 'Q') {
				sys_move(SYS_DIR_UP);
			} else {
				sys_move(SYS_DIR_DOWN);
			}

			if (event == SYS_EVENT_DOWN) {
				g_timer = key;
				sys_tset(key, 10);
			}
		}

		return 1;
	} else {
		return 0;
	}
}