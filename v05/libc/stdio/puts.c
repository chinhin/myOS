#include <stdio.h>

#if defined(__is_libk)
#include <kernel/tty.h>
#endif

void puts(const char* data) {
#if defined(__is_libk)
	terminal_writestring(data);
#else
	// TODO: Implement stdio and the write system call.
#endif
}
