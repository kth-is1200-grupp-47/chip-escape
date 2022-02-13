# This makefile builds data into object files making them easy to transfer to the ChipKIT
# Written by Hannes Mann

BUILDER = $(BINDIR)/builder
BUILDER_FILES = data/builder.c data/builder_image.c

IMAGES = $(wildcard data/images/*.png)
OBJFILES += $(IMAGES:.png=.png.o)

# Launch shell with clean environment since we're building a binary for the local machine
$(BUILDER): $(BUILDER_FILES) $(BINDIR)
	env -i bash -l -c "cc data/builder.c data/builder_image.c -o $(BUILDER) -lm"

%.png.o: %.png $(BUILDER)
	$(BUILDER) -image $< $@