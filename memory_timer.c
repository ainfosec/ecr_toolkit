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

#include "main.h"
#include "memory_timer.h"

struct page_struct {
    char bytes[PAGE_SIZE];
};

unsigned long memory_timing_probe(char *addr) {
    volatile unsigned long time;

    asm volatile(
        "mfence\n"
        "lfence\n"
        "rdtsc\n"
        "lfence\n"
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

int memory_timing_start(void) {
    unsigned long t;
    int i;
    extern uintptr_t *vmem_aligned;
  
    void *ptv = vmem_aligned;
  
    struct page_struct *array = ptv;
  
    for ( i = 0; i < 10; i++ ) {
        t = memory_timing_probe((char *) &array[i]);
        t = memory_timing_probe((char *) &array[i]);
        pr_info("Cycles to access page %d: %d\n", i, (int)t);
    }
  
    return 0;
}
