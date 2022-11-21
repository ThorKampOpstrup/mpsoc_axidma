CC=$(CROSS_COMPILE)gcc

obj-m += pinner.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

PINNERDIR := modules/pinner

default: $(PINNERDIR)
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD}:modules/pinner:modules/axidma pinner

$(PINNERDIR):
	${MAKE} -C $@

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
	
