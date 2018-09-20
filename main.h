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
