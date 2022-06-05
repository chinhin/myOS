/*
 * =====================================================================================
 * entry.c
 * Description:  内核的入口函数
 * =====================================================================================
 */

#include "console.h"

int kern_entry()
{
	console_clear();

	console_write_color("Hello, OS kernel!\n", rc_blue, rc_red);

	return 0;
}

