include $(MAKEINC)

SRCS 		= putchar.c puts.c printf.c
OBJECTS 	= $(SRCS:.c=.o)
TARGETLIB 	= ../libstdio.a
TARGETLIBK  = ../libstdio_k.a

LIBK_OBJECTS = $(OBJECTS:.o=.libk.o)

all: libstdio.a

depend: .depend

.depend: $(SRCS)
	@rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ >> ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

libk: $(LIBK_OBJECTS)
	$(ECHO) "Linking $(TARGETLIBK)"
	$(AR) $(ARFLAGS) rv $(TARGETLIBK) $?
	$(RANLIB) $(TARGETLIBK)

libcstring.a: $(OBJECTS)
	$(ECHO) "Linking $(TARGETLIB)"
	$(AR) $(ARFLAGS) rv $(TARGETLIB) $?
	$(RANLIB) $(TARGETLIB)

%.libk.o: %.c
	$(CC) $(LIBKFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(ECHO) "Cleaning stdio"
	@rm -rf $(OBJECTS)
	@rm -rf $(LIBK_OBJECTS)
	@rm -rf $(TARGETLIB)

purge:
	@rm -rf .depend
