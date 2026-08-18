/* Ghidra type stubs -- included automatically by Synthex for compilation.
 * These map Ghidra's internal types to standard C types so decompiled
 * code can compile with gcc -c -fsyntax-only.
 */

#ifndef GHIDRA_TYPES_H
#define GHIDRA_TYPES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* Ghidra undefined types */
typedef unsigned char undefined;
typedef unsigned char undefined1;
typedef unsigned short undefined2;
typedef unsigned int undefined4;
typedef unsigned long long undefined8;
typedef unsigned char byte;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long longlong;
typedef unsigned long long ulonglong;
typedef int bool;
typedef int BOOL;

/* Windows types commonly seen in PE decompilation */
typedef void *PVOID;
typedef void *LPVOID;
typedef void *HANDLE;
typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef long LONG;
typedef int INT;
typedef unsigned int UINT;
typedef long long LONGLONG;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef void *HMODULE;
typedef void *HINSTANCE;
typedef void *HWND;
typedef int (*FARPROC)(void);
typedef void *LPTOP_LEVEL_EXCEPTION_FILTER;
typedef unsigned long long size_t_ghidra;

/* Ghidra-specific constructs */
#define LOCK() do {} while(0)
#define UNLOCK() do {} while(0)
#define CONCAT44(a,b) (((unsigned long long)(a) << 32) | (unsigned int)(b))
#define SUB84(x,n) ((unsigned int)((x) >> ((n)*8)))
#define SEXT48(x) ((long long)(int)(x))
#define ZEXT48(x) ((unsigned long long)(unsigned int)(x))

/* Thread-local storage / stack canary stubs */
#ifndef in_FS_OFFSET
static long long __ghidra_fs_base = 0;
#define in_FS_OFFSET __ghidra_fs_base
#endif

/* Common CRT stubs that Ghidra references */
#ifndef __stack_chk_fail
#define __stack_chk_fail() abort()
#endif

/* code type for function pointers */
typedef void code;

#endif /* GHIDRA_TYPES_H */
