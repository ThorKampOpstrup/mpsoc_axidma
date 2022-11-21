CC = gcc

ifeq ($(PREFIX),)
	PREFIX := /usr
endif

LIBNAME = axidma

INSTALL_IDIR = $(DESTDIR)$(PREFIX)/include
INSTALL_LDIR = $(DESTDIR)$(PREFIX)/lib

ODIR = obj
LDIR = lib

SDIR = src
IDIR = include

_OUT = libaxidma.so

OUT = $(LDIR)/$(_OUT)
INSTALL_OUT = $(INSTALL_LDIR)/$(_OUT)

_HEADERS = axidma.h pinner_fns.h pinner.h
HEADERS = $(patsubst %,$(IDIR)/%,$(_HEADERS))
INSTALL_HEADERS = $(patsubst %,$(INSTALL_IDIR)/$(LIBNAME)/%,$(_HEADERS))

_OBJS = axidma.o pinner_fns.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

CINC = -I$(IDIR)

$(ODIR)/%.o: $(SDIR)/%.c $(ODIR)
	$(CC) -c $(CINC) -o $@ $< $(CFLAGS)

$(OUT): $(OBJS) $(LDIR)
	$(CC) -shared -o $(OUT) $(OBJS)

$(ODIR):
	mkdir $(ODIR)

$(LDIR):
	mkdir $(LDIR)

.PHONY: install
install: $(OUT) $(HEADERS)
	install -d $(INSTALL_LDIR)
	install -m 644 $(OUT) $(INSTALL_LDIR)
	install -d $(INSTALL_IDIR)/$(LIBNAME)
	install -m 644 $(HEADERS) $(INSTALL_IDIR)/$(LIBNAME)

.PHONY: uninstall
uninstall: $(INSTALL_OUT) $(INSTALL_HEADERS)
	rm -rf $(INSTALL_HEADERS)
	rm -rf $(INSTALL_OUT)

.PHONY: clean
clean:
	rm -rf $(ODIR) $(LDIR)
