CC=$(CROSS_COMPILE)gcc

obj-m += pinner.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

default:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} modules

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
	
