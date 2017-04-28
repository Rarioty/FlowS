OBJECTS = src/boot/loader.o

LDFLAGS = -T src/boot/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o src/boot/kernel.elf

os.iso: kernel.elf
	cp src/boot/kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
                -b boot/grub/stage2_eltorito    \
                -no-emul-boot                   \
                -boot-load-size 4               \
                -A os                           \
                -input-charset utf8             \
                -quiet                          \
                -boot-info-table                \
                -o bin/os.iso                   \
                iso

run: os.iso
	bochs -f extern/bochs_configuration -q

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf src/boot/*.o
	rm -rf src/boot/kernel.elf
	rm -rf bin/os.iso
