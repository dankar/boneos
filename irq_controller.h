#ifndef _IRQ_CONTROLLER_H_
#define _IRQ_CONTROLLER_H_


typedef void (*irq_isr)(uint32_t addr);

void irq_controller_start();
void irq_controller_register_isr(uint32_t interrupt, irq_isr isr);
void irq_controller_isr(uint32_t addr);

#endif
