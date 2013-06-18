#ifndef _MMU_H_
#define _MMU_H_



enum
{
        MMU_UNMAPPED 	= 0x0,
        MMU_SECTION 	= 0x2,
        MMU_LARGE_PAGE 	= 1,
        MMU_SMALL_PAGE 	= 2,
        MMU_TINY_PAGE 	= 3
};

enum
{
	USER_NO_ACCESS 		= 0x01,
	USER_READ_ACCESS 	= 0x02,
	USER_READ_WRITE_ACCESS	= 0x03
};

void mmu_init();
void map_mem(uint32_t virtual_addr, uint32_t physical_addr, uint8_t type, uint8_t user_access);
uint8_t* alloc_page(uint8_t user_access);

#endif
