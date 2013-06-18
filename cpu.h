#ifndef _CPU_H_
#define _CPU_H_

static inline void synchronize()
{
        asm(
                "mov r0, #0\n"
                "mcr p15, 0, r0, c7, c10, 4\n"
                : : : "r0"
        );
}

static inline void enable_interrupts()
{
        asm(
                "mrs    r0, cpsr\n"
                "bic    r0, r0, #0x80|0x40\n"
                "msr    cpsr, r0\n"
                : : : "r0"
        );
}

static inline void disable_interrupts()
{
        asm(
                "mrs    r0, cpsr\n"
                "orr    r0, r0, #0x80|0x40\n"
                "msr    cpsr, r0\n"
                : : : "r0"
        );
}

static inline void invalidate_cache()
{
        asm(
                "mcr    p15, 0, r0, c7, c5, 0\n"
                "mcr    p15, 0, r0, c7, c10, 4\n"
                "mcr    p15, 0, r0, c7, c5, 4\n"
                : : : "r0"
        );
}

static inline void set_vbar(uint32_t vbar)
{
        uint32_t *ptr = &vbar;
        asm(
                "ldr    r0, [%[vbar]]\n"
                "mcr    p15, 0, r0, c12, c0, 0\n"
                : : [vbar]"r"(ptr) : "r0"
        );
}

static inline void set_ttb_base(uint32_t ttb_base)
{
        uint32_t *ptr = &ttb_base;
        asm(
                "ldr    r0, [%[ptr]]\n"
                "mcr    p15, 0, r0, c2, c0, 0\n"
                : : [ptr]"r"(ptr) : "r0"
        );
}

static inline uint32_t get_ttb_base()
{
	uint32_t ttb_base = 0;
        asm(
                "mrc    p15, 0, r0, c2, c0, 0\n"
		"mov    %[ttb_base], r0\n"
            : [ttb_base]"=r"(ttb_base) : : "r0"
        );

	return ttb_base;
}

#endif
