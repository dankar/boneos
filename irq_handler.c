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

void print_regs(uint32_t *regs)
{
	uint32_t i;
	for(i = 0; i < 13; i++)
	{
		if(i != 0 && i % 4 == 0)
			printf("\n");

		printf("\tr%d = %x", i, regs[i]);
	}

	printf("\n");
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

enum
{
	TERMINAL_EXCEPTION		= 0x2,
	VECTOR_EXCEPTION		= 0x0,
	ALIGNMENT_EXCEPTION 		= 0x1,
	EXTERNAL_TRANSLATION_EXCEPTION	= 0xC,
	TRANSLATION_EXCEPTION		= 0x5,
	DOMAIN_EXCEPTION		= 0x9,
	PERMISSION_EXCEPTION		= 0xD,
	LINEFETCH_EXCEPTION		= 0x4,
	EXTERNAL_ABORT_EXCEPTION	= 0x8
};

void print_fsr_far()
{
	uint32_t fsr = 0, far = 0, arg;

	asm(
		"mrc	p15, 0, r1, c5, c0, 0\n" // get FSR
 		"mrc	p15, 0, r0, c6, c0, 0\n" // get FAR
		"mov %[fsr], r1\n"
		"mov %[far], r0\n"
		: [far]"=r"(far), [fsr]"=r"(fsr): : "r1", "r0"
	);

	fsr &= 0xf;

	arg = get_bit(fsr, 1);
	clear_bit(&fsr, 1);

	(void)arg;

	printf("\t");
	switch(fsr)
	{
	case TERMINAL_EXCEPTION:
		printf("Terminal Exception\n");
		break;
	case VECTOR_EXCEPTION:
		printf("Vector Exception\n");
		break;
	case ALIGNMENT_EXCEPTION:
		printf("Alignment Exception\n");
		break;
	case EXTERNAL_TRANSLATION_EXCEPTION:
		printf("External Abort on Translation\n");
		break;
	case TRANSLATION_EXCEPTION:
		printf("Translation Exception\n");
		break;
	case DOMAIN_EXCEPTION:
		printf("Domain Exception:\t");
		if(arg)
			printf("page\n");
		else
			printf("section\n");
		break;
	case PERMISSION_EXCEPTION:
		printf("Permission Exception:\t");
		if(arg)
			printf("page\n");
		else
			printf("section\n");
		break;
	case LINEFETCH_EXCEPTION:
		printf("External Abort on Linefetch\n");
		break;
	case EXTERNAL_ABORT_EXCEPTION:
		printf("External Abot on Non-Linefetch\n");
		break;
	}

	printf("\tFault Address: \t%x\n\n", far);
}

void cdata_abort_handler(uint32_t *regs)
{
	printf("\tData abort:\n");

	print_fsr_far();
	print_regs(regs);

	printf("Halting\n");
	for(;;);
}
