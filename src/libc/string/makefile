include $(MAKEINC)

OBJECTS 	= strlen.o
TARGETLIB 	= ../libcstring.a

all: libcstring.a

libcstring.a: $(OBJECTS)
	$(ECHO) "Linking libcstring.a"
	$(AR) $(ARFLAGS) rv $(TARGETLIB) $?
	$(RANLIB) $(TARGETLIB)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS)
	rm -rf $(TARGETLIB)
