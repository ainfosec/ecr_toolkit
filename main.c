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

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <asm/cpufeatures.h>
#include <linux/debugfs.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>

#include "main.h"

// These variables are used to kmalloc() memory for non-temporal detection and cache/memory timing
uintptr_t *vmem;
uintptr_t *vmem_aligned;

static int timing_open(struct inode *inode, struct file *file) {
    (void) inode;
    (void) file;

    pr_alert("IOCTL open succeeded\n");
    return 0;
}

static int timing_release(struct inode *inode, struct file *file) {
    (void) inode;
    (void) file;

    pr_alert("IOCTL release succeeded\n");
    return 0;
}

static long timing_unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {

    (void) file;

    pr_info("Command received (cmd): 0x%x\n", cmd);

    switch ( cmd ) {
        case TIMING_IOCTL_NOP:
            pr_info("Finished without doing anything (NOP requested)\n");
            break;
        case TIMING_IOCTL_WALL:
            pr_info("Commencing wall timing measurements...\n");
            return (wall_timing_start());
        case TIMING_IOCTL_THREAD:
            pr_info("Commencing NOP timing measurements...\n");
            return (nop_timing_start(1));
        case TIMING_IOCTL_CACHE:
            pr_info("Commencing cache timing measurements...\n");
            if ( !vmem ) {
                pr_alert("Kernel memory not allocated!");
                return -EFAULT;
            }
            return (cache_timing_start());
        case TIMING_IOCTL_MEMORY:
            pr_info("Commencing memory timing measurements...\n");
            if ( !vmem ) {
                pr_alert("Kernel memory not allocated!");
                return -EFAULT;
            }
            return (memory_timing_start());
        case TIMING_IOCTL_KMALLOC:
            pr_info("Allocating memory (kmalloc())\n");
            if ( !vmem ) {
                vmem = kmalloc(KMALLOC_SIZE, GFP_KERNEL);
                vmem_aligned = (uintptr_t *) ( ((uintptr_t) vmem + 15) & ~(uintptr_t) 0x0F ); // Align for load ops
            }
            if ( !*vmem || !*vmem_aligned ) {
                pr_alert("Kernel memory allocation failed!");
                return -EFAULT;
            }
            pr_info("Memory allocated at virtual address 0x%p (aligned at 0x%p) with a buffer size of %d bytes\n", \
                (void *) *vmem, (void *) *vmem_aligned, (int) KMALLOC_SIZE);
            break;
        case TIMING_IOCTL_WALL_FL:
            pr_info("Commencing buffer flush wall timing measurements...\n");
            return (buffer_flush_start());
        case TIMING_IOCTL_THREAD_FL:
            pr_info("Commencing buffer flush NOP timing measurements...\n");
            return (nop_timing_start(0));
        default:
            return -EINVAL;
    }

    return 0;
}

static struct file_operations fops = {
    .open = timing_open,
    .release = timing_release,
    .unlocked_ioctl = timing_unlocked_ioctl,
};

static struct miscdevice ecr_dev = {
    MISC_DYNAMIC_MINOR,
    "ECR",
    &fops
};

int timing_init_module(void) {
    pr_info("Inserting ECR kernel object\n");

    if ( misc_register(&ecr_dev) != 0 )
    {
        pr_alert("misc_register failed\n");
        return -EPERM;
    }

    return 0;
}

void timing_cleanup_module(void) {
    kfree(vmem);
    misc_deregister(&ecr_dev);

    pr_alert("Removing ECR kernel object\n");
}

module_init(timing_init_module);
module_exit(timing_cleanup_module);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Environmental Characterization and Reponse");
MODULE_AUTHOR("Assured Information Security, Inc");
