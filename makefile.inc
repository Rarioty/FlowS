CC      = gcc
CFLAGS  = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
	-nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
AS      = nasm
ASFLAGS =
LD      = ld
LDFLAGS =
AR      = ar
ARFLAGS =
RANLIB  = ranlib
RM      = rm
ECHO    = echo

MAKE    = make
MFLAGS  = --no-print-directory

SHELL   = /bin/sh

.SILENT:
