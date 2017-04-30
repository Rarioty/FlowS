.PHONY: clean

all: sources

sources:
	make -C src

os.iso: sources
	cp src/kernel.elf iso/boot/kernel.elf
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

clean:
	make -C src clean
	rm -rf bin/os.iso
