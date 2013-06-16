#include <stdint.h>
#include "uart.h"
#include "mmio.h"

enum UART_BASE
{
	UART0 = 0x44E09000,
	UART1 = 0x48022000,
	UART2 = 0x48024000,
	UART3 = 0x481A6000,
	UART4 = 0x481A8000,
	UART5 = 0x481AA000
};

uint32_t UART_LUT[] = { UART0, UART1, UART2, UART3, UART4, UART5 };

enum
{
	MODE_A = 0x80,
	MODE_B = 0xBF,
	MODE_0 = 0x00
};

enum
{
	
	MODE0_RHR 	= 0x0,
	MODE0_THR 	= 0x0,
	MODE0_IER 	= 0x4,
	MODE0_IIR 	= 0x8,
	MODE0_LCR 	= 0xC,
	MODE0_MCR 	= 0x10,
	MODE0_LSR 	= 0x14,
	MODE0_MSR 	= 0x18,
	MODE0_TCR 	= 0x18,
	MODE0_SPR 	= 0x1C,
	MODE0_TLR 	= 0x1C,
	MODE0_MDR1	= 0x20,
	MODE0_MDR2	= 0x24,
	MODE0_SFSLR 	= 0x28,
	MODE0_TXFLL	= 0x28,
	MODE0_RESUME 	= 0x2c,
	MODE0_TXFLH 	= 0x2c,
	MODE0_SFREGL 	= 0x30,
	MODE0_RXFLL 	= 0x30,
	MODE0_SFREGH 	= 0x34,
	MODE0_RXFLH 	= 0x34,
	MODE0_BLR 	= 0x38,
	MODE0_ACREG	= 0x3c,
	MODE0_SCR	= 0x40,
	MODE0_SSR	= 0x44,
	MODE0_SSR2 	= 0x44,
	MODE0_EBLR 	= 0x48,
	MODE0_MVR 	= 0x50,
	MODE0_SYSC 	= 0x54,
	MODE0_SYSS 	= 0x58,
	MODE0_WER 	= 0x5c,
	MODE0_CFPS 	= 0x60,


	MODEA_DLL 	= 0x0,
	MODEA_DLH 	= 0x4,
	MODEA_IIR 	= 0x8,
	MODEA_FCR 	= 0x8,
	MODEA_LCR 	= 0xc,
	MODEA_MCR 	= 0x10,
	MODEA_LSR 	= 0x14,
	MODEA_MSR 	= 0x18,
	MODEA_TCR 	= 0x18,
	MODEA_SPR 	= 0x1c,
	MODEA_TLR 	= 0x1c,
	MODEA_MDR1 	= 0x20,
	MODEA_MDR2 	= 0x24,
	MODEA_SFLSR 	= 0x28,
	MODEA_TXFLL 	= 0x28,
	MODEA_RESUME 	= 0x2c,
	MODEA_TXFLH 	= 0x2c,
	MODEA_SFREGL 	= 0x30,
	MODEA_RXFLL 	= 0x30,
	MODEA_SFREGH 	= 0x34,
	MODEA_RXFLH 	= 0x34,
	MODEA_UASR 	= 0x38,
	MODEA_SCR 	= 0x40,
	MODEA_SSR 	= 0x44,
	MODEA_SSR2 	= 0x44,
	MODEA_MVR 	= 0x50,
	MODEA_SYSC 	= 0x54,
	MODEA_SYSS 	= 0x58,
	MODEA_WER 	= 0x5c,
	MODEA_CFPS 	= 0x60,

	MODEB_DLL 	= 0x0,
	MODEB_DLH 	= 0x4,
	MODEB_EFR 	= 0x8,
	MODEB_LCR 	= 0xc,
	MODEB_XON1	= 0x10,
	MODEB_ADDR1	= 0x10,
	MODEB_XON2 	= 0x14,
	MODEB_ADDR2 	= 0x14,
	MODEB_XOFF1 	= 0x18,
	MODEB_TCR 	= 0x18,
	MODEB_XOFF2 	= 0x1c,
	MODEB_TLR 	= 0x1c,
	MODEB_MDR1 	= 0x20,
	MODEB_MDR2 	= 0x24,
	MODEB_SFLSR 	= 0x28,
	MODEB_TXFLL 	= 0x28,
	MODEB_RESUME 	= 0x2c,
	MODEB_TXFLH 	= 0x2c,
	MODEB_SFREGL 	= 0x30,
	MODEB_RXFLL 	= 0x30,
	MODEB_SFREGH 	= 0x34,
	MODEB_RXFLH 	= 0x34,
	MODEB_UASR 	= 0x38,
	MODEB_SCR 	= 0x40,
	MODEB_SSR 	= 0x44,
	MODEB_SSR2 	= 0x44,
	MODEB_MVR	= 0x50,
	MODEB_SYSC 	= 0x54,
	MODEB_SYSS 	= 0x58,
	MODEB_WER 	= 0x5c,
	MODEB_CFPS 	= 0x60
	
};

enum
{
	RESETDONE = 0,
	SOFTRESET = 1,
	ENHANCED_EN = 4,
	TCR_TLR = 6,
	TXFIFOFULL = 0
};

enum
{
	ENABLE = 1,
	DISABLE = 0
};

uint8_t uart_set_mode(uint32_t uart, uint32_t mode)
{
	uint8_t old_mode = mmio_read(UART_LUT[uart] + MODE0_LCR);
	mmio_write(UART_LUT[uart] + MODE0_LCR, mode);
	return old_mode;
}

uint8_t uart_set_enhanced_mode(uint32_t uart, uint8_t set)
{
	uint8_t old_val = get_register_bit(UART_LUT[uart] + MODEB_EFR, ENHANCED_EN);
	set_register_bit_value(UART_LUT[uart] + MODEB_EFR, ENHANCED_EN, set);
	return old_val;
}

uint32_t uart_init(uint32_t uart, uint32_t baud)
{
	uint8_t old_enhanced, old_tcr_tlr, old_mode;
	// Software reset

	set_register_bit_value(UART_LUT[uart] + MODE0_SYSC, SOFTRESET, ENABLE);
	while(!get_register_bit(UART_LUT[uart] + MODE0_SYSS, RESETDONE));

	old_mode = uart_set_mode(uart, MODE_B);
	old_enhanced = uart_set_enhanced_mode(uart, ENABLE);

	uart_set_mode(uart, MODE_A);

	// Enable TLR reg
	old_tcr_tlr = set_register_bit_value(UART_LUT[uart] + MODEA_MCR, TCR_TLR, ENABLE);

	set_register(UART_LUT[uart] + MODEA_FCR, 0x1); // Enable fifo buffers, can set trigger levels

	uart_set_mode(uart, MODE_B);

	// Should maybe set SCR here for FIFO triggers

	uart_set_enhanced_mode(uart, old_enhanced);

	uart_set_mode(uart, MODE_A);

	set_register_bit_value(UART_LUT[uart] + MODEA_MCR, TCR_TLR, old_tcr_tlr);

	uart_set_mode(uart, old_mode);

	// Set baud rate and protocol

	// Enable DLL/DLH by setting bit 2:0 of MDR1
	set_register_or(UART_LUT[uart] + MODE0_MDR1, 0b111);

	uart_set_mode(uart, MODE_B);
	old_enhanced = uart_set_enhanced_mode(uart, ENABLE);
	
	// Enable access of IER reg
	uart_set_mode(uart, MODE_0);

	// Clear IER reg to change DLL/DLH
	set_register(UART_LUT[uart] + MODE0_IER, 0x0000);
	
	uart_set_mode(uart, MODE_B);

	// Load divisor!

	set_register(UART_LUT[uart] + MODEB_DLL, 0x71);
	set_register(UART_LUT[uart] + MODEB_DLH, 0x02);

	uart_set_enhanced_mode(uart, old_enhanced);

	set_register(UART_LUT[uart] + MODE0_LCR, 0b111); // 8 bit character, 1 stop bit, no parity, also reverts to mode 0

	// Clear lowest two bits of MDR1 to specify 16x mode of uart
	//clear_register_nand(UART_LUT[uart] + MODE0_MDR1, 0x3); // Clear lowest two bits of MDR1 to specify
	set_register(UART_LUT[uart] + MODE0_MDR1, 0);

	return 1;
}

void uart_putc(uint32_t uart, char c)
{
	if(c == '\n')
		uart_putc(uart, '\r');

	while(get_register_bit(UART_LUT[uart] + MODE0_SSR, TXFIFOFULL));

	set_register(UART_LUT[uart] + MODE0_THR, c);
}

void uart_print(uint32_t uart, const char* str)
{
	while(*str)
		uart_putc(uart, *str++);
}

const char hexarr[] = "0123456789ABCDEF";

void uart_print_hex(uint32_t uart, uint32_t val)
{
	int i;

	for(i = 7; i >= 0; i--)
	{
		uart_putc(uart, hexarr[(val >> i*4) & 0xf]);
	}
}

