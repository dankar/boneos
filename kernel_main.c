#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "irq_handler.h"

void kernel_main(unsigned int r0, unsigned int r1)
{
	uart_init(0, 4800);
	uart_print(0, "\n\n\tBooting\n\n");
	uart_print(0, "R0: ");
	uart_print_hex(0, r0);
	uart_print(0, "\t\tR1: ");
	uart_print_hex(0, r1);
	uart_print(0, "\n\n");

	setup_irq();

	asm("swi 0x12\n");

	uart_print(0, "Bye!\n");
}
