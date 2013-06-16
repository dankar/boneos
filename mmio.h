#ifndef _MMIO_H_
#define _MMIO_H_

#include <stdint.h>

static inline void mmio_write(uint32_t reg, uint32_t data)
{
	uint32_t *ptr = (uint32_t*)reg;
	asm volatile("str %[data], [%[reg]]"
		: : [reg]"r"(ptr), [data]"r"(data));
}

static inline uint32_t mmio_read(uint32_t reg)
{
	uint32_t *ptr = (uint32_t*)reg;
	uint32_t data;

	asm volatile("ldr %[data], [%[reg]]"
		: [data]"=r"(data) : [reg]"r"(ptr));

	return data;
}


static inline uint8_t get_bit(uint32_t data, uint8_t bit)
{
	return data & (1 << bit) >> bit;
}


static inline void set_bit(uint32_t *data, uint8_t bit)
{
	*data |= (1 << bit);
}

static inline void clear_bit(uint32_t *data, uint8_t bit)
{
	*data &= ~(1 << bit);
}

static inline uint8_t set_bit_value(uint32_t *data, uint8_t bit, uint8_t val)
{
	uint8_t old_val = get_bit(*data, bit);

	if(val)
		set_bit(data, bit);
	else
		clear_bit(data, bit);

	return old_val;
}

static inline uint8_t set_register_bit_value(uint32_t reg, uint8_t bit, uint8_t set)
{
	uint32_t reg_val;
	uint8_t old_val;

	reg_val = mmio_read(reg);
	
	old_val = set_bit_value(&reg_val, bit, set);

	mmio_write(reg, reg_val);

	return old_val;
}

static inline uint8_t get_register_bit(uint32_t reg, uint8_t bit)
{
	uint32_t reg_val;
	reg_val = mmio_read(reg);

	return get_bit(reg_val, bit);
}

static inline void set_register_or(uint32_t reg, uint32_t val)
{
	uint32_t reg_val;
	reg_val = mmio_read(reg);

	reg_val |= val;

	mmio_write(reg, reg_val);
}

static inline void clear_register_nand(uint32_t reg, uint32_t val)
{
	uint32_t reg_val;
	reg_val = mmio_read(reg);
	reg_val &= ~val;
	mmio_write(reg, reg_val);
}

static inline void set_register(uint32_t reg, uint32_t val)
{
	mmio_write(reg, val);
}

static inline uint32_t get_register(uint32_t reg)
{
	return mmio_read(reg);
}

#endif
