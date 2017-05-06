include $(MAKEINC)

.PHONY: clean

LIBS = \
	libcstring.a	\
	libstdio.a

LIBS_LIBK = \
	libcstring_k.a	\
	libstdio_k.a

# all: libc.a libk.a # Not ready for libc yet.
all: libk.a

depend: force_look
	cd string; $(MAKE) $(MFLAGS) depend

libc.a: $(LIBS)
	@mkdir tmp
	mv libcstring.a tmp
	mv libstdio.a tmp
	cd tmp; $(AR) -x libcstring.a
	cd tmp; $(AR) -x libstdio.a
	$(ECHO) "Linking libc.a"
	cd tmp; $(AR) $(ARFLAGS) rv $(LIBSDIR)/libc.a *.o
	$(RANLIB) $(LIBSDIR)/libc.a
	@rm -rf tmp

libk.a: $(LIBS_LIBK)
	@mkdir tmp
	mv libcstring_k.a tmp
	mv libstdio_k.a tmp
	cd tmp; $(AR) -x libcstring_k.a
	cd tmp; $(AR) -x libstdio_k.a
	$(ECHO) "Linking libk.a"
	cd tmp; $(AR) $(ARFLAGS) rv $(LIBSDIR)/libk.a *.o
	$(RANLIB) $(LIBSDIR)/libk.a
	@rm -rf tmp

libcstring.a: force_look
	cd string; $(MAKE) $(MFLAGS)
libcstring_k.a: force_look
	cd string; $(MAKE) $(MFLAGS) libk

libstdio.a: force_look
	cd stdio; $(MAKE) $(MFLAGS)
libstdio_k.a: force_look
	cd stdio; $(MAKE) $(MFLAGS) libk


clean:
	$(ECHO) "Cleaning libc"
	cd string; $(MAKE) $(MFLAGS) clean
	cd stdio; $(MAKE) $(MFLAGS) clean
	rm -rf $(LIBSDIR)/libc.a
	rm -rf $(LIBSDIR)/libk.a

force_look:
	true
