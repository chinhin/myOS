#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>

void kernel_main(void) {
	int i;

	init_debug();
	terminal_initialize();

	for(i=0; i<12; i++)
	{
		printf("Hello %2d, kernel World!\n", i);
	}

	console_write_hex(22);
	printf("\n");
	console_write_dec(22);
	printf("\n");
	print_cur_status();
	printf("mmap_length=%d\n", glb_mboot_ptr->mmap_length);
	printf("mmap_addr=%p\n", glb_mboot_ptr->mmap_addr);
	abort();
}
