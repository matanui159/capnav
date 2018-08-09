#include "sys/sys.h"

static _Bool g_repeat[26];
static char g_timer;

_Bool sys_key_event(char key, sys_event_t event) {
	_Bool repeat = 0;
	int index = key - 'A';
	if (event == SYS_EVENT_DOWN) {
		repeat = g_repeat[index];
		g_repeat[index] = 1;
	} else if (event == SYS_EVENT_UP) {
		g_repeat[index] = 0;
	}

	if (sys_key_caps()) {
		if (event != SYS_EVENT_UP) {
			switch (key) {
				case 'A':
					sys_key_arrow(SYS_ARROW_LEFT);
					break;
				case 'D':
					sys_key_arrow(SYS_ARROW_RIGHT);
					break;
				case 'W':
					sys_key_arrow(SYS_ARROW_UP);
					break;
				case 'S':
					sys_key_arrow(SYS_ARROW_DOWN);
					break;
			}
		}

		if (!repeat && (key == 'Q' || key == 'E')) {
			if (event == SYS_EVENT_UP) {
				if (key == g_timer) {
					sys_timer_kill();
				}
			} else if (key == 'Q') {
				sys_key_arrow(SYS_ARROW_UP);
			} else {
				sys_key_arrow(SYS_ARROW_DOWN);
			}

			if (event == SYS_EVENT_DOWN) {
				g_timer = key;
				sys_timer_set(key, 10);
			}
		}

		if (!repeat && (key == 'F' || key == 'R')) {
			sys_key_action(key);
		}

		return 1;
	} else {
		return 0;
	}
}