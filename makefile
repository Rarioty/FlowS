.PHONY: clean sources docs

CURRENTDIR  = $(shell pwd)
LIBSDIR		= $(CURRENTDIR)/libs
INCLUDEDIR  = $(CURRENTDIR)/include
MAKEINC		= $(CURRENTDIR)/makefile.inc

export LIBSDIR
export INCLUDEDIR
export MAKEINC

include $(MAKEINC)

all: sources

sources: depend
	$(ECHO) "Building sources..."
	cd src; $(MAKE) $(MFLAGS)
	$(ECHO) "DONE"

depend:
	mkdir -p extern
	mkdir -p libs
	mkdir -p doc
	mkdir -p bin
	cd src; $(MAKE) $(MFLAGS) depend

os.iso: sources
	$(ECHO) "Generating iso image..."
	cp src/kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A os                           \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o bin/os.iso                   \
                iso
	$(ECHO) "DONE"

run: os.iso
	-bochs -f extern/bochs_configuration -q

docs:
	@doxygen doxyfile.inc

clean:
	$(ECHO) "Cleaning launched"
	rm -rf bin/os.iso
	cd src; $(MAKE) $(MFLAGS) clean
	rm -rf libs

purge: clean
	cd src; $(MAKE) $(MFLAGS) purge
	rm -rf doc
