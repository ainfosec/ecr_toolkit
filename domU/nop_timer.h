#include <linux/kthread.h>
#include <linux/delay.h>

#ifndef MAIN_H
#define MAIN_H
#include "main.h"
#endif

#define NOP_TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS) \
    reset_count = true;                                             \
    while ( reset_count == true && !kthread_should_stop() ) {}      \
    for ( int i = 0; i < LOOP_ITERATIONS; i++ ) {                   \
        FUNCTION_CALL;                                              \
    }                                                               \
    pr_info("NOP_%s: ", PRINT_NAME);                                \
    print_count = true;                                             \
    while ( print_count == true && !kthread_should_stop() ) {}

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
