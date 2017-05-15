#include <kernel/devices.h>
#include <kernel/ports.h>
#include <kernel/tty.h>
#include <kernel/irq.h>
#include <kernel/io.h>
#include <stdio.h>

#define PIT_SCALE                   1193180
#define PIT_SUBTICKS_PER_TICK       100

/**
 *  \brief  Setting byte for the PIT
 *
 *  Structure of the byte:
 *      <ul>
 *      <li>bit 0:      BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD</li>
 *      <li>
 *      bit 1-3:    Operating mode:
 *          <ul>
 *          <li>0 0 0:  Mode 0 (interrupt on terminal count)</li>
 *          <li>0 0 1:  Mode 1 (hardware re-triggerable one-shot)</li>
 *          <li>0 1 0:  Mode 2 (rate generator)</li>
 *          <li>0 1 1:  Mode 3 (square wave generator)</li>
 *          <li>1 0 0:  Mode 4 (software triggered strobe)</li>
 *          <li>1 0 1:  Mode 5 (hardware triggered strobe)</li>
 *          <li>1 1 0:  Mode 2 (rate generator, same as 010b)</li>
 *          <li>1 1 1:  Mode 3 (square wave generator, same as 011b)</li>
 *          </ul>
 *      </li>
 *      <li>
 *      bit 4-5:    Access mode:
 *          <ul>
 *          <li>0 0:    Latch count value command</li>
 *          <li>0 1:    Access mode: lobyte only</li>
 *          <li>1 0:    Access mode: hibyte only</li>
 *          <li>1 1:    Access mode: lobyte/hibyte only</li>
 *          </ul>
 *      </li>
 *      <li>
 *      bit 6-7:    Select channel:
 *          <ul>
 *          <li>0 0:    Channel 0</li>
 *          <li>0 1:    Channel 1</li>
 *          <li>1 0:    Channel 2</li>
 *          <li>1 1:    Read-back command (8254 only)</li>
 *          </ul>
 *      </li>
 */
#define PIT_SET     0x36

unsigned long timer_ticks = 0;
unsigned char timer_subticks = 0;

/**
 * \brief   Set the phase (in hertz) for the PIT
 */
void clock_set_phase(int hz)
{
    int divisor = PIT_SCALE / hz;

    outb(PORT_PIT_COMMAND,      PIT_SET);
    outb(PORT_PIT_CHANNEL_0,    divisor & 0xFF);
    outb(PORT_PIT_CHANNEL_0,    (divisor >> 8) & 0xFF);
}

void clock_handler(struct regs* r)
{
    if (++timer_subticks == PIT_SUBTICKS_PER_TICK)
    {
        timer_ticks++;
        timer_subticks = 0;

        putchar('.');
    }

    (void) r;
}

void clock_install(void)
{
    terminal_special("Initializing clock driver\n", TERMINAL_INFO);
    irq_install_handler(CLOCK_IRQ, clock_handler);
    clock_set_phase(100);
}
