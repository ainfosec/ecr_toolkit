ccflags-y := -std=gnu99

obj-m += ecr.o
ecr-y := main.o instructions.o wall_timer.o nop_timer.o cache_timer.o memory_timer.o walk_page_table.o

UNAME_R = $(shell uname -r)
KDIR ?= /lib/modules/$(UNAME_R)/build

all:
		gcc -o mod_ioctl mod_ioctl.c
		make -C $(KDIR) M=$(PWD) modules

clean:
		rm mod_ioctl
		make -C $(KDIR) M=$(PWD) clean
