#if !defined(__i386__)
#error "You need a ix86-elf compiler !"
#endif

int kernel_main(void)
{
    return 0xCAFEBABE;
}
