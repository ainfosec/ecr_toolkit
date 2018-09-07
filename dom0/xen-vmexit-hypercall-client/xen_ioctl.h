/* Exec Control Types */
#define PIN_BASED_VM_EXEC_CONTROL                0x00004000
#define CPU_BASED_VM_EXEC_CONTROL                0x00004002
#define SECONDARY_VM_EXEC_CONTROL                0x0000401e

typedef struct privcmd_hypercall_t {
    __u64 op;
    __u64 arg[5];
} privcmd_hypercall;

typedef struct {
    char* name;
    unsigned long mask;
} vmcs_bit;

vmcs_bit primary_exec[16];
vmcs_bit secondary_exec[10];

void build_vmcs() {

    primary_exec[0].name = "CPU_BASED_HLT_EXITING";
    primary_exec[0].mask = 0x00000080;
    primary_exec[1].name = "CPU_BASED_INVLPG_EXITING";
    primary_exec[1].mask = 0x00000200;
    primary_exec[2].name = "CPU_BASED_MWAIT_EXITING";
    primary_exec[2].mask = 0x00000400;
    primary_exec[3].name = "CPU_BASED_RDPMC_EXITING";
    primary_exec[3].mask = 0x00000800;
    primary_exec[4].name = "CPU_BASED_RDTSC_EXITING";
    primary_exec[4].mask = 0x00001000;
    primary_exec[5].name = "CPU_BASED_CR3_LOAD_EXITING";
    primary_exec[5].mask = 0x00008000;
    primary_exec[6].name = "CPU_BASED_CR3_STORE_EXITING";
    primary_exec[6].mask = 0x00010000;
    primary_exec[7].name = "CPU_BASED_CR8_LOAD_EXITING";
    primary_exec[7].mask = 0x00080000;
    primary_exec[8].name = "CPU_BASED_CR8_STORE_EXITING";
    primary_exec[8].mask = 0x00100000;
    primary_exec[9].name = "CPU_BASED_MOV_DR_EXITING";
    primary_exec[9].mask = 0x00800000;
    primary_exec[10].name = "CPU_BASED_UNCOND_IO_EXITING";
    primary_exec[10].mask = 0x01000000;
    primary_exec[11].name = "CPU_BASED_ACTIVATE_IO_BITMAP";
    primary_exec[11].mask = 0x02000000;
    primary_exec[12].name = "CPU_BASED_ACTIVATE_MSR_BITMAP";
    primary_exec[12].mask = 0x10000000;
    primary_exec[13].name = "CPU_BASED_MONITOR_EXITING";
    primary_exec[13].mask = 0x20000000;
    primary_exec[14].name = "CPU_BASED_PAUSE_EXITING";
    primary_exec[14].mask = 0x40000000;
    primary_exec[15].name = "CPU_BASED_ACTIVATE_SECONDARY_CONTROLS";
    primary_exec[15].mask = 0x80000000;

    secondary_exec[0].name = "SECONDARY_EXEC_DESCRIPTOR_TABLE_EXITING";
    secondary_exec[0].mask = 0x00000004;
    secondary_exec[1].name = "SECONDARY_EXEC_ENABLE_RDTSCP";
    secondary_exec[1].mask = 0x00000008;
    secondary_exec[2].name = "SECONDARY_EXEC_WBINVD_EXITING";
    secondary_exec[2].mask = 0x00000040;
    secondary_exec[3].name = "SECONDARY_EXEC_PAUSE_LOOP_EXITING";
    secondary_exec[3].mask = 0x00000400;
    secondary_exec[4].name = "SECONDARY_EXEC_RDRAND_EXITING";
    secondary_exec[4].mask = 0x00000800;
    secondary_exec[5].name = "SECONDARY_EXEC_ENABLE_INVPCID";
    secondary_exec[5].mask = 0x00001000;
    secondary_exec[6].name = "SECONDARY_EXEC_ENABLE_VMCS_SHADOWING";
    secondary_exec[6].mask = 0x00004000;
    secondary_exec[7].name = "SECONDARY_EXEC_ENABLE_ENCLS_EXITING";
    secondary_exec[7].mask = 0x00008000;
    secondary_exec[8].name = "SECONDARY_EXEC_RDSEED_EXITING";
    secondary_exec[8].mask = 0x00010000;
    secondary_exec[9].name = "SECONDARY_EXEC_XSAVES";
    secondary_exec[9].mask = 0x00100000;
}

extern long int strtol (const char* str, char** endptr, int base);

/* Everything from here down is not really used anymore */
/* Exec Controls */
#define CPU_BASED_HLT_EXITING                    0x00000080
#define CPU_BASED_INVLPG_EXITING                 0x00000200
#define CPU_BASED_MWAIT_EXITING                  0x00000400
#define CPU_BASED_RDPMC_EXITING                  0x00000800
#define CPU_BASED_RDTSC_EXITING                  0x00001000
#define CPU_BASED_CR3_LOAD_EXITING               0x00008000
#define CPU_BASED_CR3_STORE_EXITING              0x00010000
#define CPU_BASED_CR8_LOAD_EXITING               0x00080000
#define CPU_BASED_CR8_STORE_EXITING              0x00100000
#define CPU_BASED_MOV_DR_EXITING                 0x00800000
#define CPU_BASED_UNCOND_IO_EXITING              0x01000000
#define CPU_BASED_ACTIVATE_IO_BITMAP             0x02000000
#define CPU_BASED_ACTIVATE_MSR_BITMAP            0x10000000
#define CPU_BASED_MONITOR_EXITING                0x20000000
#define CPU_BASED_PAUSE_EXITING                  0x40000000
#define CPU_BASED_ACTIVATE_SECONDARY_CONTROLS    0x80000000

/* Secondary Exec Controls */
#define SECONDARY_EXEC_DESCRIPTOR_TABLE_EXITING  0x00000004
#define SECONDARY_EXEC_ENABLE_RDTSCP             0x00000008
#define SECONDARY_EXEC_WBINVD_EXITING            0x00000040
#define SECONDARY_EXEC_PAUSE_LOOP_EXITING        0x00000400
#define SECONDARY_EXEC_RDRAND_EXITING            0x00000800
#define SECONDARY_EXEC_ENABLE_INVPCID            0x00001000
#define SECONDARY_EXEC_ENABLE_VMCS_SHADOWING     0x00004000
#define SECONDARY_EXEC_ENABLE_ENCLS_EXITING      0x00008000
#define SECONDARY_EXEC_RDSEED_EXITING            0x00010000
#define SECONDARY_EXEC_XSAVES                    0x00100000

/* Intel CPU Flags in CR0 */
#define X86_CR0_PE                               0x00000001 /* Enable Protected Mode    (RW) */
#define X86_CR0_MP                               0x00000002 /* Monitor Coprocessor      (RW) */
#define X86_CR0_EM                               0x00000004 /* Require FPU Emulation    (RO) */
#define X86_CR0_TS                               0x00000008 /* Task Switched            (RW) */
#define X86_CR0_ET                               0x00000010 /* Extension type           (RO) */
#define X86_CR0_NE                               0x00000020 /* Numeric Error Reporting  (RW) */
#define X86_CR0_WP                               0x00010000 /* Supervisor Write Protect (RW) */
#define X86_CR0_AM                               0x00040000 /* Alignment Checking       (RW) */
#define X86_CR0_NW                               0x20000000 /* Not Write-Through        (RW) */
#define X86_CR0_CD                               0x40000000 /* Cache Disable            (RW) */
#define X86_CR0_PG                               0x80000000 /* Paging                   (RW) */

/* Intel CPU Features in CR4 */
#define X86_CR4_VME                              0x00000001 /* enable vm86 extensions */
#define X86_CR4_PVI                              0x00000002 /* virtual interrupts flag enable */
#define X86_CR4_TSD                              0x00000004 /* disable time stamp at ipl 3 */
#define X86_CR4_DE                               0x00000008 /* enable debugging extensions */
#define X86_CR4_PSE                              0x00000010 /* enable page size extensions */
#define X86_CR4_PAE                              0x00000020 /* enable physical address extensions */
#define X86_CR4_MCE                              0x00000040 /* Machine check enable */
#define X86_CR4_PGE                              0x00000080 /* enable global pages */
#define X86_CR4_PCE                              0x00000100 /* enable performance counters at ipl 3 */
#define X86_CR4_OSFXSR                           0x00000200 /* enable fast FPU save and restore */
#define X86_CR4_OSXMMEXCPT                       0x00000400 /* enable unmasked SSE exceptions */
#define X86_CR4_UMIP                             0x00000800 /* enable UMIP */
#define X86_CR4_VMXE                             0x00002000 /* enable VMX */
#define X86_CR4_SMXE                             0x00004000 /* enable SMX */
#define X86_CR4_FSGSBASE                         0x00010000 /* enable {rd,wr}{fs,gs}base */
#define X86_CR4_PCIDE                            0x00020000 /* enable PCID */
#define X86_CR4_OSXSAVE                          0x00040000 /* enable XSAVE/XRSTOR */
#define X86_CR4_SMEP                             0x00100000 /* enable SMEP */
#define X86_CR4_SMAP                             0x00200000 /* enable SMAP */
#define X86_CR4_PKE                              0x00400000 /* enable PKE */

/* Guest/Host Masks and Read Shadows for CR0 and CR4 */
#define CR0_GUEST_HOST_MASK                      0x00006000
#define CR4_GUEST_HOST_MASK                      0x00006002
#define CR0_READ_SHADOW                          0x00006004
#define CR4_READ_SHADOW                          0x00006006

/* Enable/disable execution control */
#define ENABLE                                   1
#define DISABLE                                  0
