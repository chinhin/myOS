#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>
#include <kernel/gdt.h>

void kernel_main(void) {
	int i;

	terminal_initialize();
	init_debug();
	init_gdt();

	for(i=0; i<2; i++)
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
