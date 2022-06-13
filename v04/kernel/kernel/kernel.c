#include <stdio.h>

#include <kernel/tty.h>

void kernel_main(void) {
	int i;

	terminal_initialize();

	for(i=0; i<25; i++)
	{
		printf("Hello %2d, kernel World!\n", i);
	}

	console_write_hex(22);
	printf("\n");
	console_write_dec(22);
	printf("\n");
}
