include $(MAKEINC)

OBJECTS = kmain.o
OBJLIBS = libboot.a

LDFLAGS := -T link.ld -melf_i386

LIBS	= -L$(LIBSDIR) -lboot

.PHONY: clean

all: kernel.elf

kernel.elf: $(OBJECTS) $(OBJLIBS)
	$(ECHO) "Linking kernel.elf..."
	$(LD) -o kernel.elf $(LDFLAGS) $(OBJECTS) $(LIBS)

libboot.a: force_look
	$(ECHO) "Generating libboot.a"
	cd boot; $(MAKE) $(MFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

force_look:
	true

clean:
	$(ECHO) "Cleaning src"
	rm -rf $(OBJECTS)
	rm -rf kernel.elf
	cd boot; $(MAKE) $(MFLAGS) clean
