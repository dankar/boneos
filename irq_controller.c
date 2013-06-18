#include <stdint.h>
#include "irq_controller.h"
#include "mmio.h"
#include "uart.h"
#include "timer.h"
#include "cpu.h"
#include "mmu.h"

enum
{
        INTC_BASE = 0x48200000,
        INTC_ILR_OFFSET = 0x100,
        INTC_ILR_STRIDE = 0x4,
};

enum
{
        INTC_SYSCONFIG  = 0x10,
        INTC_SYSSTATUS  = 0x14,
        INTC_SIR_IRQ    = 0x40,
        INTC_CONTROL    = 0x48,
        INTC_IDLE       = 0x50,

        INTC_MIR_SET0   = 0x8c,
        INTC_MIR_CLEAR0 = 0x88,

        INTC_MIR_SET1   = 0xAC,
        INTC_MIR_CLEAR1 = 0xA8,

        INTC_MIR_SET2   = 0xcc,
        INTC_MIR_CLEAR2 = 0xc8,

        INTC_MIR_SET3   = 0xEC,
        INTC_MIR_CLEAR3 = 0xE8,

};

uint32_t MIR_SET_LUT[] = { INTC_MIR_SET0, INTC_MIR_SET1, INTC_MIR_SET2, INTC_MIR_SET3 };
uint32_t MIR_CLEAR_LUT[] = { INTC_MIR_CLEAR0, INTC_MIR_CLEAR1, INTC_MIR_CLEAR2, INTC_MIR_CLEAR3 };

enum
{
        SOFTRESET = 0x1,
        RESETDONE = 0x0,
};

irq_isr irq_vectors[128];

void irq_controller_start()
{
        uint32_t INTC_BASE = 0x48200000;
	int i;

	map_mem(INTC_BASE, INTC_BASE, MMU_SECTION, USER_NO_ACCESS);

	for(i = 0; i < 128; i++)
	{
		irq_vectors[i] = 0x00000000;
	}

        set_register_bit_value(INTC_BASE + INTC_SYSCONFIG, SOFTRESET, 1);
        while(!get_register_bit(INTC_BASE + INTC_SYSSTATUS, RESETDONE));

        set_register_bit_value(INTC_BASE + INTC_CONTROL, 0, 1);
}


void irq_controller_register_isr(uint32_t interrupt, irq_isr isr)
{
        uint32_t reg;

	irq_vectors[interrupt] = isr;
        set_register(INTC_BASE + INTC_ILR_OFFSET + INTC_ILR_STRIDE * interrupt, 0x0);
        reg = interrupt/32;
        set_register_bit_value(INTC_BASE + MIR_CLEAR_LUT[reg], interrupt%32, 1);
}

void irq_controller_isr(uint32_t addr)
{
	uint32_t irq;

        irq = get_register(INTC_BASE + INTC_SIR_IRQ);
	irq_isr isr = irq_vectors[irq];

	if(!isr)
	{
		uart_print(0, "No ISR for received IRQ\n");
		for(;;);
		return;
	}

	isr(addr);
        set_register_bit_value(INTC_BASE + INTC_CONTROL, 0, 1);

        synchronize();
}
