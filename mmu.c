#include <stdint.h>
#include "mmu.h"
#include "cpu.h"
#include "uart.h"
#include "mmio.h"

extern uint32_t kernel_size;
extern uint32_t kernel_base;
const uint32_t ttb_base = 0x80000000;

void map_mem(uint32_t virtual_addr, uint32_t physical_addr, uint8_t type, uint8_t user_access)
{
	uint32_t *ptr = 0;
	virtual_addr = virtual_addr & 0xfff00000;
	physical_addr = physical_addr & 0xfff00000;

	ptr = (uint32_t*)(ttb_base + 4 * (virtual_addr >> (5*4)));
	(void)ptr;

	if(type != MMU_UNMAPPED)
	{
		if(user_access & 0x2)
			set_bit(&physical_addr, 11); // Set permissions
		if(user_access & 0x1)
			set_bit(&physical_addr, 10);
	}

	physical_addr |= type;

	*ptr = physical_addr;
}

void memzero(uint8_t *ptr, uint8_t val, uint32_t size)
{
	uint8_t *p;

	for(p = ptr; p < (ptr + size); p++)
		*p = val;
}

uint8_t* alloc_page(uint8_t user_access)
{
	map_mem(0x00800000, 0x80500000, MMU_SECTION, user_access);

	return (uint8_t*)0x00800000;
}



void mmu_init()
{
	memzero((uint8_t*)ttb_base, 0, 0x4000);
	map_mem(ttb_base, ttb_base, MMU_SECTION, USER_READ_ACCESS);
	map_mem(kernel_base, kernel_base, MMU_SECTION, USER_READ_ACCESS);
	map_mem(0x44e09000, 0x44e09000, MMU_SECTION, USER_READ_WRITE_ACCESS);

	set_ttb_base(ttb_base);

	printf("woop\n");
	// Set permissions for domain 0 (check permissions for each section)
	asm(
		"mov 	r0, #0x1\n"
		"mcr 	p15, 0, r0, c3, c0, 0\n"
		: : : "r0"
	);

	// Enable MMU
	asm(
		"mrc	p15, 0, r0, c1, c0, 0\n"
		"orr	r0, r0, #0x1\n"
		"mcr	p15, 0, r0, c1, c0, 0\n"
		: : : "r0"
	);

}
