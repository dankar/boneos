#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "irq_handler.h"

#define I_BIT 0x80
#define F_BIT 0x40

void enable_interrupts()
{
	asm(
		"mrs 	r0, cpsr\n"
		"bic 	r0, r0, #0x80|0x40\n"
		"msr 	cpsr, r0\n"
		: : : "r0"
	);
}

void disable_interrupts()
{
	asm(
		"mrs 	r0, cpsr\n"
		"orr 	r0, r0, #0x80|0x40\n"
		"msr 	cpsr, r0\n"
		: : : "r0"
	);
}

void invalidate_cache()
{
	asm(
		"mcr	p15, 0, r0, c7, c5, 0\n"
		"mcr	p15, 0, r0, c7, c10, 4\n"
		"mcr	p15, 0, r0, c7, c5, 4\n"
		: : : "r0"
	);
}

void set_vbar(uint32_t vbar)
{
	uint32_t *ptr = &vbar;
	asm(
		"ldr 	r0, [%[vbar]]\n"
		"mcr	p15, 0, r0, c12, c0, 0\n"
		: : [vbar]"r"(ptr) : "r0"
	);
}

extern uint32_t vector_table;

void setup_irq()
{

	set_vbar(vector_table);
	invalidate_cache();

	enable_interrupts();
	uart_print(0, "Setting VBAR to: ");
	uart_print_hex(0, vector_table);
	uart_print(0, "\n");
}

void swi_handler(uint32_t func)
{
	uart_print(0, "Kernel received software interrupt. Function is ");
	uart_print_hex(0, func);
	uart_print(0, "\n");
	
}
