include $(MAKEINC)

SRCS		= tty.c
OBJECTS 	= tty.o

all: libkernel.a

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

libkernel.a: $(OBJECTS)
	$(ECHO) "Linking libkernel.a"
	$(AR) $(ARFLAGS) rv $(LIBSDIR)/libkernel.a $?
	$(RANLIB) $(LIBSDIR)/libkernel.a

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGETLIB)
