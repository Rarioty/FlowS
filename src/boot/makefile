OBJECTS = loader.o

LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

.PHONY: clean

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o
	rm -rf kernel.elf
