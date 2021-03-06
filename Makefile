# PIC32 device number
DEVICE		= 32MX340F512H

# UART settings for programmer
TTYDEV		?=/dev/ttyUSB0
TTYBAUD		?=115200

# Name of the project
PROGNAME	= chip-escape

# Linkscript
LINKSCRIPT	:= p$(shell echo "$(DEVICE)" | tr '[:upper:]' '[:lower:]').ld

# Compiler and linker flags
# TODO: Check if optimizations/LTO cause any problems
CFLAGS		+= -ffreestanding -march=mips32r2 -msoft-float -Wa,-msoft-float -std=gnu99 -Ofast -Wall -Wno-unused-function -Wno-unused-variable -flto -I"$(shell pwd)"
ASFLAGS		+= -msoft-float
LDFLAGS		+= -T $(LINKSCRIPT) -lc -s -flto

# Filenames
BINDIR      = bin
ELFFILE		= $(BINDIR)/$(PROGNAME).elf
HEXFILE		= $(BINDIR)/$(PROGNAME).hex

# Find all source files automatically
# Modified by Hannes Mann to include subdirectories
CFILES_ALL  = $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)

# Exclude files from builder directory which are built by a separate makefile
CFILES      = $(filter-out data/builder/%.c,$(CFILES_ALL))
ASFILES     = $(wildcard *.S) $(wildcard */*.S) $(wildcard */*/*.S)
SYMSFILES   = $(wildcard *.syms) $(wildcard */*.syms) $(wildcard */*/*.syms)

# Object file names
OBJFILES    = $(CFILES:.c=.c.o)
OBJFILES    +=$(ASFILES:.S=.S.o)
OBJFILES	+=$(SYMSFILES:.syms=.syms.o)

# Hidden directory for dependency files
DEPDIR = .deps
df = $(DEPDIR)/$(*F)

.PHONY: all clean install envcheck
.SUFFIXES:

all: $(HEXFILE)

# Build data into object files that are linked to the final binary
include data/Makefile

clean:
	$(RM) $(OBJFILES)
	$(RM) -R $(DEPDIR) $(BINDIR)

envcheck:
	@echo "$(TARGET)" | grep mcb32 > /dev/null || (\
		echo ""; \
		echo " **************************************************************"; \
		echo " * Make sure you have sourced the cross compiling environment *"; \
		echo " * Do this by issuing:                                        *"; \
		echo " * . /path/to/crosscompiler/environment                       *"; \
		echo " **************************************************************"; \
		echo ""; \
		exit 1)

install: envcheck
	$(TARGET)avrdude -v -p $(shell echo "$(DEVICE)" | tr '[:lower:]' '[:upper:]') -c stk500v2 -P "$(TTYDEV)" -b $(TTYBAUD) -U "flash:w:$(HEXFILE)"

$(ELFFILE): $(OBJFILES) $(BINDIR) envcheck
	$(CC) $(CFLAGS) -o $@ $(OBJFILES) $(LDFLAGS)

$(HEXFILE): $(ELFFILE) $(BINDIR) envcheck
	$(TARGET)bin2hex -a $(ELFFILE)

$(DEPDIR) $(BINDIR):
	@mkdir -p $@

# Compile C files
%.c.o: %.c envcheck | $(DEPDIR)
	$(CC) $(CFLAGS) -c -MD -o $@ $<
	@cp $*.c.d $(df).c.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.c.d >> $(df).c.P; $(RM) $*.c.d

# Compile ASM files with C pre-processor directives
%.S.o: %.S envcheck | $(DEPDIR)
	$(CC) $(CFLAGS) $(ASFLAGS) -c -MD -o $@ $<
	@cp $*.S.d $(df).S.P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $*.S.d >> $(df).S.P; $(RM) $*.S.d

# Link symbol lists to object files
%.syms.o: %.syms
	$(LD) -o $@ -r --just-symbols=$<

# Check dependencies
-include $(CFILES:%.c=$(DEPDIR)/%.c.P)
-include $(ASFILES:%.S=$(DEPDIR)/%.S.P)