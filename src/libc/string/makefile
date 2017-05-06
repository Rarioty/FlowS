include $(MAKEINC)

SRCS 		= strlen.c
OBJECTS 	= $(SRCS:.c=.o)
TARGETLIB 	= ../libcstring.a
TARGETLIBK  = ../libcstring_k.a

LIBK_OBJECTS = $(OBJECTS:.o=.libk.o)

all: libcstring.a

depend: .depend

.depend: $(SRCS)
	rm -f ./.depend
	$(CC) $(CFLAGS) -MM $^ -MF ./.depend

# Add it after depend command because it will raise an error otherwise
include .depend

libk: $(LIBK_OBJECTS)
	$(ECHO) "Linking $(TARGET_LIBK)"
	$(AR) $(ARFLAGS) rv $(TARGETLIBK) $?
	$(RANLIB) $(TARGETLIBK)

libcstring.a: $(OBJECTS)
	$(ECHO) "Linking $(TARGET)"
	$(AR) $(ARFLAGS) rv $(TARGETLIB) $?
	$(RANLIB) $(TARGETLIB)

%.libk.o: %.c
	$(CC) $(LIBKFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(ECHO) "Cleaning string"
	rm -rf $(OBJECTS)
	rm -rf $(LIBK_OBJECTS)
	rm -rf $(TARGETLIB)
