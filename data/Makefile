# This makefile builds data into object files making them easy to transfer to the ChipKIT
# Written by Hannes Mann

BUILDER = $(BINDIR)/builder
BUILDER_FILES = $(wildcard data/builder/*.c)

IMAGES = $(wildcard data/images/*.png)
OBJFILES += $(IMAGES:.png=.png.o)
LEVELS = $(wildcard data/levels/*.png)
OBJFILES += $(LEVELS:.png=.png.o)

# Launch shell with clean environment since we're building a binary for the local machine
$(BUILDER): $(BUILDER_FILES) $(BINDIR)
	env -i bash -l -c "cc $(BUILDER_FILES) -o $(BUILDER) -lm -I\"$(shell pwd)\""

data/images/%.png.o: data/images/%.png $(BUILDER)
	$(BUILDER) -image $< $@
data/levels/%.png.o: data/levels/%.png $(BUILDER)
	$(BUILDER) -level $< $@