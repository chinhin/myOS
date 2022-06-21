#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>
#include "elf.h"

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* __restrict, ...);
void puts(const char*);

// 初始化 Debug 信息
void init_debug();

// 打印当前的段存器值
void print_cur_status();

#ifdef __cplusplus
}
#endif

#endif
