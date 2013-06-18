#include <stdint.h>
#include "mmio.h"
#include "uart.h"
#include "irq_handler.h"
#include "timer.h"
#include "mmu.h"
#include "cpu.h"


void scheduler()
{
	printf("I'm the scheduler!\n");
}

void user_mode()
{
	printf("Entering user mode\n");
	asm(
		"mrs r0, cpsr\n"
		"bic r0, #0x1f\n" // Clear mode
		"orr r0, #0x10\n" // Select user mode
		"msr cpsr, r0\n" // Enter user mode
		"ldr sp, =kernel_stack+0x10000\n"
		: : : "r0"
	);

	for(;;)
	{
		printf("I'm user mode!\n");
		asm("swi 0x123");
	}

	for(;;); // things will go wrong if we return here
}

void kernel_main(unsigned int r0, unsigned int r1)
{

	uart_init(0, 4800);

	printf("\n\n");
	printf("UART init done\n");


	printf("mmu init...");
	mmu_init();
	printf("done\n");

//	for(;;);

	setup_irq();

	timer2_register_callback(&scheduler);

	user_mode();

	uart_print(0, "Bye!\n");
}
