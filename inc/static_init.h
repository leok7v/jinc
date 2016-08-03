#pragma once
#ifndef STATIC_INIT_H
#define STATIC_INIT_H
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

#include <stdio.h>
#include <stdlib.h>

#define _STATIC_INIT_EXPAND_(a) a
#define _STATIC_INIT_CONCATENATE_(a, b) _STATIC_INIT_EXPAND_(a) ## _STATIC_INIT_EXPAND_(b)
#define _STATIC_INIT_ _static_init_
#if defined(_MSC_VER)
    #pragma section(".CRT$XCU",read)
    #ifdef _WIN64
        #define _static_init_(f, c) static void f##c(void); __declspec(allocate(".CRT$XCU")) void (*f##c##_)(void) = f##c; __pragma(comment(linker,"/include:" #f #c "_")) static void f##c(void)
    #else
        #define _static_init_(f, c) static void f##c(void); __declspec(allocate(".CRT$XCU")) void (*f##c##_)(void) = f##c; __pragma(comment(linker,"/include:_" #f #c "_")) static void f##c(void)
    #endif
    #define static_init_(f, c) _static_init_(f, c)
    #define static_init static_init_(_STATIC_INIT_EXPAND_(_STATIC_INIT_),_STATIC_INIT_EXPAND_(__COUNTER__))
#else
    #define static_init_(f, c) __attribute__((constructor)) static void f##c (void)
    #define static_init static_init_(_STATIC_INIT_EXPAND_(_STATIC_INIT_),_STATIC_INIT_EXPAND_(__COUNTER__))
#endif

/* 
    same as Java:
    static {
        System.out.println("called before main()\n");
    }
    usage:
    static void after_main(void) { printf("called after main()\n"); }
    static_void() { printf("called before main\n"); atexit(after_main); }

    int main() {   
        printf("main()\n");
    }
*/

#endif /* STATIC_INIT_H */
