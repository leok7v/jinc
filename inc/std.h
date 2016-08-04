#pragma once
#ifndef STD_H
#define STD_H
/*  Copyright (c) 2003-2013, Leo Kuznetsov
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

    * Neither the name of the {organization} nor the names of its
      contributors may be used to endorse or promote products derived from
      this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#ifdef _MSC_VER
#include <malloc.h>
#pragma warning(suppress: 6262) // excessive stack usage
#pragma warning(suppress: 6263) // using alloca in a loop
#pragma warning(suppress: 6255) // _alloca indicates failure by raising a stack overflow exception
#pragma warning(suppress: 4996) // This function or variable may be unsafe
#include <windows.h>
#define __BYTE_ORDER REG_DWORD
#define __BIG_ENDIAN REG_DWORD_BIG_ENDIAN
#define __LITTLE_ENDIAN REG_DWORD_LITTLE_ENDIAN
#elif defined(__MACH__)
#include <machine/endian.h>
#define __BUG_ENDIAN 1234
#define __LITTLE_ENDIAN 4321
#if defined(__BIG_ENDIAN__)
#define __BYTE_ORDER __BIG_ENDIAN
#elif defined(__LITTLE_ENDIAN__)
#define __BYTE_ORDER __LITTLE_ENDIAN
#else
#pragma(error, "endianess?")
#endif
#else
#include <endian.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _DEBUG
#define DEBUG _DEBUG
#endif

#define null NULL
#define boolean bool

#define stack_alloc(bytes) alloca(bytes)
#define stack_allocz(bytes) memsetz(alloca(bytes), 0, (bytes))
static inline void* memsetz(void* a, int byte, int size) { if (a != null) { memset(a, byte, size); } return a; }

#ifdef __GNUC__
#define thread_local_storage __thread
#ifndef __cplusplus
#include <stdbool.h>
#else
#define true    1
#define false   0
#endif
#else
#define thread_local_storage __declspec(thread)
#define snprintf _snprintf
#define stricmp  _stricmp
#define strdup   _strdup
#define gettid() ((int)GetCurrentThreadId())
#ifndef __cplusplus
#define bool    int
#define true    1
#define false   0
#endif

#endif // GCC / MSVC

/* static_init(unique_name) { ... } */

#ifdef _MSC_VER // "extern" because of: https://connect.microsoft.com/VisualStudio/Feedback/Details/1587892
#define static_init(n) void n(void); extern void (*_init_##n##_)(void); __pragma(section(".CRT$XCU", read)) __declspec(allocate(".CRT$XCU")) void (*_init_##n##_)(void) = n; void n(void)
#else
#define static_init(n) __attribute__((constructor)) static void n(void)
#endif

/*
same as in Java:
    static { System.out.println("called before main()\n"); }
sample usage: order is not guaranteed
    static void after_main1(void) { printf("called after main() 1\n"); }
    static void after_main2(void) { printf("called after main() 2\n"); }
    static_init(main_init1)       { printf("called before main 1\n"); atexit(after_main1); }
    static_init(main_init2)       { printf("called before main 2\n"); atexit(after_main2); }
    int main() { printf("main()\n"); return 0; }
*/

#ifdef __cplusplus
}
#endif

#endif /* STD_H */
