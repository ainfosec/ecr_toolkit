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

#ifndef MOD_IOCTL_H
#define MOD_IOCTL_H

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

#define IOCTL_HANDLER                                                                           \
    if (ret == -1) {                                                                            \
        perror("ioctl() failed");                                                               \
        exit(EXIT_FAILURE);                                                                     \
    }                                                                                           \
    printf("IOCTL completed with arguments: 0x%lx, 0x%lx; ioctl() returned: 0x%x; errno: %d\n", \
        arg_struct.i, arg_struct.j, ret, errno);                                                \
    break;

#endif
