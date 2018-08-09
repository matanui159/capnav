#ifndef SYS_H_
#define SYS_H_

typedef enum sys_dir_t {
	SYS_DIR_LEFT,
	SYS_DIR_RIGHT,
	SYS_DIR_UP,
	SYS_DIR_DOWN
} sys_dir_t;

typedef enum sys_event_t {
	SYS_EVENT_DOWN,
	SYS_EVENT_UP,
	SYS_EVENT_TIMER
} sys_event_t;

_Bool sys_caps();
void sys_move(sys_dir_t dir);
void sys_tset(char key, int timer);
void sys_tkill();

_Bool sys_key(char key, sys_event_t event);

#endif