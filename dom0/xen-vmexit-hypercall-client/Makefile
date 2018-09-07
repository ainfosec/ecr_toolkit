# Makefile for Xen IOCTL hypercall client

UNAME_R=$(shell uname -r)

CC=gcc

IFLAGS=-I /usr/src/linux-headers-$(UNAME_R)/include/ -I /usr/src/linux-headers-$(UNAME_R)/arch/x86/include/

all: main

main:
	$(CC) -o xen_ioctl xen_ioctl.c $(IFLAGS)

clean:
	rm xen_ioctl
