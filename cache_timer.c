/*
 * Environmental Characterization & Response
 * Copyright (C) 2018 Assured Information Security, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "cache_timer.h"

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
        cache_timing_flush((char *) (vmem_aligned+offset));
        msleep(1);
        t = cache_timing_probe((char *) (vmem_aligned+offset));
        avg += t;
    }

    avg /= COUNT;

    pr_info("Average clock cycles at address 0x%lx (offset: 0x%x): %d\n", \
            (unsigned long) vmem_aligned+offset, offset, avg);

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
