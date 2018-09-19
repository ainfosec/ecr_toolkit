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
