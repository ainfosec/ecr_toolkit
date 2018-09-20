#ifndef MAIN_H
#define MAIN_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/cpufeatures.h>
#include <linux/debugfs.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
#include <linux/sched/signal.h>
#endif

#include "mod_ioctl.h"
#include "instructions.h"
#include "wall_timer.h"
#include "nop_timer.h"
#include "cache_timer.h"
#include "memory_timer.h"

#define KMALLOC_SIZE sizeof(uint64_t) * 64

extern int wall_timing_start(void);
extern int nop_timing_start(bool action);
extern int cache_timing_start(void);
extern int memory_timing_start(void);

#endif
