CC=$(CROSS_COMPILE)gcc

obj-m += pinner.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

MODULEDIRS := $(wildcard $(PWD)/modules/*)

default: $(MODULEDIRS)
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD}:modules/pinner:modules/axidma pinner

$(MODULEDIRS):
	${MAKE} -C $@

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
	
