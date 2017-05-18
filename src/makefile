include $(MAKEINC)

SRCS	= kmain.c
OBJECTS = kmain.o
OBJLIBS = \
	libboot.a		\
	libk.a			\
	libkernel.a

LDFLAGS := -T link.ld -melf_i386

LIBS	= -L$(LIBSDIR) -lboot -lkernel -lk

.PHONY: clean

all: kernel.elf

depend: .depend
	cd libc; $(MAKE) $(MFLAGS) depend
	cd kernel; $(MAKE) $(MFLAGS) depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

kernel.elf: $(OBJECTS) $(OBJLIBS)
	$(ECHO) "Linking kernel.elf..."
	$(LD) -o kernel.elf $(LDFLAGS) $(OBJECTS) $(LIBS)

libboot.a: force_look
	$(ECHO) "Generating libboot.a"
	cd boot; $(MAKE) $(MFLAGS)

libk.a: force_look
	$(ECHO) "Generating libc.a"
	cd libc; $(MAKE) $(MFLAGS)

libkernel.a: force_look
	$(ECHO) "Generating libkernel.a"
	cd kernel; $(MAKE) $(MFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

force_look:
	true

clean:
	$(ECHO) "Cleaning src"
	rm -rf $(OBJECTS)
	rm -rf kernel.elf
	cd boot; $(MAKE) $(MFLAGS) clean
	cd libc; $(MAKE) $(MFLAGS) clean
	cd kernel; $(MAKE) $(MFLAGS) clean

purge:
	rm -rf .depend
	cd libc; $(MAKE) $(MFLAGS) purge
	cd kernel; $(MAKE) $(MFLAGS) purge
