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

#ifndef NOP_TIMER_H
#define NOP_TIMER_H

#include <linux/kthread.h>
#include <linux/delay.h>

#define NOP_TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS) \
    if ( num_online_cpus() >= 2 ) {                                  \
        reset_count = true;                                         \
        while ( reset_count == true && !kthread_should_stop() ) {}  \
        for ( int i = 0; i < LOOP_ITERATIONS; i++ ) {               \
            FUNCTION_CALL;                                          \
        }                                                           \
        pr_info("NOP_%s: ", PRINT_NAME);                            \
        print_count = true;                                         \
        while ( print_count == true && !kthread_should_stop() ) {}  \
    } else {                                                        \
        pr_info("NOP_%s: 0\n", PRINT_NAME);                         \
    }

#define NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS, FLAGS) \
    SUSPEND_INTERRUPTS(FLAGS, NOP_TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS))

#define NOP_TIMING_LOOP_BOOT_FEATURE_CHECK(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS, FEATURE_NAME) \
    if ( boot_cpu_has(FEATURE_NAME) ) {                                                              \
        NOP_TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS)                                  \
    }

#define NOP_TIMING_LOOP_CHECK_FOR_KMALLOC(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS) \
    if ( vmem_aligned ) {                                                             \
        NOP_TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS)                   \
    } else {                                                                          \
        pr_info("NOP_%s: 0\n", PRINT_NAME);                                           \
    }


#endif
