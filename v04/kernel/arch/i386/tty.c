#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <kernel/tty.h>

#include "vga.h"
#include "common.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// 移动光标
static void move_cursor()
{
	// 屏幕是 80 字节宽
	uint16_t cursorLocation = terminal_row * VGA_WIDTH + terminal_column;

	// VGA 内部的寄存器多达300多个，显然无法一一映射到I/O端口的地址空间。
	// 对此 VGA 控制器的解决方案是，将一个端口作为内部寄存器的索引：0x3D4，
	// 再通过 0x3D5 端口来设置相应寄存器的值。
	// 在这里用到的两个内部寄存器的编号为14与15，分别表示光标位置的高8位与低8位。

	outb(0x3D4, 14);                  	// 告诉 VGA 我们要设置光标的高字节
	outb(0x3D5, cursorLocation >> 8); 	// 发送高 8 位
	outb(0x3D4, 15);                  	// 告诉 VGA 我们要设置光标的低字节
	outb(0x3D5, cursorLocation);     	// 发送低 8 位
}

// 屏幕滚动操作
static void scroll()
{
	// cursor_y 到 25 的时候，就该换行了
	if (terminal_row >= VGA_HEIGHT) {
		// 将所有行的显示数据复制到上一行，第一行永远消失了...
		for (size_t y = 0; y < (VGA_HEIGHT - 1); y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index_new = y * VGA_WIDTH + x;
				const size_t index_org = (y + 1) * VGA_WIDTH + x;
				terminal_buffer[index_new] = terminal_buffer[index_org];
			}
		}

		// 最后的一行数据现在填充空格，不显示任何字符
		for (size_t y = (VGA_HEIGHT - 1); y < VGA_HEIGHT; y++) {
			for (size_t x = 0; x < VGA_WIDTH; x++) {
				const size_t index = y * VGA_WIDTH + x;
				terminal_buffer[index] = vga_entry(' ', terminal_color);
			}
		}

		// 向上移动了一行，所以 cursor_y 现在是 24
		terminal_row = (VGA_HEIGHT - 1);
	}
}

// 清屏操作
void console_clear()
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}

	terminal_row = 0;
	terminal_column = 0;
	move_cursor();
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}

void terminal_initialize(void) {
	terminal_buffer = VGA_MEMORY;
	terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
	console_clear();
}

void terminal_putchar(char c) {
	// 0x08 是 退格键 的 ASCII 码
	// 0x09 是 tab 键 的 ASCII 码
	if (c == 0x08 && terminal_column) {
	      terminal_column--;
	} else if (c == 0x09) {
	      terminal_column = (terminal_column + 8) & ~(8 - 1);
	} else if (c == '\r') {
	      terminal_column = 0;
	} else if (c == '\n') {
		terminal_column = 0;
		terminal_row++;
	} else if (c >= ' ') {
		unsigned char uc = c;
		const size_t index = terminal_row * VGA_WIDTH + terminal_column;
		terminal_buffer[index] = vga_entry(uc, terminal_color);
		terminal_column++;
	}

	// 每 80 个字符一行，满80就必须换行了
	if (terminal_column >= VGA_WIDTH) {
		terminal_column = 0;
		terminal_row++;
	}

	// 如果需要的话滚动屏幕显示
	scroll();

	// 移动硬件的输入光标
	move_cursor();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}

// 屏幕输出一个十六进制的整型数
void console_write_hex(uint32_t n)
{
	int tmp;
	char noZeroes = 1;

	terminal_writestring("0x");

	int i;
	for (i = 28; i >= 0; i -= 4) {
		tmp = (n >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) {
		      continue;
		}
		noZeroes = 0;
		if (tmp >= 0xA) {
		      terminal_putchar(tmp - 0xA + 'a');
		} else {
		      terminal_putchar(tmp + '0');
		}
	}
}

// 屏幕输出一个十进制的整型数
void console_write_dec(uint32_t n)
{
	if (n == 0) {
		terminal_putchar('0');
		return;
	}

	uint32_t acc = n;
	char c[32];
	int i = 0;
	while (acc > 0) {
		c[i] = '0' + acc % 10;
		acc /= 10;
		i++;
	}
	c[i] = 0;

	char c2[32];
	c2[i--] = 0;

	int j = 0;
	while(i >= 0) {
	      c2[i--] = c[j++];
	}

	terminal_writestring(c2);
}
