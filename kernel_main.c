#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "irq_handler.h"
#include "timer.h"


void scheduler()
{
	uart_print(0, "I'm the scheduler!\n");
}

void user_mode()
{
	uart_print(0, "Entering user mode\n");
	asm(
		"mrs r0, cpsr\n"
		"bic r0, #0x1f\n" // Clear mode
		"orr r0, #0x10\n" // Select user mode
		"msr cpsr, r0\n" // Enter user mode
		: : : "r0"
	);

	for(;;)
	{
		uart_print(0, "I'm user mode!\n");
		asm("swi 0x123");
	}

	for(;;); // things will go wrong if we return here
}

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

	timer2_register_callback(&scheduler);

	user_mode();

	uart_print(0, "Bye!\n");
}
