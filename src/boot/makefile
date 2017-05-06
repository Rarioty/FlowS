include $(MAKEINC)

OBJECTS 	= loader.o

ASFLAGS := -f elf

.PHONY: clean

all: libboot.a

libboot.a: $(OBJECTS)
	$(ECHO) "Linking libboot.a"
	$(AR) $(ARFLAGS) rv $(LIBSDIR)/libboot.a $?
	$(RANLIB) $(LIBSDIR)/libboot.a

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(ECHO) "Cleaning boot"
	$(RM) -rf $(OBJECTS)
	$(RM) -rf $(LIBSDIR)/libboot.a
