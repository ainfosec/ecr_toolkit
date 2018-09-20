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
#include "wall_timer.h"

int wall_timing_start(void) {
    struct timespec ts_start, ts_end, test_of_time;
    unsigned long flags;

    cpuid_test_support_f();

    pr_info("Aggregating wall clock timing\n");

    TIMING_LOOP(xor_f(), "XOR", 100)
    TIMING_LOOP(cpuid_f(), "CPUID", 100)
    TIMING_LOOP(rdtsc_f(), "RDTSC", 100)
    TIMING_LOOP(rdtscp_f(), "RDTSCP", 100)
    TIMING_LOOP(invd_f(), "INVD", 100)
    TIMING_LOOP(clts_f(), "CLTS", 100)
    TIMING_LOOP(inb_f(), "INB", 100)
    TIMING_LOOP(outb_f(), "OUTB", 100)
    TIMING_LOOP(invlpg_f(), "INVLPG", 100)
    /* TIMING_LOOP(invpcid_f(), "INVPCID", 100) */
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_from_cr8_f(), "MOV_FROM_CR8", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr0_f(), "MOV_TO_CR0", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr3_f(), "MOV_TO_CR3", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr4_f(), "MOV_TO_CR4", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr8_f(), "MOV_TO_CR8", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr0_f(), "MOV_DR0", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr1_f(), "MOV_DR1", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr2_f(), "MOV_DR2", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr3_f(), "MOV_DR3", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr4_f(), "MOV_DR4", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr5_f(), "MOV_DR5", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr6_f(), "MOV_DR6", 100, flags)
    TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr7_f(), "MOV_DR7", 100, flags)
    TIMING_LOOP(rdmsr_f(), "RDMSR", 100)
    TIMING_LOOP(rdpmc_f(), "RDPMC", 100)
    TIMING_LOOP_BOOT_FEATURE_CHECK(rdrand_f(), "RDRAND", 100, X86_FEATURE_RDRAND)
    TIMING_LOOP_BOOT_FEATURE_CHECK(rdseed_f(), "RDSEED", 100, X86_FEATURE_RDSEED)
    TIMING_LOOP(wbinvd_f(), "WBINVD", 100)
    TIMING_LOOP(wrmsr_f(), "WRMSR", 100)
    TIMING_LOOP_CHECK_FOR_KMALLOC(movntdqa_f(), "MOVNTDQA", 100)
    TIMING_LOOP_CHECK_FOR_KMALLOC(movntdq_f(), "MOVNTDQ", 100)

    return 0;
}

int buffer_flush_start(void) {
    struct timespec ts_start, ts_end, test_of_time;

    pr_info("Aggregating buffer flush wall clock timing\n");

    TIMING_LOOP_CHECK_FOR_KMALLOC(clflush_f(), "CLFLUSH", 100)
    TIMING_LOOP(sfence_f(), "SFENCE", 100)
    TIMING_LOOP(mfence_f(), "MFENCE", 100)

    return 0;
}
