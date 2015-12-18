#include "std.h"
#include "clock.h"
#ifdef __cplusplus
extern "C" {
#endif
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

#ifdef _MSC_VER

#include <profileapi.h>

static LONGLONG freq = 0;
static LONGLONG start = 0;

double clock_in_milliseconds() {
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    if (freq == 0) {
        LARGE_INTEGER f;
        QueryPerformanceFrequency(&f);
        freq = f.QuadPart;
        start = li.QuadPart - freq; /* make sure all times > 0 */
    }
    double milliseconds = (li.QuadPart - start) * 1000.0 / freq;
    assert(milliseconds > 0);
    return milliseconds;
}

#else

#include <unistd.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#define CLOCK_MONOTONIC SYSTEM_CLOCK   // ~ 3ms resolution

static clock_serv_t cclock;

static void osx_clock_gettime(struct timespec* ts) {
    mach_timespec_t mts;
    clock_get_time(cclock, &mts);
    ts->tv_sec = mts.tv_sec;
    ts->tv_nsec = mts.tv_nsec;
}

__attribute__ ((constructor))
static void ctor() {
    host_get_clock_service(mach_host_self(), CLOCK_MONOTONIC, &cclock);
}
__attribute__ ((destructor))
extern void dtor() {
    mach_port_deallocate(mach_task_self(), cclock);
}

#endif

uint64_t nanotime() {
    struct timespec tm = {0};
#if defined(CLOCK_REALTIME_HR)
    clock_gettime(CLOCK_REALTIME_HR, &tm);
#elif defined(CLOCK_REALTIME)
    clock_gettime(CLOCK_REALTIME, &tm);
#elif defined(__MACH__)
    osx_clock_gettime(&tm);
#else
    clock_gettime(CLOCK_REALTIME, &tm);
#endif
    return NANOSECONDS_IN_SECOND * (uint64_t)tm.tv_sec + tm.tv_nsec;
}

static uint64_t start = 0;

double clock_in_milliseconds() {
    uint64_t ns = nanotime();
    if (start == 0) {        
        start = ns - 1; /* make sure all times > 0 */
    }
    return (ns - start) / (double)NANOSECONDS_IN_MILLISECOND;
}

#endif

#ifdef __cplusplus
}
#endif
