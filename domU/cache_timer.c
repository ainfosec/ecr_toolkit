#ifndef CACHE_TIMER_H
#define CACHE_TIMER_H
#include "cache_timer.h"
#endif

unsigned long inline cache_timing_probe(char *addr) {
    volatile unsigned long time;

    asm volatile(
        "mfence\n"
        "rdtsc\n"
        "movl %%eax, %%esi\n"
        "movl (%1), %%eax\n"
        "lfence\n"
        "rdtsc\n"
        "subl %%esi, %%eax\n"
        : "=a"(time)
        : "c"(addr)
        : "%esi", "%edx"
    );
    return time;
}

void inline cache_timing_flush(char *addr) {
    asm volatile(
        "clflush 0(%0)\n"
        "mfence\n"
        :
        : "c"(addr)
        : "%rax"
    );
}

int cache_timing_handler(int offset) {
    unsigned long t;
    int i, avg = 0;
    extern uintptr_t *vmem_aligned;
  
    for ( i = 0; i < COUNT; i++ ) {
        cache_timing_flush((char *) (*vmem_aligned+offset));
        msleep(1);
        t = cache_timing_probe((char *) (*vmem_aligned+offset));
        avg += t;
    }

    avg /= COUNT;

    pr_info("Average clock cycles at address 0x%lx (offset: 0x%x): %d\n", \
            (unsigned long) *vmem_aligned+offset, offset, avg);

    return 0;
}

int cache_timing_start(void) {
    cache_timing_handler(0);
    cache_timing_handler(64);
    cache_timing_handler(128);
    cache_timing_handler(256);
    cache_timing_handler(4096);

    return 0;
}
