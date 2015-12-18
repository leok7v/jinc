#include "i2i.h"
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

#define key_t         int64_t
#define value_t       int64_t 
#define key_equ(a, b) ((a) == (b))
#define key_dup(k)    (k)
#define value_dup(v)  (v)
#define key_free(k)   
#define value_free(v) 
#define key_null      0
#define value_null    0

#define map_t        i2i_t 
#define map_create   i2i_create 
#define map_get      i2i_get    
#define map_put      i2i_put    
#define map_put_all  i2i_put_all 
#define map_for_each i2i_for_each 
#define map_remove   i2i_remove 
#define map_size     i2i_size   
#define map_clear    i2i_clear  
#define map_dispose  i2i_dispose

#define map_init_static i2i_create_static

static int hashcode(int64_t k) {
    unsigned int h = (unsigned int)k ^ (unsigned int)(((uint64_t)k) >> 32);
    /* Doug Lea's supplemental secondaryHash function */
    h ^= (h >> 20) ^ (h >> 12);
    h ^= (h >> 7) ^ (h >> 4);
    return (int)(h & 0x7FFFFFFF);
}

#include "map.c"

#ifdef __cplusplus
}
#endif

