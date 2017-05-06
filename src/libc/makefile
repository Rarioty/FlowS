include $(MAKEINC)

.PHONY: clean

LIBS = \
	libcstring.a

all: libc.a

depend: force_look
	cd string; $(MAKE) $(MFLAGS) depend

libc.a: $(LIBS)
	@mkdir tmp
	mv libcstring.a tmp
	cd tmp; $(AR) -x libcstring.a
	$(ECHO) "Linking libc.a"
	cd tmp; $(AR) $(ARFLAGS) rv $(LIBSDIR)/libc.a *.o
	$(RANLIB) $(LIBSDIR)/libc.a
	@rm -rf tmp

libcstring.a: force_look
	cd string; $(MAKE) $(MFLAGS)

clean:
	cd string; $(MAKE) $(MFLAGS) clean
	rm -rf $(LIBSDIR)/libc.a

force_look:
	true
