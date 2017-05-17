include $(MAKEINC)

DEVICES_SRCS 	= $(wildcard devices/*.c)
UTILS_SRCS 		= $(wildcard utils/*.c)
INTERRUPTS_SRCS	= $(wildcard interrupts/*.c)
MEMORY_SRCS		= $(wildcard memory/*.c)
PROCESS_SRCS 	= $(wildcard process/*.c)
SRCS			= tty.c cursor.c $(DEVICES_SRCS) $(UTILS_SRCS) $(INTERRUPTS_SRCS) $(MEMORY_SRCS) $(PROCESS_SRCS)
OBJECTS 		= $(SRCS:.c=.o) interrupts/interrupts_asm.o process/scheduler_asm.o

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
