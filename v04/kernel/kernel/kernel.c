#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	int i;

	terminal_initialize();

	for(i=0; i<25; i++)
	{
		printf("Hello, kernel World!%d\n", i);
	}
}
