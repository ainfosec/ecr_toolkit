#ifndef WALL_TIMER_H
#define WALL_TIMER_H

extern uintptr_t *vmem_aligned;

#define TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS) \
    getnstimeofday(&ts_start);                                  \
    for ( int i = 0; i < LOOP_ITERATIONS; i++ ) {               \
        FUNCTION_CALL;                                          \
    }                                                           \
    getnstimeofday(&ts_end);                                    \
    test_of_time = timespec_sub(ts_end, ts_start);              \
    pr_info("WALL_%s: %lu\n", PRINT_NAME, test_of_time.tv_nsec);

#define TIMING_LOOP_SUSPEND_INTERRUPTS(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS, FLAGS) \
    SUSPEND_INTERRUPTS(FLAGS, TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS))

#define TIMING_LOOP_BOOT_FEATURE_CHECK(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS, FEATURE_NAME) \
    if ( boot_cpu_has(FEATURE_NAME) ) {                                                          \
        TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS)                                  \
    }

#define TIMING_LOOP_CHECK_FOR_KMALLOC(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS) \
    if ( vmem_aligned ) {                                                         \
        TIMING_LOOP(FUNCTION_CALL, PRINT_NAME, LOOP_ITERATIONS)                   \
    } else {                                                                      \
        pr_info("WALL_%s: 0\n", PRINT_NAME);                                      \
    }

int wall_timing_start(void);
int buffer_flush_start(void);

#endif
