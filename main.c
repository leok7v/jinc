#include "std.h"
#include "str.h"
#include "str2str.h"
#include "i2i.h"
#include "s2i.h"
#include "clock.h"
#include "static_init.h"
#if defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))
#define _CRTDBG_MAP_ALLOC
#undef strdup
#undef _malloca
#include <crtdbg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

static char timestamp_storage[16 * 1024];
static s2i_t timestamps;

void timestamp(const char *label) {
    if (timestamps == null) {
        timestamps = s2i_init_static(timestamp_storage, sizeof(timestamp_storage));
    }
    assert(sizeof(double) <= sizeof(int64_t));
    int64_t t = s2i_get(timestamps, label);
    double time = clock_in_milliseconds();
    if (t == 0) {
        s2i_put(timestamps, label, *(int64_t *) &time);
    } else {
        s2i_remove(timestamps, label);
        time = time - *(double *) &t;
        printf("%s %9.6f milliseconds\n", label, time);
    }
}

static_const_str(static_string, "foo");

static void print_str(const char *label, str_t s) {
    printf("%s \"%s\" length=%d bar hash=%d\n", label, s, str_length(s), str_hash(s));
}

static void str_test() {
    print_str("str_empty", str_empty);
    print_str("static_string", static_string);
    str_t stack = str_stack_dup(static_string);
    print_str("stack", stack);
    str_t heap = str_dup(stack);
    print_str("heap", heap);
    str_free(heap);
    heap = null;
    str_t stack_concat = str_stack_concat("foo", "bar");
    print_str("stack_concat", stack_concat);
    str_t heap_concat = str_concat("foo", "bar");
    print_str("heap_concat", heap_concat);
    str_t stack_foo = str_stack_substring(stack_concat, 0, 3);
    print_str("stack_foo", stack_foo);
    str_t stack_bar = str_stack_substring(stack_concat, 3, 6);
    print_str("stack_bar", stack_bar);
    str_t heap_foo = str_substring(heap_concat, 0, 3);
    print_str("heap_foo", heap_foo);
    str_t heap_bar = str_substring(heap_concat, 3, 6);
    print_str("heap_bar", heap_bar);
    str_t heap_empty = str_substring(heap_concat, 0, 0);
    print_str("heap_empty", heap_empty);
    assert(heap_empty == str_empty);
    str_free(heap_foo);
    heap_foo = null;
    str_free(heap_bar);
    heap_bar = null;
    str_free(heap_empty);
    heap_empty = null;
    str_free(heap_concat);
    heap_concat = null;
    printf("%s\n", str_stack_printf("Hello %s", "World!"));
}

void each(void *that, str2str_t m, str_t k) {
    printf("%s=%s\n", k, str2str_get(m, k));
    (void) that;
}

static void str2str_map_test() {
    str2str_t m = str2str_create(128, true);
    static_const_str(SUNDAY, "Sunday");
    static_const_str(MONDAY, "Monday");
    static_const_str(TUESDAY, "Tuesday");
    static_const_str(WEDNESDAY, "Wednesday");
    static_const_str(THURSDAY, "Thursday");
    static_const_str(FRIDAY, "Friday");
    static_const_str(SATURDAY, "Saturday");
    str2str_put(m, SUNDAY, "domingo");
    str2str_put(m, MONDAY, "lunes");
    str2str_put(m, TUESDAY, "martes");
    str2str_put(m, WEDNESDAY, "miércoles");
    str2str_put(m, THURSDAY, "jueves");
    str2str_put(m, FRIDAY, "viernes");
    str2str_put(m, SATURDAY, "sábado");
    str_t foo = str_stack_dup("foo");
    str2str_put(m, foo, "bar");
    printf("%s=%s\n", foo, str2str_get(m, foo));
    str2str_for_each(null, m, each);
    str2str_remove(m, foo);
    str2str_for_each(null, m, each);
    str2str_put(m, foo, "bar");
    str2str_for_each(null, m, each);
    str2str_put(m, foo, "xxx");
    str2str_for_each(null, m, each);
    str2str_t m1 = str2str_create(128, true);
    str2str_put_all(m1, m);
    str2str_dispose(m);
    str2str_for_each(null, m1, each);
    str2str_clear(m1);
    assert(str2str_size(m1) == 0);
    str2str_dispose(m1);

    enum {
        N = 1000 * 1000
    };
    i2i_t map = i2i_create(N * 2, true);
    timestamp("1,000,000 i2i_put");
    for (int i = 0; i < N * 3 / 4; i++) {
        i2i_put(map, i * 3 + 1, i * 13 + 1);
    }
    timestamp("1,000,000 i2i_put");
    timestamp("1,000,000 i2i_get");
    for (int i = 0; i < N * 3 / 4; i++) {
        int64_t v = i2i_get(map, i * 3 + 1);
        assert(v == i * 13 + 1);
        (void) v;
    }
    timestamp("1,000,000 i2i_get");
    i2i_dispose(map);
}

static void after_main1(void) { printf("called after main() 1\n"); }
static void after_main2(void) { printf("called after main() 2\n"); }

static_init(before_main) { printf("called before main 1\n"); atexit(after_main1); }
static_init(before_main) { printf("called before main 2\n"); atexit(after_main2); }

#if defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))
static _CrtMemState s1;
#endif

static void after_main(void) { 
#if defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))
    //  void* intentional_leak_for_testing = malloc(153); (void)intentional_leak_for_testing;
        _CrtMemState s2, s3;
        _CrtMemCheckpoint(&s2);
        if (_CrtMemDifference(&s3, &s1, &s2)) {
            _CrtMemDumpStatistics(&s3);
            // we want to dump memory leaks earlier to enable memory
            // examination when we heat breakpoint below
            _CrtDumpMemoryLeaks();
            _CrtSetDbgFlag(0); // not to dump leaks twice
            __debugbreak();    // this is your chance to examine leaked memory in Alt+6 Debugger Memory View pane
        }
#endif
}

static_init(before_main) { 
#if defined(_MSC_VER) && (defined(DEBUG) || defined(_DEBUG))
    _CrtMemCheckpoint(&s1);
    _CrtSetDbgFlag((_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF) & ~_CRTDBG_CHECK_CRT_DF);
//  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_CRT_DF | _CRTDBG_CHECK_ALWAYS_DF);
/*
    _crtBreakAlloc = 241; // {241} crt block at 0x012B13E8, subtype 0, 24 bytes long
    _CrtSetBreakAlloc(241);
*/
#endif
    atexit(after_main); 
}

int main(int argc, const char **argv) {
    (void) argc; (void) argv; // unused
    str_test();
    str2str_map_test();
    void* intentional_leak = malloc(1); (void)intentional_leak;
    getchar();
    return 0;
}

#ifdef __cplusplus
} // extern "C"
#endif
