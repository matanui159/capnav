#ifndef SYS_H_
#define SYS_H_

typedef enum sys_event_t {
	SYS_EVENT_DOWN,
	SYS_EVENT_UP,
	SYS_EVENT_TIMER
} sys_event_t;

typedef enum sys_arrow_t {
	SYS_ARROW_LEFT,
	SYS_ARROW_RIGHT,
	SYS_ARROW_UP,
	SYS_ARROW_DOWN
} sys_arrow_t;

_Bool sys_key_event(char key, sys_event_t event);

_Bool sys_key_caps();
void sys_key_arrow(sys_arrow_t dir);
void sys_key_action(char key);

void sys_timer_set(char key, int timer);
void sys_timer_kill();

#endif