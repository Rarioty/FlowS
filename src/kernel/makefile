include $(MAKEINC)

SRCS		= tty.c gdt.c
OBJECTS 	= $(SRCS:.c=.o)

TARGET 		= libkernel.a

all: libkernel.a

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

libkernel.a: $(OBJECTS)
	$(ECHO) "Linking $(TARGET)"
	$(AR) $(ARFLAGS) rv $(LIBSDIR)/$(TARGET) $?
	$(RANLIB) $(LIBSDIR)/$(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(ECHO) "Cleaning kernel"
	rm -rf $(OBJECTS)
	rm -rf $(TARGETLIB)
