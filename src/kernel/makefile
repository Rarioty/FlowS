include $(MAKEINC)

DEVICES 	= $(wildcard devices/*.c)
SRCS		= tty.c gdt.c idt.c pic.c cursor.c irq.c isrs.c $(DEVICES)
OBJECTS 	= $(SRCS:.c=.o) interrupts_asm.o

TARGET 		= libkernel.a

all: libkernel.a

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

libkernel.a: $(OBJECTS)
	$(ECHO) "Linking $(TARGET)"
	$(AR) $(ARFLAGS) rv $(LIBSDIR)/$(TARGET) $?
	$(RANLIB) $(LIBSDIR)/$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(ECHO) "Cleaning kernel"
	rm -rf $(OBJECTS)
	rm -rf $(TARGETLIB)

purge:
	rm -rf .depend
