#ifndef _UART_H_
#define _UART_H_

uint32_t uart_init(uint32_t uart, uint32_t baud);
void uart_print(uint32_t uart, const char *str);
void uart_print_hex(uint32_t uart, uint32_t val);

#endif
