#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

// 屏幕输出一个十六进制的整型数
void console_write_hex(uint32_t n);

// 屏幕输出一个十进制的整型数
void console_write_dec(uint32_t n);

#endif
