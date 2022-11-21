CC=$(CROSS_COMPILE)gcc

obj-m += axidma.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

default:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} modules

.PHONY: install
install:
	-mkdir /usr/lib/modules/$( uname -r )/kernel/drivers/dma 2> /dev/null ; true
	cp axidma.ko /usr/lib/modules/$(KVERSION)/kernel/drivers/dma
	modprobe axidma

.PHONY: uninstall
uninstall:
	rm -f /usr/lib/modules/$(KVERSION)/kernel/drivers/dma/axidma.ko
	modprobe -r axidma

.PHONY: clean
clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
	
