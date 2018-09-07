typedef struct {
    unsigned long i;
    unsigned long j;
} timing_ioctl_struct;

#define TIMING_IOCTL_MAGIC      0xEC2
#define TIMING_IOCTL_NOP        _IOR(TIMING_IOCTL_MAGIC, 0, timing_ioctl_struct)
#define TIMING_IOCTL_WALL       _IOR(TIMING_IOCTL_MAGIC, 1, timing_ioctl_struct)
#define TIMING_IOCTL_THREAD     _IOR(TIMING_IOCTL_MAGIC, 2, timing_ioctl_struct)
#define TIMING_IOCTL_CACHE      _IOR(TIMING_IOCTL_MAGIC, 3, timing_ioctl_struct)
#define TIMING_IOCTL_MEMORY     _IOR(TIMING_IOCTL_MAGIC, 4, timing_ioctl_struct)
#define TIMING_IOCTL_WALL_FL    _IOR(TIMING_IOCTL_MAGIC, 5, timing_ioctl_struct)
#define TIMING_IOCTL_THREAD_FL  _IOR(TIMING_IOCTL_MAGIC, 6, timing_ioctl_struct)
#define TIMING_IOCTL_KMALLOC    _IOR(TIMING_IOCTL_MAGIC, 7, timing_ioctl_struct)
#define TIMING_IOCTL_PAGEWALK   _IOR(TIMING_IOCTL_MAGIC, 8, timing_ioctl_struct)

#define IOCTL_HANDLER                                                                           \
    if (ret == -1) {                                                                            \
        perror("ioctl() failed");                                                               \
        exit(EXIT_FAILURE);                                                                     \
    }                                                                                           \
    printf("IOCTL completed with arguments: 0x%lx, 0x%lx; ioctl() returned: 0x%x; errno: %d\n", \
        arg_struct.i, arg_struct.j, ret, errno);                                                \
    break;

