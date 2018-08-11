#ifndef IO_H_
#define IO_H_
#include <windows.h>

#define IO_WRITE(text, color) (io_write(TEXT(text), sizeof(text) - 1, (color)))
#define IO_ERROR(text, color) (io_error(TEXT(text), sizeof(text) - 1, (color)))

void io_init();
void io_read(PTSTR text, int size, char color);
void io_write(PTSTR text, int size, char color);
void io_error(PTSTR text, int size, char color);

#endif