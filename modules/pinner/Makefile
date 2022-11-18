# THIS MAKEFILE WORKS WITH MY PC -THOR
# export ARCH:=arm64
# export CROSS_COMPILE:=aarch64-linux-gnu-

CC=$(CROSS_COMPILE)gcc

obj-m += pinner.o

KVERSION := $(shell uname -r)
PWD		:= $(shell pwd)

default:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} modules

clean:
	${MAKE} -C /lib/modules/$(KVERSION)/build M=${PWD} clean
