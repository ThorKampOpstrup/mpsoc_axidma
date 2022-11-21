CC=$(CROSS_COMPILE)gcc

obj-m += pinner.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

default:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} modules

install:
	-mkdir /usr/lib/modules/$( uname -r )/kernel/drivers/dma 2> /dev/null ; true
	cp pinner.ko /usr/lib/modules/$(KVERSION)/kernel/drivers/dma
	modprobe pinner

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
