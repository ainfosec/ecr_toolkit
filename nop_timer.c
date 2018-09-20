#include "main.h"
#include "nop_timer.h"

struct task_struct *nop_th;
struct task_struct *instr_th;
unsigned long count;
bool reset_count;
bool print_count;
bool finish;

int nop_f(void *unused) {
    allow_signal(SIGKILL);
    /* Run NOP thread until signalled to stop by module cleanup */
    while ( !kthread_should_stop() ) {
        /* Check for thread kill signal */
        if ( signal_pending(nop_th) )
            break;
        /* Check for reset counter signal from INSTR thread */
        if ( reset_count == true ) {
            count = 0;
            reset_count = false;
        }
        /* Check for print counter signal from INSTR thread */
        if ( print_count == true ) {
            pr_info("%lu\n", count);
            print_count = false;
        }
        /* Run NOP command and increment counter */
        asm volatile ("nop");
        count++;
    }

    do_exit(0);
    return 0;
}

int instr_f(void *unused) {
    unsigned long flags;
    extern uintptr_t *vmem_aligned;

    allow_signal(SIGKILL);

    /* Run series of instruction timings */
    pr_info("Aggregating NOP based timings\n");
    NOP_TIMING_LOOP(xor_f(), "XOR", 100)
    NOP_TIMING_LOOP(cpuid_f(), "CPUID", 100)
    NOP_TIMING_LOOP(rdtsc_f(), "RDTSC", 100)
    NOP_TIMING_LOOP(rdtscp_f(), "RDTSCP", 100)
    NOP_TIMING_LOOP(invd_f(), "INVD", 100)
    NOP_TIMING_LOOP(clts_f(), "CLTS", 100)
    NOP_TIMING_LOOP(inb_f(), "INB", 100)
    NOP_TIMING_LOOP(outb_f(), "OUTB", 100)
    NOP_TIMING_LOOP(invlpg_f(), "INVLPG", 100)
    /* NOP_TIMING_LOOP(invpcid_f(), "INVPCID", 100) */
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_from_cr8_f(), "MOV_FROM_CR8", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr0_f(), "MOV_TO_CR0", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr3_f(), "MOV_TO_CR3", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr4_f(), "MOV_TO_CR4", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_to_cr8_f(), "MOV_TO_CR8", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr0_f(), "MOV_DR0", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr1_f(), "MOV_DR1", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr2_f(), "MOV_DR2", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr3_f(), "MOV_DR3", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr4_f(), "MOV_DR4", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr5_f(), "MOV_DR5", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr6_f(), "MOV_DR6", 100, flags)
    NOP_TIMING_LOOP_SUSPEND_INTERRUPTS(mov_dr7_f(), "MOV_DR7", 100, flags)
    NOP_TIMING_LOOP(rdmsr_f(), "RDMSR", 100)
    NOP_TIMING_LOOP(rdpmc_f(), "RDPMC", 100)
    NOP_TIMING_LOOP_BOOT_FEATURE_CHECK(rdrand_f(), "RDRAND", 100, X86_FEATURE_RDRAND)
    NOP_TIMING_LOOP_BOOT_FEATURE_CHECK(rdseed_f(), "RDSEED", 100, X86_FEATURE_RDSEED)
    NOP_TIMING_LOOP(wbinvd_f(), "WBINVD", 100)
    NOP_TIMING_LOOP(wrmsr_f(), "WRMSR", 100)
    NOP_TIMING_LOOP_CHECK_FOR_KMALLOC(movntdqa_f(), "MOVNTDQA", 100)
    NOP_TIMING_LOOP_CHECK_FOR_KMALLOC(movntdq_f(), "MOVNTDQ", 100)

    finish = true;
    do_exit(0);
    return 0;
}

int buffer_flush_instr_f(void *unused) {

    allow_signal(SIGKILL);

    /* Run series of instruction timings */
    pr_info("Aggregating buffer flush NOP based timing\n");

    NOP_TIMING_LOOP_CHECK_FOR_KMALLOC(clflush_f(), "CLFLUSH", 100)
    NOP_TIMING_LOOP(sfence_f(), "SFENCE", 100)
    NOP_TIMING_LOOP(mfence_f(), "MFENCE", 100)

    finish = true;
    do_exit(0);
    return 0;
}

int nop_timing_start(bool action) {
    count = 0;
    reset_count = false;
    print_count = false;
    finish = false;

    /* Create NOP thread and INSTR thread */
    nop_th = kthread_create(nop_f, NULL, "nop");
    if ( action ) {
        instr_th = kthread_create(instr_f, NULL, "instr");
    } else {
        instr_th = kthread_create(buffer_flush_instr_f, NULL, "instr");
    }
    if ( nop_th && instr_th ) {
        /* Bind threads to separate CPUs */
        kthread_bind(nop_th, 0);
        kthread_bind(instr_th, 1);
        /* Start running threads */
        wake_up_process(nop_th);
        wake_up_process(instr_th);
    }

    /* Wait until INSTR thread has finished */
    while ( finish == false ) {
        ssleep(1);
    }
    /* Send stop signals to threads if not already finished */
    kthread_stop(nop_th);
    kthread_stop(instr_th);
    return 0;
}
