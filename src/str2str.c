#include "str2str.h"
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

#define key_t         str_t 
#define value_t       str_t 
#define key_equ(a, b) str_equ(a, b)
#define key_dup(k)    str_dup(k)
#define value_dup(v)  str_dup(v)
#define key_free(k)   str_free(k)
#define value_free(v) str_free(v)
#define key_null      null
#define value_null    null

#define map_t        str2str_t 
#define map_create   str2str_create 
#define map_get      str2str_get    
#define map_put      str2str_put    
#define map_put_all  str2str_put_all 
#define map_for_each str2str_for_each 
#define map_remove   str2str_remove 
#define map_size     str2str_size   
#define map_clear    str2str_clear  
#define map_dispose  str2str_dispose

#define map_init_static str2str_init_static

static inline int hashcode(key_t k) { return str_hash(k); }

#include "map.c"

#ifdef __cplusplus
}
#endif
