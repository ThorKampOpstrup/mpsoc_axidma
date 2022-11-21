CC=$(CROSS_COMPILE)gcc
EXTRA_CFLAGS=-I/usr/include/$( gcc -dumpmachine )/asm

obj-m += axidma.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

default:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} modules

install:
	mkdir /usr/lib/modules/$( uname -r )/kernel/drivers/dma 2> /dev/null
	cp axidma.ko /usr/lib/modules/$( uname -r )/kernel/drivers/dma
	modprobe axidma

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
	
