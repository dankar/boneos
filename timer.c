#include <stdint.h>
#include "mmio.h"
#include "timer.h"
#include "uart.h"

enum
{
        TIMER2_BASE = 0x48040000,
};

enum
{
        TIOCP_CFG       = 0x10,
	IRQSTATUS_RAW	= 0x24,
	IRQSTATUS	= 0x28,
        IRQENABLE_SET   = 0x2c,
        TCRR            = 0x3c,
        TCLR            = 0x38,
        TLDR            = 0x40,
        TTGR            = 0x44,
        TWPS            = 0x48,
};

enum
{
        W_PEND_TMAR = 4,
        W_PEND_TTGR = 3,
        W_PEND_TLDR = 2,
        W_PEND_TCRR = 1,
        W_PEND_TCLR = 0,
};

void timer2_isr(uint32_t addr)
{

	uint32_t val;

	val = get_register(TIMER2_BASE + IRQSTATUS);

	uart_print(0, "Timer IRQ status: ");
	uart_print_hex(0, val);
	uart_print(0, "\n");

	// Acknowledge interrupt
	set_register(TIMER2_BASE + IRQSTATUS, 0x7);

}

void enable_timer2()
{
        set_register_bit_value(TIMER2_BASE + TIOCP_CFG, 0, 1);

        while(get_register_bit(TIMER2_BASE + TIOCP_CFG, 0));

        set_register_bit_value(TIMER2_BASE + IRQENABLE_SET, 1, 1); // Enable overflow interrupt

        while(get_register_bit(TIMER2_BASE + TWPS, W_PEND_TCLR));

        set_register_bit_value(TIMER2_BASE + TCLR, 1, 1); // Auto reload

        while(get_register_bit(TIMER2_BASE + TWPS, W_PEND_TLDR));

        set_register(TIMER2_BASE + TLDR, 0xfff00000); // Load value on reload

        while(get_register_bit(TIMER2_BASE + TWPS, W_PEND_TTGR));
        set_register(TIMER2_BASE + TTGR, 0x00000000);

        while(get_register_bit(TIMER2_BASE + TWPS, W_PEND_TCLR));
        set_register_bit_value(TIMER2_BASE + TCLR, 0, 1); // Start!
}
