#include "s2s.h"
#include "str.h"
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

#ifdef __cplusplus
extern "C" {
#endif

#define key_t         const char* 
#define value_t       const char* 
#define key_equ(a, b) str_equ(a, b)
#define key_dup(k)    (k)
#define value_dup(v)  strdup(v)
#define key_free(k)   
#define value_free(v) free((void*)(v))
#define key_null      null
#define value_null    null

#define map_t        s2s_t 
#define map_create   s2s_create 
#define map_get      s2s_get    
#define map_put      s2s_put    
#define map_put_all  s2s_put_all 
#define map_for_each s2s_for_each 
#define map_remove   s2s_remove 
#define map_size     s2s_size   
#define map_clear    s2s_clear  
#define map_dispose  s2s_dispose

#define map_init_static s2s_init_static

static inline int hashcode(key_t k) { return str_hashcode(k); }

#include "map.c"

#ifdef __cplusplus
}
#endif
