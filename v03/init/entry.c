/*
 * =====================================================================================
 * entry.c
 * Description:  内核的入口函数
 * =====================================================================================
 */

#include "console.h"
#include "debug.h"

int kern_entry()
{
	init_debug();

	console_clear();

	printk_color(rc_black, rc_green, "Hello, OS kernel!\n");
	
	print_cur_status();
	printk("glb_mboot_ptr=%p\n", glb_mboot_ptr);
	printk("mmap_length=%d\n", glb_mboot_ptr->mmap_length);
	printk("mmap_addr=%p\n", glb_mboot_ptr->mmap_addr);

	panic("test");

	return 0;
}

