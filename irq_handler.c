#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "irq_handler.h"
#include "cpu.h"
#include "timer.h"
#include "irq_controller.h"

extern uint32_t vector_table;

void setup_irq()
{
	set_vbar(vector_table);
	invalidate_cache();

	uart_print(0, "Setting VBAR to: ");
	uart_print_hex(0, vector_table);
	uart_print(0, "\n");

	uart_print(0, "Enabling core interrupts\n");
	enable_interrupts();

	uart_print(0, "Starting IRQ controller\n");
	irq_controller_start();

	uart_print(0, "Enabling clock interrupt\n");
	irq_controller_register_isr(68, &timer2_isr);

	uart_print(0, "Start timer\n");
	timer2_enable();


//	enable_interrupt_in_controller(67);
}


void cundefined_interrupt_handler(uint32_t addr)
{
	uart_print(0, "Undefined instruction @ ");
	uart_print_hex(0, addr);
	uart_print(0, "\nHalting.\n");
	for(;;);
}

void csoftware_interrupt_handler(uint32_t func)
{
	uart_print(0, "Kernel received software interrupt. Function is ");
	uart_print_hex(0, func);
	uart_print(0, "\n");
}

void cirq_handler(uint32_t addr)
{
	irq_controller_isr(addr);
}

void cfiq_handler()
{
	uart_print(0, "fiq\n");
}

void cprefetch_abort_handler()
{
	uart_print(0, "Prefetch abort, halting\n");
	for(;;);
}

void cdata_abort_handler()
{
	uart_print(0, "Data abort, halting\n");
	for(;;);
}
