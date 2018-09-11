#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/cpufeatures.h>
#include <linux/debugfs.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/sched/signal.h>

#ifndef MOD_IOCTL_H
#define MOD_IOCTL_H
#include "mod_ioctl.h"
#endif
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include "instructions.h"
#endif
#ifndef WALL_TIMER_H
#define WALL_TIMER_H
#include "wall_timer.h"
#endif
#ifndef NOP_TIMER_H
#define NOP_TIMER_H
#include "nop_timer.h"
#endif
#ifndef CACHE_TIMER_H
#define CACHE_TIMER_H
#include "cache_timer.h"
#endif
#ifndef MEMORY_TIMER_H
#define MEMORY_TIMER_H
#include "memory_timer.h"
#endif

#define KMALLOC_SIZE sizeof(uint64_t) * 64

extern int wall_timing_start(void);
extern int nop_timing_start(bool action);
extern int cache_timing_start(void);
extern int memory_timing_start(void);
